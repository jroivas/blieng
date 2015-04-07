#include <blieng/data.h>
#include <iostream>

int main(int argc, char **argv)
{
    blieng::Data data;
    if (!data.initialize()) {
        std::cout << "Error reading\n";
        return 2;
    }
    if (argc <= 1) {
        std::cout << "Error file\n";
        return 3;
    }

    std::string res = data.readString(argv[1]);
    if (res == "") {
        std::cout << "No param\n";
    }

    return 0;
}
