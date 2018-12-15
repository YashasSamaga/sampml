#include <iostream>
#include <vector>

#include <dlib/statistics.h>
#include <sampml/data.hpp>

#include "common.hpp"
#include "transform.hpp"

template<class PositiveCont, class NegativeCont>
void print_statistics(const PositiveCont& data_positive, const NegativeCont& data_negative) {
    static_assert(PositiveCont::value_type::NR == NegativeCont::value_type::NR);

    std::cout << "Aggregate parameter(s) statistics:\n";
    for(int i = 0; i < PositiveCont::value_type::NR; i++) {
        dlib::running_stats<double> values;
        for(const auto& v : data_positive)
            values.add(v(i));
        std::cout << "positive field " << i << ":\n";
        std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << '\n'
                  << "min: " << values.min() << ", max: " << values.max()  << '\n'
                  << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << "\n\n";
        values.clear();

        for(const auto& v : data_negative)
            values.add(v(i));
        std::cout << "negative field " << i << ":\n";
        std::cout << "average: " << values.mean() << ", stddev: " << values.stddev() << '\n'
                  << "min: " << values.min() << ", max: " << values.max()  << '\n'
                  << "skewness: " << values.skewness() << ", excess kurtosis: " << values.ex_kurtosis() << '\n';
        
        std::cout << "\n\n";
    }
}

int main () {
    using sample_type = output_vector;
    sampml::data::reader<sample_type> data_positive_train(positive_train_data);
    sampml::data::reader<sample_type> data_positive_test(positive_test_data);

    std::vector<sample_type> data_positive;
    data_positive.insert(data_positive.end(), data_positive_train.begin(), data_positive_train.end());
    data_positive.insert(data_positive.end(), data_positive_test.begin(), data_positive_test.end());

    sampml::data::reader<sample_type> data_negative_train(negative_train_data);
    sampml::data::reader<sample_type> data_negative_test(negative_test_data);   

    std::vector<sample_type> data_negative;
    data_negative.insert(data_negative.end(), data_negative_train.begin(), data_negative_train.end());
    data_negative.insert(data_negative.end(), data_negative_test.begin(), data_negative_test.end());

    print_statistics(data_positive, data_negative);
    return 0;
}