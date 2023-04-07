#include <iostream>
#include "Utils/HttpConnect.h"
int main() {
    HttpConnect conn;
    std::cout << "Hello, World!" << std::endl;
    std::cout << conn.readFromUrl("https://raw.githubusercontent.com/lr8soft/MoreElectricTools/master/README.txt");
    return 0;
}
