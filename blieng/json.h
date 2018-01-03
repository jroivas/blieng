#ifndef __BLIENG_JSON_H
#define __BLIENG_JSON_H

// https://github.com/nlohmann/json
#include "json.hpp"
#include <string>
#include <vector>

namespace blieng
{

typedef nlohmann::json BliengJson;

std::vector<std::string> JsonKeys(const BliengJson &val);

}

#endif
