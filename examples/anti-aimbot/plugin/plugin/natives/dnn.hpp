#ifndef ANTIAIMBOT_DNN_HPP
#define ANTIAIMBOT_DNN_HPP

#include <dlib/dnn.h>

#include "transform.hpp"

using sample_type = output_vector;

using base_network_type = 
                    dlib::fc<2,
                    dlib::relu<dlib::fc<6,
                    dlib::relu<dlib::fc<6,
                    dlib::input<sample_type> 
                    >>>>>;

using train_network_type = dlib::loss_multiclass_log<base_network_type>;
using test_network_type = dlib::softmax<base_network_type>;

#endif /* ANTIAIMBOT_DNN_HPP */