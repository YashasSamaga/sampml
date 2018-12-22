#include <iostream>
#include <vector>

#include <dlib/dnn.h>
#include <sampml/data.hpp>
#include <sampml/random_forest.hpp>
#include <sampml/svm_classifier.hpp>

#include "common.hpp"
#include "dnn.hpp"

std::vector<output_vector> samples;
void transform (std::string_view path) {
    sampml::data::reader<input_vector> data(path);
    transformer t;
    t.submit(data);

    samples = std::move(t.pool);
}

void test() {
    sampml::trainer::svm_classifier<sample_type> svm;
    svm.deserialize(std::string(svm_model.begin(), svm_model.end()));

    sampml::trainer::random_forest<sample_type> rf;
    rf.deserialize(std::string(rf_model.begin(), rf_model.end()));

    test_network_type dnn;
    dlib::deserialize(std::string(dnn_model.begin(), dnn_model.end())) >> dnn.subnet();

    int aimbot_samples = 0,
        clean_samples = 0;
    dlib::running_stats<double> values;
    for(auto& sample : samples) {
        double svm_prob = svm.test(sample),
               rf_prob = std::clamp(rf.test(sample), 0.0, 1.0),
               dnn_prob = dlib::mat(dnn(sample))(1);
        double prob = (svm_prob + rf_prob + dnn_prob)/3;

        const float cutoff = 0.5;
        if(prob > cutoff)
            aimbot_samples++;
        else
            clean_samples++;
        values.add(prob);
    }
    std::cout << "test statistics: " << '\n';
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << '\n'
              << "min: " << values.min() << ", max: " << values.max()  << '\n'
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << '\n';

    std::cout << "samples classified as aimbot: " << aimbot_samples << '\n'
              << "samples classified as clean: " << clean_samples << '\n'
              << "possible use of aimbot: " << float(aimbot_samples)/(samples.size()) * 100 << "%\n";
}

int main (int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "Usage: " << argv[0] << " [raw data file]" << std::endl;
        return 0;
    }

    transform(argv[1]);
    test();
}