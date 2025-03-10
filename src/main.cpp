#include <iostream>
#include "my_lib.h"

int main() {
    std::cout << "Hello, CMake!" << std::endl;
    std::cout << "2 + 3 = " << add(2, 3) << std::endl;
    #ifdef __FLAG
      std::cout << "FLAG SET" << std::endl;
    #endif
    return 0;
}
