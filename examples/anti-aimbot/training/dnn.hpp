#ifndef ANTIAIMBOT_DNN_HPP
#define ANTIAIMBOT_DNN_HPP

#include <dlib/dnn.h>

#include "transform.hpp"

using sample_type = output_vector;

using aa_network_type = dlib::loss_multiclass_log<
                    dlib::fc<2,
                    dlib::prelu<dlib::fc<8,
                    dlib::prelu<dlib::fc<10,
                    dlib::input<sample_type> 
                    >>>>>>;

#endif /* ANTIAIMBOT_DNN_HPP */