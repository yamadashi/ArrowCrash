#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>


//シーンを跨いで必要な情報
struct SharedData {
	//ゲームの結果とかをリザルトで使いたいときとか
	int player_num;
	int cellSize = 20; //とりあえず
};


enum class SceneName {
	Title,
	Game
};


using MySceneManager = SceneManager<SceneName, SharedData>;
using Scene = MySceneManager::Scene;