#ifndef SAMPML_INCLUDE_DATA_HPP
#define SAMPML_INCLUDE_DATA_HPP

#include <vector>
#include <fstream>
#include <sstream>
#include <string_view>

#include <dlib/svm.h>

#include "common.hpp"
#include "exception.hpp"
#include "feature_vector.hpp"

namespace SAMPML_NAMESPACE {
    namespace data {
        template <class sample_type>
        class reader {
        public:
            using value_type = sample_type;                      
            using reference = std::add_lvalue_reference_t<value_type>;
            using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
            using container_type = std::vector<value_type>;
            using iterator = typename container_type::iterator;
            using const_iterator = typename container_type::const_iterator; 
            using size_type = typename container_type::size_type;
            using difference_type = typename container_type::difference_type;     

            reader() = default;
            reader(std::string_view path) { open(path); }
            reader(const reader& other) : samples(other.samples) { }
            reader(reader&& other) : samples(std::move(other.samples)) { }

            reader& operator=(const reader& other) { 
                if (&other != this) {
                    samples = other.samples;
                }
                return *this;
            }

            reader& operator=(reader&& other) { 
                if (&other != this) {
                    samples = std::move(other.samples);
                }
                return *this;
            }

            void open(std::string_view path) {
                process(path);
            }

            size_type size() const { return samples.size(); }
            size_type max_size() const { return samples.max_size(); }
            bool empty() const { return samples.empty(); }

            iterator begin() { return samples.begin(); }
            iterator end() { return samples.end(); }
            const_iterator begin() const { return samples.begin(); }
            const_iterator end() const { return samples.end(); }
            const_iterator cbegin() const { return samples.cbegin(); }
            const_iterator cend() const { return samples.cend(); }            
            
            reference operator[](int index) { return samples[index]; }
            const_reference operator[](int index) const { return samples[index]; }

            reference at(int index) { return samples.at(index); }
            const_reference at(int index) const { return samples.at(index); }

            void swap(reader& other) {
                samples.swap(other.samples);
            }

        protected:
            container_type samples;
            
        private:
            void process(std::string_view path) {
                std::ifstream input_file(std::string(path.begin(), path.end()));
                if(!input_file.is_open()) { throw io_error(); }

                using T = typename value_type::type;

                samples.clear();

                std::string line;
                while (std::getline(input_file, line)) {
                    std::istringstream input(line);
                    std::istream_iterator<T> itr(input);
                    samples.push_back({});
                    for (int i = 0; i < samples.back().size(); i++) {
                        if(itr == std::istream_iterator<T>()) {
                            if(i == 0)
                                break;
                            throw bad_format("Bad Input Format: less features than expected");
                        }
                                                    
                        samples.back()(i) = *itr++;
                        
                    }
                    if(itr != std::istream_iterator<T>()) {
                        throw bad_format("Bad Input Format: more features than expected");            
                    }
                }
            }
        };

        template<class sample_type>
        void swap(reader<sample_type>& lhs, reader<sample_type>& rhs) {
            lhs.swap(rhs);
        }

        template<class sample_type>
        bool operator==(const reader<sample_type>& lhs, const reader<sample_type>& rhs) {
            return lhs.samples == rhs.samples;
        }
             	
        template<class sample_type>
        bool operator!=(const reader<sample_type>& lhs, const reader<sample_type>& rhs) {
            return lhs.samples != rhs.samples;
        }

        template<class container_type>
        void write(const container_type& samples, std::string_view path) {
            std::ofstream output_file(std::string(path.begin(), path.end()));
            if(!output_file.is_open()) { throw io_error(); }

            output_file << std::setprecision(6);
            for(const auto& sample : samples) {
                for(auto value : sample)
                    output_file << value << ' ';
                output_file << std::endl;
            }
        }
    }
}
#endif /* SAMPML_INCLUDE_DATA_HPP */
