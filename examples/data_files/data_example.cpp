//#include <blieng/blieng.h>
#include <blieng/data.h>
#include <blieng/datafile.h>
#include <blieng/json.h>

std::string genIndent(long indent)
{
    std::string res;
    for (long i = 0; i < indent; ++i) {
        res += ' ';
    }
    return res;
}

void walkJsonObject(blieng::Data &data, const json_value* json_data, long indent);

void walkJsonArray(blieng::Data &data, const json_value* json_data, long indent=0)
{
    if (!json_data->isArray()) {
        throw std::string("JSON data is not array");
    }
    ++indent;

    auto arrIter = json_data->u.array.begin();
    bool first = true;
    std::cout << genIndent(indent);
    while (arrIter != json_data->u.array.end()) {
        if (!first) {
            std::cout << ", ";
        } else {
            first = false;
        }
        switch((*arrIter)->type) {
            case json_type::json_object:
                std::cout << "\n";
                walkJsonObject(data, *arrIter, indent + 1);
                break;
            case json_type::json_array:
                std::cout << "\n";
                walkJsonArray(data, *arrIter, indent + 1);
                break;
            case json_type::json_integer:
                std::cout << (*arrIter)->asInt();
                break;
            case json_type::json_uinteger:
                std::cout << (*arrIter)->asUInt();
                break;
            case json_type::json_double:
                std::cout << (*arrIter)->asDouble();
                break;
            case json_type::json_string:
                std::cout << (*arrIter)->asString();
                break;
            case json_type::json_boolean:
                std::cout << (*arrIter)->asBool();
                break;
            default:
                std::cout << "Uknknown";
                break;
        }
        ++arrIter;
    }
    std::cout << "\n";
}

void walkJsonObject(blieng::Data &data, const json_value* json_data, long indent=0)
{
    if (!json_data->isObject()) {
        throw std::string("JSON data is not object");
    }

    auto json_keys = data.getJsonKeys(json_data);
    ++indent;

    for (auto key : json_keys) {
        std::cout << genIndent(indent) << "Key: " << key << "\n";
        auto json_val = data.getJsonValue(json_data, key);
        switch((json_val)->type) {
            case json_type::json_object:
                walkJsonObject(data, json_val, indent + 1);
                break;
            case json_type::json_array:
                walkJsonArray(data, json_val, indent + 1);
                break;
            case json_type::json_integer:
                std::cout << genIndent(indent + 1) << (json_val)->asInt() << "\n";
                break;
            case json_type::json_uinteger:
                std::cout << genIndent(indent + 1) << (json_val)->asUInt() << "\n";
                break;
            case json_type::json_double:
                std::cout << genIndent(indent + 1) << (json_val)->asDouble() << "\n";
                break;
            case json_type::json_string:
                std::cout << genIndent(indent + 1) << (json_val)->asString() << "\n";
                break;
            case json_type::json_boolean:
                std::cout << genIndent(indent + 1) << (json_val)->asBool() << "\n";
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
