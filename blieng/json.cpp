#include "json.h"

std::vector<std::string> blieng::JsonKeys(const blieng::BliengJson &val)
{
    std::vector<std::string> res;
    auto it = val.cbegin();
    while (it != val.cend()) {
        res.push_back(it.key());
        ++it;
    }
    return res;
}
