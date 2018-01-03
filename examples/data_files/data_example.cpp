//#include <blieng/blieng.h>
#include <blieng/data.h>
#include <blieng/datafile.h>
#include <blieng/json.h>

using blieng::BliengJson;

std::string genIndent(long indent)
{
    std::string res;
    for (long i = 0; i < indent; ++i) {
        res += ' ';
    }
    return res;
}

void walkJsonObject(blieng::Data &data, const BliengJson json_data, long indent);

void walkJsonArray(blieng::Data &data, const BliengJson json_data, long indent=0)
{
    if (!json_data.is_array()) {
        throw std::string("JSON data is not array");
    }
    ++indent;

    auto arrIter = json_data.cbegin();
    bool first = true;
    std::cout << genIndent(indent);
    while (arrIter != json_data.cend()) {
        if (!first) {
            std::cout << ", ";
        } else {
            first = false;
        }
        switch(arrIter->type()) {
            case BliengJson::value_t::object:
                std::cout << "\n";
                walkJsonObject(data, *arrIter, indent + 1);
                break;
            case BliengJson::value_t::array:
                std::cout << "\n";
                walkJsonArray(data, *arrIter, indent + 1);
                break;
            case BliengJson::value_t::number_integer:
            case BliengJson::value_t::number_unsigned:
            case BliengJson::value_t::number_float:
            case BliengJson::value_t::string:
            case BliengJson::value_t::boolean:
                std::cout << arrIter.value();
                break;
            default:
                std::cout << "Uknknown";
                break;
        }
        ++arrIter;
    }
    std::cout << "\n";
}

void walkJsonObject(blieng::Data &data, const BliengJson json_data, long indent=0)
{
    if (!json_data.is_object() || json_data.is_array()) {
        throw std::string("JSON data is not object");
    }

    auto json_keys = blieng::JsonKeys(json_data);
    ++indent;

    for (auto key : json_keys) {
        std::cout << genIndent(indent) << "Key: " << key << "\n";
        auto json_val = json_data[key];
        switch(json_val.type()) {
            case BliengJson::value_t::object:
                walkJsonObject(data, json_val, indent + 1);
                break;
            case BliengJson::value_t::array:
                walkJsonArray(data, json_val, indent + 1);
                break;
            case BliengJson::value_t::number_integer:
            case BliengJson::value_t::number_unsigned:
            case BliengJson::value_t::number_float:
            case BliengJson::value_t::string:
            case BliengJson::value_t::boolean:
                std::cout << genIndent(indent + 1) << json_val << "\n";
                break;
            default:
                std::cout << genIndent(indent + 1) << "Uknknown\n";
                break;
        }
    }
}

int main(int argc, char **argv)
{
    try {
        // Initialize with default settings
        // No data archive file, just plain "data" folder
        blieng::Data data;

        // We can define custom paths to search data files from
        data.setGameLocation("examples/data_files");

        // Read data/example.txt
        std::string string_data = data.readString("example.txt");

        std::cout << string_data << "\n";

        //
        auto json_data = data.readJson("example.json");

        walkJsonObject(data, json_data, 0);

        data.freeJson(json_data);
    }
    catch (std::string e) {
        std::cout << "Error: " << e << "\n";
    }
}
