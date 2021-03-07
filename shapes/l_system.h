#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include <set>
#include <string>
#include <map>
#include <vector>
#include "point.h"

namespace shp
{
    class L_System
    {
    public:
        L_System();
        L_System(std::set<char> alphabet,
                 std::string initiator,
                 double alpha,
                 double delta,
                 std::map<char, std::string> replacements);
        std::pair<std::vector<shp::Point>, std::vector<std::pair<unsigned int, unsigned int>>> generate(unsigned int n);

    private:
        std::string _generate_string(unsigned int n);

        std::set<char> _alphabet; // The one symbols, not the zero symbols
        std::string _initiator;
        double _alpha;
        double _delta;
        std::map<char, std::string> _replacements;
    };
}

#endif