cmake_minimum_required(VERSION 3.8)

add_executable(${PROJECT_NAME}
	Main.cpp

	Utils/Time.hpp
	Utils/Utils.hpp
	Utils/Utils.cpp
	Utils/State.hpp
	Utils/State.cpp
	Utils/ProfilePicture.hpp
	Utils/ProfilePicture.cpp

	Graphics/TextureManager.hpp
	Graphics/TextureManager.cpp

    Scene/Scene.hpp
	Scene/Scene.cpp
	Scene/BurgerMenu.hpp
	Scene/BurgerMenu.cpp
    Scene/HomeMenuScene.hpp
    Scene/HomeMenuScene.cpp
	Scene/MatchMenuScene.hpp
	Scene/MatchMenuScene.cpp

	Network/PacketID.hpp
	Network/ENetClient.hpp
	Network/ENetClient.cpp
 "Scene/LeaderboardMenuScene.cpp" "Scene/FriendListMenuScene.cpp" "Utils/FileDialogUtils.cpp")
target_link_libraries(${PROJECT_NAME} raylib_static fmt nlohmann_json enet)
target_include_directories(${PROJECT_NAME} PUBLIC .)
