project(nlohmann_json LANGUAGES C CXX)
add_library(nlohmann_json INTERFACE json.hpp json_fwd.hpp)
target_include_directories(nlohmann_json INTERFACE .)
