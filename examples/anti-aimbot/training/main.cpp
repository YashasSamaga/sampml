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
    dlib::randomize_samples(samples_positive);

    int training_count = 0.8 * samples_positive.size();
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

    training_count = 0.8 * samples_negative.size();
    std::vector<output_vector> samples_train_negative(samples_negative.begin(), 
                                                     samples_negative.begin() + training_count);
    std::vector<output_vector> samples_test_negative(samples_negative.begin() + training_count, 
                                                     samples_negative.end());

    sampml::data::write(samples_train_negative, "data/transformed/negative_train.dat");
    sampml::data::write(samples_test_negative, "data/transformed/negative_test.dat");
    std::cout << "raw negative vectors: " << data_negative.size() << ", "
              << "transformed negative vectors: " << samples_negative.size() << std::endl;
    std::cout << "training samples: " << training_count<< std::endl;
    std::cout << "\n\n" << std::flush;  
}

template<class PositiveCont, class NegativeCont>
void print_statistics(const PositiveCont& data_positive, const NegativeCont& data_negative) {
    std::cout << "Aggregate parameter(s) statistics: " << std::endl;
    for(int i = 0; i < 15; i++) {
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
    std::cout << "\n\n" << std::flush;
}

void train() {
    using sample_type = output_vector;

    sampml::data::reader<sample_type> data_positive_train("data/transformed/positive_train.dat");
    sampml::data::reader<sample_type> data_negative_train("data/transformed/negative_train.dat");

    print_statistics(data_positive_train, data_negative_train);

    sampml::trainer::svm_classifier<sample_type> model;
    model.set_samples(data_positive_train, data_negative_train);
    model.train();

    std::cout << "Feature Ranking: " << std::endl;
    std::cout << model.rank_features() << "\n\n" << std::flush;

    model.serialize("models/classifier.dat");    
}

void test() {
    using sample_type = output_vector;

    sampml::data::reader<sample_type> data_positive_test("data/transformed/positive_test.dat");
    sampml::data::reader<sample_type> data_negative_test("data/transformed/negative_test.dat");

    sampml::trainer::svm_classifier<sample_type> model;
    model.deserialize("models/classifier.dat");

    print_statistics(data_positive_test, data_negative_test);

    dlib::running_stats<double> values;
    for(const auto& v : data_positive_test)
        values.add(model.test(v));

    std::cout << "positive test set statistics: " << std::endl;
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << std::endl
              << "min: " << values.min() << ", max: " << values.max()  << std::endl
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << std::endl;

    values.clear();
    for(const auto& v : data_negative_test)
        values.add(model.test(v));

    std::cout << "negative test set statistics: " << std::endl;
    std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << std::endl
              << "min: " << values.min() << ", max: " << values.max()  << std::endl
              << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << std::endl;
}

int main()
{
    transform();
    train();
    test();
    return 0;
}