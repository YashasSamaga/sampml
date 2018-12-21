#ifndef ANTIAIMBOT_CONFIG_READER_HPP_INCLUDED
#define ANTIAIMBOT_CONFIG_READER_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <map>

namespace tools {
    class config_reader {
    public:
        config_reader() { }
        config_reader(std::string_view path) { open(path); }

        void open(std::string_view path) {
            file.open(std::string(path.begin(), path.end()));
            if (file.is_open())
                process();
        }

        template <class T>
        bool get(std::string key, T& ref) const {
            if (pairs.count(key)) {
                std::istringstream ss(pairs.at(key));
                ss >> ref;
                return true;
            }
            return false;
        }

        template <class T>
        bool get(std::string key, T& ref, T _default) const {
            if (get(key, ref))
                return true;
            ref = _default;
            return false;
        }

    private:
        std::ifstream file;
        std::map<std::string, std::string> pairs;
        void process() {
            std::string key, value;
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream stream(line);
                if (stream >> key) {
                    value.clear();
                    stream >> value;
                    pairs.emplace(key, value);
                }
            }
        }
    };
}

#endif /* ANTIAIMBOT_CONFIG_READER_HPP_INCLUDED */