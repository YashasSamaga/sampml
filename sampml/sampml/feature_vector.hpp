#ifndef SAMPML_INCLUDE_FEATURE_VECTOR_HPP
#define SAMPML_INCLUDE_FEATURE_VECTOR_HPP

#include <dlib/matrix.h>

#include "common.hpp"

namespace SAMPML_NAMESPACE {
    template <typename T, std::size_t N>
    using feature_vector = dlib::matrix<T, N, 1>;
}

#endif /* SAMPML_INCLUDE_COMMON_HPP */