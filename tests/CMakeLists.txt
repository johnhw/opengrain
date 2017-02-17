# Build file for the various API tests



include_directories(${OPENGRAIN_SOURCE_DIR}/src/api)
link_directories(${OPENGRAIN_BINARY_DIR}/src)
add_executable(test_initshutdown test_initshutdown)
add_executable(test_audio test_audio)

target_link_libraries(test_initshutdown opengrain)
target_link_libraries(test_audio opengrain)

