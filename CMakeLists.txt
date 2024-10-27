file(GLOB LOGS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/c-liblogs/src/*.c")

add_library(c-liblogs ${LOGS_SRC})

set_property(TARGET c-liblogs PROPERTY C_STANDARD 17)

target_include_directories(c-liblogs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/c-liblogs/include)