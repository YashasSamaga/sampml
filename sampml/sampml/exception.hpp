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
}

#endif /* SAMPML_INCLUDE_EXCEPTION_HPP */