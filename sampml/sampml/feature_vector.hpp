#include <dlib/matrix.h>

#include "common.hpp"

namespace SAMPML_NAMESPACE {
    template <typename T, std::size_t N>
    using feature_vector = dlib::matrix<T, N, 1>;
}