cmake_minimum_required(VERSION 3.8)

add_executable(${PROJECT_NAME}
	Main.cpp
	Graphics/TextureManager.cpp
)
target_link_libraries(${PROJECT_NAME} raylib_static fmt)
target_include_directories(${PROJECT_NAME} PUBLIC .)
