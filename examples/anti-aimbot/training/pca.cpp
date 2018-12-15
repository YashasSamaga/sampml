#include <iostream>
#include <dlib/statistics/dpca.h>

#include <sampml/data.hpp>
#include "common.hpp"
#include "transform.hpp"

using sample_type = output_vector;

void compute_pca() {
    sampml::data::reader<sample_type> data_positive_train(positive_train_data);
    sampml::data::reader<sample_type> data_negative_train(negative_train_data);

    std::vector<sample_type> samples;
    for (const auto& v : data_positive_train)
        samples.push_back(v);
    
    for (const auto& v : data_negative_train)
        samples.push_back(v);

    dlib::vector_normalizer_pca<dlib::matrix<double, 0, 0>> pca;
    pca.train(samples);

    std::cout << "Means: " << pca.means() << '\n';
    std::cout << "Stddev: " << pca.std_devs() << '\n';
    std::cout << "PCA Matrix: " << pca.pca_matrix() << '\n';
}

int main () {
    compute_pca();
    return 0;
}