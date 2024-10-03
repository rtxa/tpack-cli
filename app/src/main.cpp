#include <iostream>
#include <string>

#include "genesis.h"
#include "bitmap.h"
#include "ram.h"

int main(int argc, char *argv[]) {
    std::cout << "Usage: imagetotxl <input image> <output image>" << std::endl;
    std::string file{ "C:/G3D/Genesis3D/v120/levels/gedit.txl" };
    geVFile *vfs = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, file.c_str(), NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
    return 0;
}