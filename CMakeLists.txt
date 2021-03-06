# Required by json-schema-validator.
# Required by FindBoost aparently.
cmake_minimum_required(VERSION 3.2)

# Set project identifier.
project(drachma)

find_package(Boost COMPONENTS regex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # not necessary, but encouraged

include(FetchContent)

# Fetch the cppJsonGraph library.
FetchContent_Declare(
  cpp_graph_json
  GIT_REPOSITORY https://github.com/nallj/cppJsonGraph.git
  GIT_TAG origin/master)
FetchContent_MakeAvailable(cpp_graph_json)

# Fetch the yaml-cpp.
FetchContent_Declare(
  yaml_cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  # GIT_TAG release-0.3.0) # Old API
  GIT_TAG yaml-cpp-0.6.3)
FetchContent_MakeAvailable(yaml_cpp)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${cpp_graph_json_SOURCE_DIR})
include_directories("${yaml_cpp_SOURCE_DIR}/include")

# TODO: Figure out how to get rid of these. drachma shouldn't need to be aware of cppGraphJson's dependencies.
include_directories("${nlohmann_json_SOURCE_DIR}/include")
include_directories("${nlohmann_json_schema_validator_SOURCE_DIR}/src")

add_subdirectory(src)
