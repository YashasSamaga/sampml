#include <cassert>

#include <dlib/svm.h>
#include <dlib/svm_threaded.h>
#include <dlib/global_optimization.h>

#include "common.hpp"
#include "data.hpp"

namespace SAMPML_NAMESPACE {
    namespace trainer {
        template <class sample_type>
        class svm_classifier {
        public:
            svm_classifier() { }
            
            template <class SampleContainerPositive, class SampleContainerNegative>
            void set_samples(const SampleContainerPositive& positives, const SampleContainerNegative& negatives) {
                static_assert(std::is_same<typename SampleContainerPositive::value_type, sample_type>());
                static_assert(std::is_same<typename SampleContainerNegative::value_type, sample_type>());

                samples.clear();
                samples.insert(samples.end(), positives.cbegin(), positives.cend());
                samples.insert(samples.end(), negatives.cbegin(), negatives.cend());
                labels.clear();
                labels.insert(labels.end(), positives.size(), +1);
                labels.insert(labels.end(), negatives.size(), -1);
            }
            
            void train (double beta_squared = 1.0) {
                if(samples.size() == 0) {
                    throw bad_input("Bad Input: no samples were provided for training");
                }
                assert(labels.size() == samples.size());

                dlib::vector_normalizer<sample_type> normalizer;
                normalizer.train(samples);
                for(auto& vector : samples)
                    vector = normalizer(vector);

                dlib::randomize_samples(samples, labels);

                auto cross_validation_score = 
                [this, beta_squared](double gamma, double cost_positive, double cost_negative) {
                    dlib::svm_c_trainer<svm_kernel_type> trainer;
                    trainer.set_kernel(svm_kernel_type(gamma));
                    trainer.set_c_class1(cost_positive);
                    trainer.set_c_class2(cost_negative);

                    dlib::matrix<double> result = dlib::cross_validate_trainer(trainer, this->samples, this->labels, 10);
                    std::cout << "gamma: " << gamma << "  c1: " << cost_positive <<  "  c2: " << cost_negative <<  "  cross validation accuracy: " << result;

                    return (1.0 + beta_squared)*dlib::prod(result)/(beta_squared * result(0) + result(1));
                };

                auto result = dlib::find_max_global(dlib::default_thread_pool(), 
                                                    cross_validation_score, 
                                                    {1e-5, 1e-5, 1e-5}, {100,  1e6,  1e6},
                                                    dlib::max_function_calls(50));

                best_gamma = result.x(0);
                best_cost_positive = result.x(1);
                best_cost_negative = result.x(2);

                dlib::svm_c_trainer<svm_kernel_type> trainer;
                trainer.set_kernel(svm_kernel_type(best_gamma));
                trainer.set_c_class1(best_cost_positive);
                trainer.set_c_class2(best_cost_negative);
 
                classifier.normalizer = normalizer;
                classifier.function = dlib::train_probabilistic_decision_function(trainer, samples, labels, 3);
            }

            auto rank_features() {
                dlib::kcentroid<svm_kernel_type> kc(svm_kernel_type(best_gamma), 0.0001);
                return dlib::rank_features(kc, samples, labels);
            } 

            void serialize(std::string classifier) {
                dlib::serialize(classifier) << this->classifier;
            }

            void deserialize(std::string classifier) {
                dlib::deserialize(classifier) >> this->classifier;
            }

            double test(const sample_type& sample) {
                return classifier(sample);
            }

        protected:
            std::vector<sample_type> samples;
            std::vector<double> labels;

            using svm_kernel_type = dlib::radial_basis_kernel<sample_type>;
            using decision_funct_type = dlib::probabilistic_decision_function<svm_kernel_type>;
            using normalized_decision_funct_type = dlib::normalized_function<decision_funct_type>;
            normalized_decision_funct_type classifier;
        
            double best_gamma;
            double best_cost_positive;
            double best_cost_negative;
        };
    }
}
