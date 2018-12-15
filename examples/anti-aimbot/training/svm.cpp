#include <iostream>
#include <vector>
#include <cmath>

#include <dlib/geometry.h>
#include <dlib/statistics.h>

#include "sampml/data.hpp"
#include "sampml/svm_classifier.hpp"

#include "common.hpp"
#include "transform.hpp"

using sample_type = output_vector;

void train() {
    std::cout << "TRAINING:\n";

    sampml::data::reader<sample_type> data_positive_train(positive_train_data);
    sampml::data::reader<sample_type> data_negative_train(negative_train_data);

    sampml::trainer::svm_classifier<sample_type> model;
    model.set_samples(data_positive_train, data_negative_train);
    model.train();

    std::cout << '\n';
    std::cout << "Feature Ranking:\n";
    std::cout << model.rank_features();

    model.serialize(std::string(svm_model.begin(), svm_model.end()));
    std::cout << "\n\n";
}

void test() {
    const float cutoff = 0.5;

    std::cout << "TESTING:\n";
    sampml::data::reader<sample_type> data_positive_test(positive_test_data);
    sampml::data::reader<sample_type> data_negative_test(negative_test_data);

    sampml::trainer::svm_classifier<sample_type> model;
    model.deserialize(std::string(svm_model.begin(), svm_model.end()));

    int true_positives = 0,
        false_positives = 0,
        true_negatives = 0,
        false_negatives = 0;
    dlib::running_stats<double> values;
    for(const auto& v : data_positive_test) {
        double prob = model.test(v);
        if(prob > cutoff)
            true_positives++;
        else
            false_negatives++;
        values.add(prob);
    }
    std::cout << "positive test set statistics: " << '\n';
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << '\n'
              << "min: " << values.min() << ", max: " << values.max()  << '\n'
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << '\n';
    values.clear();

    for(const auto& v : data_negative_test) {
        double prob = model.test(v);
        if(prob < cutoff)
            true_negatives++;
        else
            false_positives++;
        values.add(prob);
    }
    std::cout << "negative test set statistics: " << '\n';
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << '\n'
              << "min: " << values.min() << ", max: " << values.max()  << '\n'
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << '\n';

    int num_correct = true_positives + true_negatives,
        num_wrong = false_positives + false_negatives;
    std::cout << "\n\n";
    std::cout << "true positives: " << true_positives << ", false positives: " << false_positives << '\n';
    std::cout << "true negatives: " << true_negatives << ", false negatives: " << false_negatives << '\n';
    std::cout << "number of samples classified corretly: " << num_correct << '\n';
    std::cout << "number of samples classified incorrectly: " << num_wrong << '\n';
    std::cout << "accuracy: " << num_correct/float(num_correct + num_wrong) << '\n';
}

int main()
{
    train();
    test();
    return 0;
}