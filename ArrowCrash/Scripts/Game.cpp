#include "Game.h"


void Game::init() {
	m_data->numOfPlayer = 1; //仮
	setGameData();
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		players.emplace_back(i, gameData);
	}
}

void Game::update() {
	for (auto& player : players) {
		player.update();
	}
}

void Game::draw() const {
	for (auto& player : players) {
		player.draw();
	}
}



//マージンとかUI(次に落ちてくるやつの表示窓とか何Pなのかとか)はまだ考えてない
void Game::setGameData() {

	auto&& window = Window::Size();

	//泥臭いのでなんかスマートな方法あればリファクタリングしたい
	switch (m_data->numOfPlayer)
	{
	case 1: {
			gameData.cellSize = window.y / constants::row_len; // calculate cell size
			int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width

			gameData.stdPositions.emplace_back((window.x - fieldWidth) / 2, 0);
			break;
	}

	case 2: {
		gameData.cellSize = window.y / constants::row_len; // calculate cell size
		int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width
		int regionWidth = window.x / 2; //各プレイヤー領域の幅

		for (int i = 0; i < 2; i++) {
			gameData.stdPositions.emplace_back(regionWidth*i + (regionWidth - fieldWidth) / 2, 0);
		}
		break;
	}

	case 3:
		//要工夫
		//gameData.cellSize = (window.y / 2) / constants::row_len; // calculate cell size
		//int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width
		//float regionWidth = ((float)window.x / 2); //1P,2Pの領域の幅
		//
		//gameData.stdPositions.emplace_back(regionWidth*i + (regionWidth - fieldWidth) / 2, 0);
		break;

	case 4: {
		gameData.cellSize = (window.y / 2) / constants::row_len; // calculate cell size
		int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width
		auto&& region = window / 2; //各プレイヤー領域

		for (int i = 0; i < 4; i++) {
			int stdPos_x = (i % 2)*region.x + (region.x - fieldWidth) / 2;
			int stdPos_y = (i / 2)*region.y;
			gameData.stdPositions.emplace_back(stdPos_x, stdPos_y);
		}
		break;
	}

	default:
		break;
	}
}