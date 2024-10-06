add_executable(tpack-cli-exe "src/main.cpp")

set_property(TARGET tpack-cli-exe PROPERTY OUTPUT_NAME tpack-cli)

target_compile_features(tpack-cli-exe PUBLIC cxx_std_17)

target_link_libraries(tpack-cli-exe PUBLIC
    GenesisSDK
    fmt::fmt
    argparse::argparse
)

set_target_properties(tpack-cli-exe PROPERTIES
    LINK_FLAGS "/NODEFAULTLIB:LIBCMT /SAFESEH:NO"
)
