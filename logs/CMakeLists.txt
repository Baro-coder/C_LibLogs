file(GLOB APP_SRC
    "*.h"
    "*.c"
)

add_executable(app ${APP_SRC})

target_link_libraries(app LINK_PUBLIC pthread)

set_property(TARGET app PROPERTY C_STANDARD 11)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})