#include "bairuofunctions.h"

namespace bairuo
{
    std::string ToString(int value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
}
