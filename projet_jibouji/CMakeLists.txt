cmake_minimum_required(VERSION 3.10)

# Automatically set the project name to the directory name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Set the project name and version
project(${PROJECT_NAME} VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find wiringPi
find_library(WIRINGPI_LIB wiringPi)

# Check if wiringPi was found
if(WIRINGPI_LIB)
    message("-----lib----- wiring Pi Found")
endif()
if(NOT WIRINGPI_LIB)
    message(FATAL_ERROR "wiringPi library not found. Please install wiringPi.")
endif()

# Find lsndfile
find_library(LSNDFILE_LIB sndfile)

# Check if sndfile was found
if(LSNDFILE_LIB)
    message("-----lib----- LSNDFILE Found")
endif()
if(NOT LSNDFILE_LIB)
    message(FATAL_ERROR "sndFile library not found. Please install sndFile.")
endif()


# Find lsndfile
find_library(ALSA_LIB asound)
# Check if asla was found
if(ALSA_LIB)
    message("-----lib----- ASLA Found")
endif()
if(NOT ALSA_LIB)
    message(FATAL_ERROR "asla library not found. Please install asla.")
endif()

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/docs)
include_directories(${CMAKE_SOURCE_DIR}/exemples)
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Include directories for header files
include_directories("${CMAKE_SOURCE_DIR}/lib")


# Find all .cpp files in the src and lib directories (including subdirectories)
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/*/*.cpp"
)

# Find all header files in the lib directory (including subdirectories)
file(GLOB_RECURSE HEADERS
    "${CMAKE_SOURCE_DIR}/lib/*/*.h"
)

# Add executable target, passing in all the .cpp files found
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link wiringPi to the executable
target_link_libraries(${PROJECT_NAME} ${WIRINGPI_LIB} ${LSNDFILE_LIB} ${ALSA_LIB})
