#include <blieng/data.h>
#include <blieng/json.h>
#include <iostream>

void print_json_val(const json_value *val, int indent=0)
{
    if (val == NULL) return;

    if (val->type == json_object) {
        for (auto key : blieng::Data::getJsonKeys(val)) {
            const json_value *new_val = blieng::Data::getJsonValue(val, key);
            for (int a = 0; a < indent; ++a) {
                std::cout << " ";
            }
            std::cout << "key:" << key << "\n";
            print_json_val(new_val, indent + 2);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " datafile.json\n";
        return 1;
    }

    blieng::Data data;
    const json_value *val = nullptr;
    try {
        val = data.readJson(argv[1]);
    }
    catch (std::string s) {
        std::cout << "Exception: " << s << "\n";
        return 2;
    }
    if (val == nullptr) {
        std::cout << "Can't read file\n";
        return 3;
    }

    print_json_val(val);
}
