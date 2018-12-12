#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include <dlib/geometry.h>
#include <dlib/statistics.h>

#include "sampml/data.hpp"
#include "sampml/feature_vector.hpp"
#include "sampml/svm_classifier.hpp"

#include "transform.hpp"

const std::string model_location = "models/svm_classifier.dat";
const std::string positive_train_data = "data/transformed/positive_train.dat";
const std::string negative_train_data = "data/transformed/negative_train.dat";
const std::string positive_test_data = "data/transformed/positive_test.dat";
const std::string negative_test_data = "data/transformed/negative_test.dat";

template<class PositiveCont, class NegativeCont>
void print_statistics(const PositiveCont& data_positive, const NegativeCont& data_negative) {
    static_assert(PositiveCont::value_type::NR == NegativeCont::value_type::NR);

    std::cout << "Aggregate parameter(s) statistics: " << std::endl;
    for(int i = 0; i < PositiveCont::value_type::NR; i++) {
        dlib::running_stats<double> values;
        for(const auto& v : data_positive)
            values.add(v(i));

        std::cout << "positive field " << i << ": "  << std::endl;
        std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << std::endl
                  << "min: " << values.min() << ", max: " << values.max()  << std::endl
                  << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << std::endl;

        values.clear();
        std::cout << std::endl;
        for(const auto& v : data_negative)
            values.add(v(i));

        std::cout << "negative field " << i << ": " << std::endl;
        std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << std::endl
                  << "min: " << values.min() << ", max: " << values.max()  << std::endl
                  << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << std::endl;
        std::cout << "\n\n" << std::flush;
    }
    std::cout << '\n' << std::endl;
}

void train() {
    std::cout << "TRAINING: " << std::endl;

    using sample_type = output_vector;
    sampml::data::reader<sample_type> data_positive_train(positive_train_data);
    sampml::data::reader<sample_type> data_negative_train(negative_train_data);
    print_statistics(data_positive_train, data_negative_train);

    sampml::trainer::svm_classifier<sample_type> model;
    model.set_samples(data_positive_train, data_negative_train);
    model.train();

    std::cout << '\n';
    std::cout << "Feature Ranking: " << std::endl;
    std::cout << model.rank_features();

    model.serialize(model_location);
    std::cout << '\n' << std::endl;
}

void test() {
    std::cout << "TESTING: " << std::endl;

    using sample_type = output_vector;
    sampml::data::reader<sample_type> data_positive_test(positive_test_data);
    sampml::data::reader<sample_type> data_negative_test(negative_test_data);
    print_statistics(data_positive_test, data_negative_test);

    sampml::trainer::svm_classifier<sample_type> model;
    model.deserialize(model_location);

    int true_positives = 0,
        false_positives = 0,
        true_negatives = 0,
        false_negatives = 0;
    dlib::running_stats<double> values;
    for(const auto& v : data_positive_test) {
        double prob = model.test(v);
        if(prob > 0.5)
            true_positives++;
        else
            false_negatives++;
        values.add(prob);
    }

    std::cout << "positive test set statistics: " << std::endl;
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << std::endl
              << "min: " << values.min() << ", max: " << values.max()  << std::endl
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << std::endl;

    values.clear();
    for(const auto& v : data_negative_test) {
        double prob = model.test(v);
        if(prob < 0.5)
            true_negatives++;
        else
            false_positives++;
        values.add(prob);
    }
    std::cout << "negative test set statistics: " << std::endl;
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << std::endl
              << "min: " << values.min() << ", max: " << values.max()  << std::endl
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << std::endl;

    int num_correct = true_positives + true_negatives,
        num_wrong = false_positives + false_negatives;
    std::cout << '\n' << std::endl;
    std::cout << "true positives: " << true_positives << ", false positives: " << false_positives << '\n';
    std::cout << "true negatives: " << true_negatives << ", false negatives: " << false_negatives << '\n';
    std::cout << "number of samples classified corretly: " << num_correct << '\n';
    std::cout << "number of samples classified incorrectly: " << num_wrong << '\n';
    std::cout << "accuracy: " << num_correct/float(num_correct + num_wrong) << std::endl;
}

int main()
{
    train();
    test();
    return 0;
}