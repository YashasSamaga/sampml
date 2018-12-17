#include <iostream>
#include <vector>

#include <dlib/dnn.h>
#include <sampml/data.hpp>

#include "common.hpp"
#include "dnn.hpp"

void train() {
    std::cout << "TRAINING:\n";

    sampml::data::reader<sample_type> data_positive_train(positive_train_data);
    sampml::data::reader<sample_type> data_negative_train(negative_train_data);

    std::vector<sample_type> training_data;
    std::vector<unsigned long> training_labels;
    for(auto& v : data_positive_train) {
        training_data.push_back(v);
        training_labels.push_back(1);
    }

    for(auto& v : data_negative_train) {
        training_data.push_back(v);
        training_labels.push_back(0);
    }

    dlib::randomize_samples(training_data, training_labels);

    aa_network_type net;    
    dlib::dnn_trainer<aa_network_type> trainer(net);
    trainer.set_learning_rate(0.01);
    trainer.set_min_learning_rate(0.00001);
    trainer.set_mini_batch_size(32);
    trainer.set_max_num_epochs(10000); 
    trainer.be_verbose();
    trainer.train(training_data, training_labels);
    net.clean();

    dlib::serialize(std::string(dnn_model.begin(), dnn_model.end())) << net;
}

void test() {
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

    aa_network_type net;
    dlib::deserialize(std::string(dnn_model.begin(), dnn_model.end())) >> net;

    std::vector<unsigned long> predicted_labels = net(testing_data);
    int true_positives = 0,
        false_positives = 0,
        true_negatives = 0,
        false_negatives = 0;

    for (std::size_t i = 0; i < testing_data.size(); ++i)
    {
        if(testing_labels[i] == 0) {
            if (predicted_labels[i])
                false_positives++;
            else
                true_negatives++;
        } else {
            if (predicted_labels[i])
                true_positives++;
            else
                false_negatives++;
        }
    }
    int num_correct = true_positives + true_negatives,
        num_wrong = false_positives + false_negatives;
    std::cout << '\n';
    std::cout << "true positives: " << true_positives << ", false positives: " << false_positives << '\n';
    std::cout << "true negatives: " << true_negatives << ", false negatives: " << false_negatives << '\n';
    std::cout << "number of samples classified corretly: " << num_correct << '\n';
    std::cout << "number of samples classified incorrectly: " << num_wrong << '\n';
    std::cout << "accuracy: " << num_correct/float(num_correct + num_wrong);

    /* FOR dlib::loss_binary_log
        std::vector<float> predicted_labels = net(training_data);
        dlib::running_stats<float> positive_stats;
        dlib::running_stats<float> negative_stats;

        for (std::size_t i = 0; i < training_data.size(); ++i)
        {
            if (training_labels[i] < 0.5) {
                negative_stats.add(predicted_labels[i]);
            } else {
                positive_stats.add(predicted_labels[i]);
            }       
        }
    */

   /* std::cout << "positive test set statistics: " << std::endl;
    std::cout << "average: " << positive_stats.mean() << ", stddev: " << positive_stats.stddev() << std::endl
              << "min: " << positive_stats.min() << ", max: " << positive_stats.max()  << std::endl
              << "skewness: " << positive_stats.skewness() << ", excess kurtosis: " << positive_stats.ex_kurtosis() << std::endl;

    std::cout << "negative test set statistics: " << std::endl;
    std::cout << "average: " << negative_stats.mean() << ", stddev: " << negative_stats.stddev() << std::endl
              << "min: " << negative_stats.min() << ", max: " << negative_stats.max()  << std::endl
              << "skewness: " << negative_stats.skewness() << ", excess kurtosis: " << negative_stats.ex_kurtosis() << std::endl;
    */
}

int main()
{
    train();
    test();
}

