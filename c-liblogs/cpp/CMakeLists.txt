file(GLOB LOGS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.cpp")

add_library(cpp-liblogs ${LOGS_SRC})

set_property(TARGET cpp-liblogs PROPERTY CPP_STANDARD 17)

target_include_directories(cpp-liblogs PUBLIC include)