
add_executable(generator_cli generator.cpp)
INSTALL(TARGETS generator_cli DESTINATION ${CMAKE_INSTALL_PREFIX})

