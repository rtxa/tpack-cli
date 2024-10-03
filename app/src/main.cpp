#include <iostream>
#include <string>

#include "genesis.h"
#include "bitmap.h"
#include "ram.h"

int main(int argc, char *argv[]) {
    std::cout << "Usage: imagetotxl <input image> <output image>" << std::endl;
    const std::string file{ "C:/G3D/Genesis3D/v120/levels/gedit.txl" };
    geVFile *vfs = geVFile_OpenNewSystem(nullptr, GE_VFILE_TYPE_VIRTUAL, file.c_str(), nullptr, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);

    if (!vfs) {
        std::cerr << "Failed to open virtual file system." << std::endl;
    }
    
    geVFile_Finder *finder = geVFile_CreateFinder(vfs, "*.*");
    if (!finder) {
        std::cerr << "Could not load textures from " << file << std::endl;
        geVFile_Close(vfs);
        return 1;
    }
    
    geVFile_Properties properties;
    while (geVFile_FinderGetNextFile(finder) != GE_FALSE) {
        geVFile_Properties Properties;
        geVFile_FinderGetProperties(finder, &properties);
        std::cout << properties.Name << std::endl;
    }

    geVFile_Close(vfs);
    return 0;
}