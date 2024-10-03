add_executable(imagetotxl "src/main.cpp")

target_compile_features(imagetotxl PUBLIC cxx_std_17)

target_link_libraries(imagetotxl PUBLIC
    ${PROJECT_SOURCE_DIR}/lib/GenesisSDK/lib/genesis.lib
    # fmt::fmt
    # argparse::argparse
    # common
)

target_include_directories(imagetotxl PUBLIC
    "${PROJECT_SOURCE_DIR}/lib/GenesisSDK/include"
)