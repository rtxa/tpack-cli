add_executable(imagetotxl "src/main.cpp")

target_compile_features(imagetotxl PUBLIC cxx_std_17)

target_link_libraries(imagetotxl PUBLIC
    GenesisSDK
    # fmt::fmt
    # argparse::argparse
    # common
)

