#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include <dlib/geometry.h>

#include "sampml/data.hpp"
#include "sampml/feature_vector.hpp"
#include "sampml/svm_classifier.hpp"

#include "transform.hpp"

void data_transform(const sampml::data::reader<input_vector>& data, std::vector<output_vector>& samples) {
	transformer t;
    for(const auto& input : data) {
        t.submit(input);
    }
    samples = std::move(t.pool);
}

void transform() {
    sampml::data::reader<input_vector> data_positive("data/raw/positive.dat");
    std::vector<output_vector> samples_positive;
    data_transform(data_positive, samples_positive);
    sampml::data::write(samples_positive, "data/transformed/positive.dat");
    std::cout << "raw negative vectors: " << data_positive.size() << ", "
              << "transformed positive vectors: " << samples_positive.size() << std::endl;

    sampml::data::reader<input_vector> data_negative("data/raw/negative.dat");
    std::vector<output_vector> samples_negative;
    data_transform(data_negative, samples_negative);
    sampml::data::write(samples_negative, "data/transformed/negative.dat");
    std::cout << "raw negative vectors: " << data_negative.size() << ", "
              << "transformed negative vectors: " << samples_negative.size() << std::endl;    
}

void train() {
    using sample_type = output_vector;

    sampml::data::reader<sample_type> data_positive("data/transformed/positive.dat");
    sampml::data::reader<sample_type> data_negative("data/transformed/negative.dat");

    sampml::trainer::svm_classifier<sample_type> model;
    model.set_samples(data_positive, data_negative);
    model.train();

    model.serialize("models/classifier.dat");
}

void test() {
    using sample_type = output_vector;

    sampml::data::reader<sample_type> data_positive("data/transformed/positive.dat");
    sampml::data::reader<sample_type> data_negative("data/transformed/negative.dat");

    sampml::trainer::svm_classifier<sample_type> model;
    model.deserialize("models/classifier.dat");

    double sum = 0.0;
    for(auto&& v : data_positive) {
        sum = model.test(v);
    }
    std::cout << "Average positive sample probability: " << sum/data_positive.size() << std::endl;

    sum = 0.0;
    for(auto&& v : data_negative) {
        sum = model.test(v);
    }
    std::cout << "Average negative sample probability: " << sum/data_negative.size() << std::endl;
}

int main()
{
    transform();
    train();
    test();
    return 0;
}