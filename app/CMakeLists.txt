add_executable(imagetotxl_exe "src/main.cpp")

set_property(TARGET imagetotxl_exe PROPERTY OUTPUT_NAME imagetotxl)

target_compile_features(imagetotxl_exe PUBLIC cxx_std_17)

target_link_libraries(imagetotxl_exe PUBLIC
    GenesisSDK
    fmt::fmt
    argparse::argparse
)

set_target_properties(imagetotxl_exe PROPERTIES
    LINK_FLAGS "/NODEFAULTLIB:LIBCMT /SAFESEH:NO"
)
