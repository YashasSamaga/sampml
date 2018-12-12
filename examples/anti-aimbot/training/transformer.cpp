#include <iostream>
#include <vector>

#include <dlib/statistics.h>

#include "sampml/data.hpp"
#include "transform.hpp"

void data_transform(const sampml::data::reader<input_vector>& data, std::vector<output_vector>& samples) {
	transformer t;
    t.submit(data);
    samples = std::move(t.pool);
}

void transform(float training_fraction = 0.7) {
    std::cout << "DATASET TRANSFORMATION: " << std::endl;
    sampml::data::reader<input_vector> data_positive("data/raw/positive.dat");
    std::vector<output_vector> samples_positive;
    data_transform(data_positive, samples_positive);
    dlib::randomize_samples(samples_positive);

    int training_count = training_fraction * samples_positive.size();
    std::vector<output_vector> samples_train_positive(samples_positive.begin(), 
                                                      samples_positive.begin() + training_count);
    std::vector<output_vector> samples_test_positive(samples_positive.begin() + training_count, 
                                                      samples_positive.end());

    sampml::data::write(samples_train_positive, "data/transformed/positive_train.dat");
    sampml::data::write(samples_test_positive, "data/transformed/positive_test.dat");

    std::cout << "raw positive vectors: " << data_positive.size() << ", "
              << "transformed positive vectors: " << samples_positive.size() << std::endl;
    std::cout << "training samples: " << training_count<< std::endl;

    sampml::data::reader<input_vector> data_negative("data/raw/negative.dat");
    std::vector<output_vector> samples_negative;
    data_transform(data_negative, samples_negative);
    dlib::randomize_samples(samples_negative);

    training_count = training_fraction * samples_negative.size();
    std::vector<output_vector> samples_train_negative(samples_negative.begin(), 
                                                     samples_negative.begin() + training_count);
    std::vector<output_vector> samples_test_negative(samples_negative.begin() + training_count, 
                                                     samples_negative.end());

    sampml::data::write(samples_train_negative, "data/transformed/negative_train.dat");
    sampml::data::write(samples_test_negative, "data/transformed/negative_test.dat");

    std::cout << "raw negative vectors: " << data_negative.size() << ", "
              << "transformed negative vectors: " << samples_negative.size() << std::endl;
    std::cout << "training samples: " << training_count<< std::endl;
}

int main()
{
    transform();
    return 0;
}