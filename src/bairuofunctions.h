// bairuofunctions.hpp
// Author: Bairuo

#ifndef BAIRUOFUNCTIONS_H_
#define BAIRUOFUNCTIONS_H_

#include <string>
#include <sstream>
#include <iostream>

namespace bairuo
{
    std::string ToString(int value);

    template<typename T>
    void debug(const T &p)
    {
        std::cout << p << std::endl;
    }
}

#endif // BAIRUOFUNCTIONS_H_
