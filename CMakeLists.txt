cmake_minimum_required(VERSION 3.23)

project(Dagger CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Check if the operating system is Linux
if (UNIX AND NOT APPLE)
    # Add TBB_INCLUDE_DIR and TBB_LIBRARY_DIR variables pointing to the TBB headers and libraries
    find_package(TBB REQUIRED)
endif()



find_package(EnTT)
find_package(glad 0.1.34)
find_package(glfw3)
find_package(glm)
find_package(imgui)
find_package(nlohmann_json)
find_package(spdlog)
find_package(fmt)
find_package(stb)
find_package(tsl-sparse-map)
find_package(simpleini)


# Add all .cpp files in the current directory and its subdirectories to the project
file(GLOB_RECURSE SOURCES source/dagger/*.cpp)

# Add all .h files in the current directory and its subdirectories to the project
file(GLOB_RECURSE HEADERS source/dagger/*.h)

# Get a list of files and subdirectories in the 'data' directory
file(GLOB_RECURSE DATA_CONTENTS ${CMAKE_SOURCE_DIR}/data/*)

# Copy each file and subdirectory individually to the build directory
foreach(DATA_CONTENT ${DATA_CONTENTS})
    # Get the relative path of the file or subdirectory
    file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR}/data ${DATA_CONTENT})
    # If it's a directory, create the corresponding directory in the build directory
    if(IS_DIRECTORY ${DATA_CONTENT})
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${REL_PATH})
    else()
        # Copy the file to the build directory
        configure_file(${DATA_CONTENT} ${CMAKE_BINARY_DIR}/${REL_PATH} COPYONLY)
    endif()
endforeach()


# Create an executable target using the collected sources and headers
add_executable(Dagger ${SOURCES} ${HEADERS} bindings/imgui_impl_glfw.cpp bindings/imgui_impl_opengl3.cpp)


target_compile_definitions(Dagger PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(Dagger PRIVATE source/dagger bindings)
target_link_libraries(Dagger EnTT::EnTT glad::glad glfw glm::glm imgui::imgui nlohmann_json::nlohmann_json spdlog::spdlog fmt::fmt stb::stb tsl::sparse_map simpleini::simpleini)

if (UNIX AND NOT APPLE)
    if (TBB_FOUND)
        # TBB found, link it
        target_link_libraries(Dagger TBB::tbb)
    else()
        message(FATAL_ERROR "TBB not found, please install it or adjust the search paths.")
    endif()
endif()

if(WIN32)
    target_link_libraries(Dagger Winmm)
endif()
