cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(game)

# Our Project
add_executable(${PROJECT_NAME} src/main.cpp)


# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
