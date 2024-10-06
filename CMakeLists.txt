cmake_minimum_required(VERSION 3.20)

project(
    tpack-cli
    VERSION 0.1.0
    LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 0c9fce2ffefecfdce794e1859584e25877b7b592 # 11.0.2
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG 68fd0277eea5412aff9b91c0b70efc698359dae0 # v3.1
)
FetchContent_MakeAvailable(argparse)

add_subdirectory(lib)
add_subdirectory(app)
