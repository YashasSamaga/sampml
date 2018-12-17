#ifndef ANTIAIMBOT_COMMON_HPP
#define ANTIAIMBOT_COMMON_HPP

#include <string_view>

/* raw dataset */
constexpr std::string_view positive_raw_data = "data/raw/positive.dat";
constexpr std::string_view negative_raw_data = "data/raw/negative.dat";

/* transformed dataset */
constexpr std::string_view positive_train_data = "data/transformed/positive_train.dat";
constexpr std::string_view negative_train_data = "data/transformed/negative_train.dat";
constexpr std::string_view positive_test_data = "data/transformed/positive_test.dat";
constexpr std::string_view negative_test_data = "data/transformed/negative_test.dat";

/* model locations */
constexpr std::string_view svm_model = "models/svm_classifier.dat";
constexpr std::string_view dnn_model = "models/dnn_classifier.dat";
constexpr std::string_view rf_model = "models/rf_classifier.dat";

#endif /* ANTIAIMBOT_COMMON_HPP */