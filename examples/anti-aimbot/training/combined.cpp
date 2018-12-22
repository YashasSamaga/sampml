#include <iostream>
#include <vector>

#include <dlib/dnn.h>
#include <sampml/data.hpp>
#include <sampml/random_forest.hpp>
#include <sampml/svm_classifier.hpp>

#include "common.hpp"
#include "dnn.hpp"

int main () {
    std::cout << "TESTING:\n";
    sampml::data::reader<sample_type> data_positive_test(positive_test_data);
    sampml::data::reader<sample_type> data_negative_test(negative_test_data);

    std::vector<sample_type> testing_data;
    std::vector<unsigned long> testing_labels;
    for(auto& v : data_positive_test) {
        testing_data.push_back(v);
        testing_labels.push_back(1);
    }

    for(auto& v : data_negative_test) {
        testing_data.push_back(v);
        testing_labels.push_back(0);
    }

    sampml::trainer::svm_classifier<sample_type> svm;
    svm.deserialize(std::string(svm_model.begin(), svm_model.end()));

    sampml::trainer::random_forest<sample_type> rf;
    rf.deserialize(std::string(rf_model.begin(), rf_model.end()));

    test_network_type dnn;
    dlib::deserialize(std::string(dnn_model.begin(), dnn_model.end())) >> dnn.subnet();

    int true_positives = 0,
        false_positives = 0,
        true_negatives = 0,
        false_negatives = 0;
    dlib::running_stats<double> positive_values, negative_values;
    for (std::size_t i = 0; i < testing_data.size(); ++i)
    {
        double svm_prob = svm.test(testing_data[i]),
               rf_prob = std::clamp(rf.test(testing_data[i]), 0.0, 1.0),
               dnn_prob = dlib::mat(dnn(testing_data[i]))(1);
        double prob = (svm_prob + rf_prob + dnn_prob)/3;
        
        const float cutoff = 0.5;
        if(testing_labels[i] == 1) {
            if(prob > cutoff) {
                true_positives++;
            } else {
                false_negatives++;
                std::cout << "false negative: (SVM: " << svm_prob << ", RF: " << rf_prob << ", DNN: " << dnn_prob << ")\n";
            }
            positive_values.add(prob);
        } else {
            if(prob < cutoff) {
                true_negatives++;
            } else {
                false_positives++;
                std::cout << "false positive: (SVM: " << svm_prob << ", RF: " << rf_prob << ", DNN: " << dnn_prob << ")\n";
            }
            negative_values.add(prob);
        }        
    }

    std::cout << "positive test set statistics: " << '\n';
    std::cout << "average: " << positive_values.mean() << ", stddev: " << positive_values.stddev() << '\n'
              << "min: " << positive_values.min() << ", max: " << positive_values.max()  << '\n'
              << "skewness: " << positive_values.skewness() << ", excess kurtosis: " << positive_values.ex_kurtosis() << '\n';
    std::cout << "negative test set statistics: " << '\n';
    std::cout << "average: " << negative_values.mean() << ", stddev: " << negative_values.stddev() << '\n'
              << "min: " << negative_values.min() << ", max: " << negative_values.max()  << '\n'
              << "skewness: " << negative_values.skewness() << ", excess kurtosis: " << negative_values.ex_kurtosis() << '\n';

    int num_correct = true_positives + true_negatives,
        num_wrong = false_positives + false_negatives;
    std::cout << "\n\n";
    std::cout << "true positives: " << true_positives << ", false positives: " << false_positives << '\n';
    std::cout << "true negatives: " << true_negatives << ", false negatives: " << false_negatives << '\n';
    std::cout << "number of samples classified corretly: " << num_correct << '\n';
    std::cout << "number of samples classified incorrectly: " << num_wrong << '\n';
    std::cout << "accuracy: " << num_correct/float(num_correct + num_wrong) << '\n';
    return 0;
}