#include <iostream>

#include "Display.h"


int main() {
    std::cout << "---------- Start ----------" << std::endl;

        Display display;
        display.drawField();

        getch();

    std::cout << "----------  End  ----------" << std::endl;
    return 0;
}
