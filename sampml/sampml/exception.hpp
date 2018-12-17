#ifndef SAMPML_INCLUDE_EXCEPTION_HPP
#define SAMPML_INCLUDE_EXCEPTION_HPP

#include <exception>   

#include "common.hpp"

namespace SAMPML_NAMESPACE {
    namespace exception {
        class exception : public std::exception
        {
        public:
            exception() : what_message("Unspecified Exception") { }
            exception(std::string_view what_message): what_message(what_message) {}
            virtual ~exception() { }

            virtual const char* what() const throw() {
                return what_message.c_str();
            }        

        private:
            const std::string what_message;
        };
    }

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
    }

    namespace data {
        class bad_format : public exception::exception {
        public:
            bad_format() : what_message("Bad Input Format") { }
            bad_format(std::string_view what_message): what_message(what_message) {}
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
            io_error(std::string_view what_message): what_message(what_message) {}
            virtual ~io_error() { }

            virtual const char* what() const throw() {
                return what_message.c_str();
            }   

        private:
            const std::string what_message;
        };
    }
}

#endif /* SAMPML_INCLUDE_EXCEPTION_HPP */