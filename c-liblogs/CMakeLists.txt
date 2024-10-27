file(GLOB LOGS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(c-liblogs ${LOGS_SRC})

set_property(TARGET c-liblogs PROPERTY C_STANDARD 17)

target_include_directories(c-liblogs PUBLIC include)