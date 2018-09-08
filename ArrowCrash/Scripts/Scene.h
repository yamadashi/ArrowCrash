#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>


//シーンを跨いで必要な情報
struct SharedData {
	int numOfPlayer; //プレイヤー数
};


enum class SceneName {
	Title,
	Game
};


using MySceneManager = SceneManager<SceneName, SharedData>;
using Scene = MySceneManager::Scene;