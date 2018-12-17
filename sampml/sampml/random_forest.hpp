#include <cassert>

#include <dlib/svm.h>
#include <dlib/svm_threaded.h>
#include <dlib/global_optimization.h>

#include "common.hpp"
#include "data.hpp"

namespace SAMPML_NAMESPACE {
    namespace trainer {
        class bad_input : public exception::exception {
        public:
            bad_input() : what_message("Bad Input") { }
            bad_input(std::string_view what_message): what_message(what_message) {}
            virtual ~bad_input() { }

            virtual const char* what() const throw() {
                return what_message.c_str();
            }   

        private:
            const std::string what_message;
        };

        template <class sample_type>
        class random_forest {
        public:
            random_forest() { }

            template <class SamplesContainer, class LabelsContainer>
            void set_samples(const SamplesContainer& _samples, const LabelsContainer& _labels) {
                samples.clear();
                samples.insert(samples.end(), _samples.cbegin(), _samples.cend());
                labels.clear();
                labels.insert(labels.end(), _labels.end(), _labels.size());
            }
            
            template <class SamplesContainer>
            void set_samples(const SamplesContainer& positives, const SamplesContainer& negatives) {
                static_assert(std::is_same<typename SamplesContainer::value_type, sample_type>());

                samples.clear();
                samples.insert(samples.end(), positives.cbegin(), positives.cend());
                samples.insert(samples.end(), negatives.cbegin(), negatives.cend());
                labels.clear();
                labels.insert(labels.end(), positives.size(), 1.0);
                labels.insert(labels.end(), negatives.size(), 0.0);
            }
            
            void cross_validate(double min_trees = 10, double max_trees = 1000,
                                double min_frac = 0.9, double max_frac = 1.0,
                                double min_min_per_leaf = 5, double max_min_per_leaf = 50,
                                int folds = 10,
                                int max_function_calls = 50) {
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
                [this, folds](int trees, double subsample_frac, int min_sampels_per_leaf) {
                    dlib::random_forest_regression_trainer<dlib::dense_feature_extractor> trainer;
                    trainer.set_num_trees(trees);
                    trainer.set_feature_subsampling_fraction(subsample_frac);
                    trainer.set_min_samples_per_leaf(min_sampels_per_leaf);

                    dlib::matrix<double> result = dlib::cross_validate_regression_trainer(trainer, this->samples, this->labels, folds);
                    std::cout << "trees: " << trees << ", cross validation accuracy: " << result << '\n';

                    return result(0);
                };

                auto result = dlib::find_min_global(dlib::default_thread_pool(), 
                                                    cross_validation_score, 
                                                    {min_trees, min_frac, min_min_per_leaf},
                                                    {max_trees, max_frac, max_min_per_leaf},
                                                    dlib::max_function_calls(max_function_calls));

                best_num_trees = result.x(0);
                best_subsample_fraction = result.x(1);
                best_min_samples_per_leaf = result.x(2);
                std::cout << result.x << '\n';
            }

            void train () {                
                dlib::random_forest_regression_trainer<feature_extractor_type> trainer;
                trainer.set_num_trees(best_num_trees);
                trainer.set_feature_subsampling_fraction(best_subsample_fraction);
                trainer.set_min_samples_per_leaf(best_min_samples_per_leaf);

                classifier = trainer.train(samples, labels, oobs);
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
            std::vector<dlib::matrix<double, 0, 1>> samples;
            std::vector<double> labels;
            std::vector<double> oobs;

            using feature_extractor_type = dlib::dense_feature_extractor;
            using decision_funct_type = dlib::random_forest_regression_function<feature_extractor_type>;
            //using normalized_decision_funct_type = dlib::normalized_function<decision_funct_type>;
            decision_funct_type classifier;
        
            double best_num_trees = 100;
            double best_subsample_fraction = 0.25;
            double best_min_samples_per_leaf = 5;
        };
    }
}