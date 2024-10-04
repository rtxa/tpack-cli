#include <filesystem>
#include <iostream>
#include <string>

#include "argparse/argparse.hpp"
#include "fmt/format.h"

#include "bitmap.h"
#include "genesis.h"
#include "ram.h"

namespace fs = std::filesystem;

/**
 * @brief Adds a texture to the virtual file system
 *
 * Opens a file in the virtual file system, creates a bitmap from it, and writes
 * the bitmap to the virtual file system in a file with the same name as the
 * original file, without extension.
 *
 * @param texturePath The path to the file to be added
 * @param vfs The virtual file system to add the texture to
 * @return true if the texture was successfully added, false otherwise
 */
bool AddTexture(const char* texturePath, geVFile* vfs) {
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

    // Extract the filename (without extension)
    std::string textureName = fs::path(texturePath).stem().string();

    // Create the file that's gonna hold the bitmap in the virtual file system
    geVFile* tempFile =
        geVFile_Open(vfs, textureName.c_str(), GE_VFILE_OPEN_CREATE);
    if (!tempFile) {
        fmt::println("Could not save bitmap {}", textureName);
        geVFile_Close(vfs);
        geBitmap_Destroy(&bitmap);
        return false;
    }

    // Write the bitmap to the virtual file system
    geBoolean WriteResult = geBitmap_WriteToFile(bitmap, tempFile);
    geVFile_Close(tempFile);
    if (WriteResult == GE_FALSE) {
        fmt::println("Could not save bitmap {}", textureName);
        geVFile_Close(vfs);
        geBitmap_Destroy(&bitmap);
        return false;
    }

    geBitmap_Destroy(&bitmap);

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
    argparse::ArgumentParser program{"ImageToTxl", "0.1.0"};

    program.add_argument("-i", "--input")
        .required()
        .help("specify the input folder.");

    program.add_argument("-o", "--output")
        .required()
        .help("specify the output file.");

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& ex) {
        fmt::println("{}", ex.what());
        std::cout << program;
        return 1;
    }

    std::string inputPath = program.get<std::string>("--input");
    std::string txlPath = program.get<std::string>("--output");

    /* Open the .txl file */
    geVFile* vfs = geVFile_OpenNewSystem(
        nullptr, GE_VFILE_TYPE_VIRTUAL, txlPath.c_str(), nullptr,
        GE_VFILE_OPEN_CREATE | GE_VFILE_OPEN_DIRECTORY);

    if (!vfs) {
        fmt::println("Could not open file {}", txlPath);
        return 1;
    }

    // Iterate through the directory and add all the textures to the .txl file
    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (fs::is_regular_file(entry.status())) {
            AddTexture(entry.path().string().c_str(), vfs);
        }
    }

    geVFile_Close(vfs);

    // Print all the textures to see if everything went okay
    ListTextures(txlPath);

    return 0;
}