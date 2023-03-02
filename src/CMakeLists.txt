cmake_minimum_required(VERSION 3.8)

add_executable(${PROJECT_NAME}
	Main.cpp

	Utils/Utils.hpp
	Utils/Utils.cpp

	Graphics/TextureManager.hpp
	Graphics/TextureManager.cpp

    Scene/Scene.hpp
	Scene/Scene.cpp
    Scene/IntroductionScene.cpp
  "Scene/MainMenuScene.cpp")
target_link_libraries(${PROJECT_NAME} raylib_static fmt)
target_include_directories(${PROJECT_NAME} PUBLIC .)
