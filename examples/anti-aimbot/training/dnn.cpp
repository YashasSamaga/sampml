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

    train_network_type net;  
    dlib::dnn_trainer<train_network_type> trainer(net);
    trainer.set_learning_rate(0.01);
    trainer.set_min_learning_rate(0.00001);
    trainer.set_mini_batch_size(32);
    trainer.set_max_num_epochs(25000); 
    //trainer.be_verbose();
    trainer.train(training_data, training_labels);
    std::cout << "average loss: " << trainer.get_average_loss() << "\n\n";
    net.clean();

    std::cout << net.subnet() << std::endl;
    std::cout << "params 1:\n" << dlib::mat(dlib::layer<1>(net).layer_details().get_layer_params())<< '\n';
    std::cout << "params 2:\n" << dlib::mat(dlib::layer<2>(net).layer_details().get_layer_params())<< '\n';
    std::cout << "params 3:\n" << dlib::mat(dlib::layer<3>(net).layer_details().get_layer_params())<< '\n';
    std::cout << "params 4:\n" << dlib::mat(dlib::layer<4>(net).layer_details().get_layer_params())<< '\n';
    std::cout << "params 5:\n" << dlib::mat(dlib::layer<5>(net).layer_details().get_layer_params())<< '\n';
    dlib::serialize(std::string(dnn_model.begin(), dnn_model.end())) << net.subnet();
}

void test() {
    const float cutoff = 0.5;

    std::cout << "TESTING:\n";

    sampml::data::reader<sample_type> data_positive_test(positive_test_data);
    sampml::data::reader<sample_type> data_negative_test(negative_test_data);

    test_network_type test_net;
    dlib::deserialize(std::string(dnn_model.begin(), dnn_model.end())) >> test_net.subnet();
 
    int true_positives = 0,
        false_positives = 0,
        true_negatives = 0,
        false_negatives = 0;
    dlib::running_stats<double> values;
    for(const auto& v : data_positive_test) {
        double prob = dlib::mat(test_net(v))(1);
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
        double prob = dlib::mat(test_net(v))(1);
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
}

