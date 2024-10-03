add_library(GenesisSDK INTERFACE)

target_link_libraries(GenesisSDK INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/genesis.lib
)

target_include_directories(GenesisSDK INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)