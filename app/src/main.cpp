#include <iostream>
#include <string>
#include "getypes.h"

int main(int argc, char *argv[]) {
    std::cout << "Usage: imagetotxl <input image> <output image>" << std::endl;
    GE_RGBA color = {255, 0, 0, 255};
    std::cout << "Color: " << color.r << ", " << color.g << ", " << color.b << ", " << color.a << std::endl;
    return 0;
}