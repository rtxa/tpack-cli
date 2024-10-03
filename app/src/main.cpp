#include <iostream>
#include <string>
#include <filesystem>

#include "fmt/format.h"

#include "bitmap.h"
#include "genesis.h"
#include "ram.h"

/**
 * @brief Add a texture to the virtual file system
 *
 * This function reads a bitmap from a file, and writes it to the virtual file
 * system as a single 8-bit bitmap.
 *
 * @param texturePath The path to the bitmap file to read
 * @param txlPath The path to the virtual file system to write to
 * @return true if successful, false otherwise
 */
static bool AddTexture(const char* texturePath, const char* txlPath) {
    /* Open the file in the virtual file system */
    geVFile* file =
        geVFile_OpenNewSystem(nullptr, GE_VFILE_TYPE_DOS, texturePath, nullptr,
                              GE_VFILE_OPEN_READONLY);

    if (!file) {
        fmt::println("Could not open {}", texturePath);
        return false;
    }

    /* Create a bitmap from the file */
    geBitmap* bitmap = geBitmap_CreateFromFile(file);
    geVFile_Close(file);
    if (!bitmap) {
        fmt::println("{} is not a valid bitmap", texturePath);
        return false;
    }

    /* Get the bitmap info */
    geBitmap_Info primaryInfo, secondaryInfo;
    geBitmap_GetInfo(bitmap, &primaryInfo, &secondaryInfo);

    /* Check if the bitmap is 8-bit */
    if (primaryInfo.Format != GE_PIXELFORMAT_8BIT) {
        fmt::println("{} is not an 8bit bitmap", texturePath);
        geBitmap_Destroy(&bitmap);
        return false;
    }

    /* Open the txl file */
    geVFile* vfs =
        geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, txlPath, NULL,
                              GE_VFILE_OPEN_CREATE | GE_VFILE_OPEN_DIRECTORY);
    if (!vfs) {
        fmt::println("Could not open file {}", txlPath);
        geBitmap_Destroy(&bitmap);
        return false;
    }

    // Extract the filename (without extension)
    std::string textureName = std::filesystem::path(texturePath).stem().string();

    // Create the file that's gonna hold the bitmap in the virtual file system
    geVFile* tempFile = geVFile_Open(vfs, textureName.c_str(), GE_VFILE_OPEN_CREATE);
    if (!tempFile) {
        fmt::println("Could not save bitmap {}", textureName.c_str());
        geVFile_Close(vfs);
        geBitmap_Destroy(&bitmap);
        return false;
    }

    // Write the bitmap to the virtual file system
    geBoolean WriteResult = geBitmap_WriteToFile(bitmap, tempFile);
    geVFile_Close(tempFile);
    if (WriteResult == GE_FALSE) {
        fmt::println("Could not save bitmap {}", textureName.c_str());
        geVFile_Close(vfs);
        geBitmap_Destroy(&bitmap);
        return false;
    }

    geBitmap_Destroy(&bitmap);
    geVFile_Close(vfs);

    return true;
}

/**
 * @brief Lists all textures in a virtual file system
 *
 * Opens a virtual file system and iterates through all files, printing the name
 * of each file to the console. Only made for debugging purposes.
 *
 * @param file The path to the virtual file system to open
 * @return true if successful, false otherwise
 */
bool ListTextures(const std::string& file) {
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
        return false;
    }

    geVFile_Properties properties;
    while (geVFile_FinderGetNextFile(finder) != GE_FALSE) {
        geVFile_Properties Properties;
        geVFile_FinderGetProperties(finder, &properties);
        fmt::println("Texture: {}", properties.Name);
    }

    geVFile_Close(vfs);
    return true;
}

int main(int argc, char* argv[]) {
    fmt::println("Usage: imagetotxl <input image> <output image>");

    const std::string texturePath{"C:/G3D/Genesis3D/v120/levels/arena.bmp"};
    const std::string txlPath{"C:/G3D/Genesis3D/v120/levels/mytexture.txl"};

    AddTexture(texturePath.c_str(), txlPath.c_str());
    ListTextures({"C:/G3D/Genesis3D/v120/levels/mytexture.txl"});
    
    return 0;
}
