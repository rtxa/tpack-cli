#include <iostream>
#include <string>

#include "fmt/format.h"

#include "bitmap.h"
#include "genesis.h"
#include "ram.h"

int main(int argc, char* argv[]) {
    fmt::println("Usage: imagetotxl <input image> <output image>");

    const std::string file{"C:/G3D/Genesis3D/v120/levels/gedit.txl"};
    geVFile* vfs = geVFile_OpenNewSystem(
        nullptr, GE_VFILE_TYPE_VIRTUAL, file.c_str(), nullptr,
        GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);

    if (!vfs) {
        fmt::println("Failed to open virtual file system.");
    }

    geVFile_Finder* finder = geVFile_CreateFinder(vfs, "*.*");
    if (!finder) {
        fmt::println("Could not load textures from {}", file);
        geVFile_Close(vfs);
        return 1;
    }

    geVFile_Properties properties;
    while (geVFile_FinderGetNextFile(finder) != GE_FALSE) {
        geVFile_Properties Properties;
        geVFile_FinderGetProperties(finder, &properties);
        fmt::println("Texture: {}", properties.Name);
    }

    geVFile_Close(vfs);
    return 0;
}
