#include <iostream>
#include <fstream>

#include "sampml/data.hpp"
#include "sampml/feature_vector.hpp"
#include "sampml/svm_classifier.hpp"

void train() {
    constexpr int feature_count = 22;
    using sample_type = sampml::feature_vector<double, feature_count>;

    sampml::data::reader<sample_type> data_negative("../datasets/proaim/ProaimNegative");
    std::cout << "Number of negative vectors: " << data_negative.size() << std::endl;

    sampml::data::reader<sample_type> data_positive("../datasets/proaim/ProaimPositive");
    std::cout << "Number of positive vectors: " << data_positive.size() << std::endl;

    sampml::trainer::svm_classifier<sample_type> proaim;
    proaim.set_samples(data_positive, data_negative);
    proaim.train();
    
    proaim.serialize("../models/proaim_classifier.dat");
}

void test() {
    constexpr int feature_count = 22;
    using sample_type = sampml::feature_vector<double, feature_count>;

    sampml::data::reader<sample_type> data_negative("../data/NoAimbot");
    std::cout << "Number of negative vectors: " << data_negative.size() << std::endl;

    sampml::data::reader<sample_type> data_positive("../data/Proaim");
    std::cout << "Number of positive vectors: " << data_positive.size() << std::endl;

    sampml::trainer::svm_classifier<sample_type> proaim;
    proaim.deserialize("models/proaim_classifier.dat");

    std::cout << "This +1 class example should have high probability.  Its probability is: " 
         << proaim.test(data_positive[45]) << std::endl;

    std::cout << "This +1 class example should have high probability.  Its probability is: " 
         << proaim.test(data_positive[98]) << std::endl;

    std::cout << "This -1 class example should have low probability.  Its probability is: " 
         << proaim.test(data_negative[44]) << std::endl;

    std::cout << "This -1 class example should have low probability.  Its probability is: " 
         << proaim.test(data_negative[144]) << std::endl;
}

int main()
{
    train();
    test();
    return 0;
}