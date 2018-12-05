#ifndef SAMPML_INCLUDE_DATA_HPP
#define SAMPML_INCLUDE_DATA_HPP

#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

#include "common.hpp"
#include "exception.hpp"
#include "feature_vector.hpp"

namespace SAMPML_NAMESPACE {
    namespace data {
        class bad_format : public exception::exception {
        public:
            bad_format() : what_message("Bad Input Format") { }
            bad_format(const std::string& what_message): what_message(what_message) {}
            virtual ~bad_format() { }

            virtual const char* what() const throw() {
                return what_message.c_str();
            }   

        private:
            const std::string what_message;
        };

        class io_error : public exception::exception {
        public:
            io_error() : what_message("Input/Output Error") { }
            io_error(const std::string& what_message): what_message(what_message) {}
            virtual ~io_error() { }

            virtual const char* what() const throw() {
                return what_message.c_str();
            }   

        private:
            const std::string what_message;
        };

        template <class feature_vector_type>
        class reader {
        public:
            using value_type = feature_vector_type;                      
            using reference = std::add_lvalue_reference_t<value_type>;
            using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
            using container_type = std::vector<value_type>;
            using iterator = typename container_type::iterator;
            using const_iterator = typename container_type::const_iterator; 
            using size_type = typename container_type::size_type;
            using difference_type = typename container_type::difference_type;     

            reader() = delete;
            reader(std::string path) : input_file(path) { 
                if(!input_file.is_open()) { throw io_error(); }
                process();
            }

            reader& operator=(const reader& other) = delete;
            reader& operator=(reader&& other) = delete;

            size_type size() const { return data.size(); }

            iterator begin() { return data.begin(); }
            iterator end() { return data.end(); }
            iterator begin() const { return data.begin(); }
            iterator end() const { return data.end(); }
            const_iterator cbegin() const { return data.cbegin(); }
            const_iterator cend() const { return data.cend(); }            
            
            reference operator[](int index) { return data[index]; }
            const_reference operator[](int index) const { return data[index]; }

            reference at(int index) { return data.at(index); }
            const_reference at(int index) const { return data.at(index); }

        private:
            std::ifstream input_file;
            container_type data;
            
            void process() {
                using T = typename value_type::type;

                std::string line;
                while (std::getline(input_file, line)) {
                    std::istringstream input(line);
                    std::istream_iterator<T> itr(input);
                    data.push_back({});
                    for (int i = 0; i < data.back().size(); i++) {
                        if(itr == std::istream_iterator<T>())
                            throw bad_format();
                        data.back()(i) = *itr++;
                    }
                    if(itr != std::istream_iterator<T>()) {
                        throw bad_format();            
                    }
                }
            }
        };
    }
}
#endif /* INCLUDE_DATA_HPP */