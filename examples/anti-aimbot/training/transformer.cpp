#include <iostream>
#include <vector>
#include <filesystem>

#include <sampml/data.hpp>

#include "common.hpp"
#include "transform.hpp"

void transform(float training_fraction, 
                        std::string_view source,
                        std::string_view train_dest,
                        std::string_view test_dest) {
    std::vector<output_vector> samples;
    for(auto&& entry : std::filesystem::directory_iterator(source)) {
        sampml::data::reader<input_vector> data(entry.path().c_str());
        transformer tf(data);
        samples.insert(samples.end(), tf.pool.begin(), tf.pool.end());
    }        
    dlib::randomize_samples(samples);

    int training_count = training_fraction * samples.size(),
        testing_count = samples.size() - training_count;
    sampml::data::write(train_dest, samples.begin(), 
                                             samples.begin() + training_count);
    sampml::data::write(test_dest, samples.begin() + training_count, 
                                            samples.end());

    std::cout << "transformed vectors: " << samples.size() << '\n'
              << "training samples: " << training_count << '\n'
              << "testing samples: " << testing_count << std::endl;
}

int main()
{
    constexpr float training_fraction = 0.7;

    std::cout << "positive samples:\n";
    transform(training_fraction,
                       positive_raw_data_directory,
                       positive_train_data,
                       positive_test_data);

    std::cout << "\nnegative samples:\n";
    transform(training_fraction,
                       negative_raw_data_directory,
                       negative_train_data,
                       negative_test_data);
    return 0;
}