#include "Game.h"


void Game::init() {
	
	initGameData();
	initUIComponents();

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

	ui.draw();
}


void Game::initGameData() {

	auto&& window = Window::Size();
	gameData.topUIMargin = window.y / 10;
	gameData.topMargin = window.y / 30;
	gameData.bottomMargin = window.y / 50;

	//泥臭いのでなんかスマートな方法あればリファクタリングしたい
	switch (m_data->numOfPlayer)
	{
	case 1: {
		auto&& region = Window::Size().movedBy(0, -(gameData.topUIMargin + gameData.topMargin + gameData.bottomMargin));
		gameData.cellSize = region.y / constants::row_len; // calculate cell size
		int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width
		
		gameData.stdPositions.emplace_back((region.x - fieldWidth) / 2, gameData.topUIMargin+gameData.topMargin);
		break;
	}

	case 2: {
		auto&& region = Window::Size().movedBy(0, -(gameData.topUIMargin + gameData.topMargin + gameData.bottomMargin));
		gameData.cellSize = region.y / constants::row_len; // calculate cell size
		int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width
		int playerRegionWidth = region.x / 2; //各プレイヤー領域の幅

		for (int i = 0; i < 2; i++) {
			gameData.stdPositions.emplace_back(playerRegionWidth*i + (playerRegionWidth - fieldWidth) / 2, gameData.topUIMargin+gameData.topMargin);
		}
		break;
	}

	case 3:
		//要工夫
		
		break;

	case 4: {
		auto&& region = Window::Size().movedBy(0, -gameData.topUIMargin - (gameData.topMargin + gameData.bottomMargin)*2);
		gameData.cellSize = (region.y / 2) / constants::row_len; // calculate cell size
		int fieldWidth = gameData.cellSize*constants::col_len; // calculate field width
		auto&& playerRegion = region / 2; //各プレイヤー領域

		for (int i = 0; i < 4; i++) {
			int stdPos_x = (i % 2)*playerRegion.x + (playerRegion.x - fieldWidth) / 2;
			int stdPos_y = gameData.topUIMargin + (i / 2)*(playerRegion.y+gameData.bottomMargin) + (i/2+1)*gameData.topMargin;
			gameData.stdPositions.emplace_back(stdPos_x, stdPos_y);
		}
		break;
	}

	default:
		break;
	}
}

void Game::initUIComponents() {

	auto&& window = Window::Size();
	ui.topUIBorder.set({ 0, gameData.topUIMargin }, { window.x, gameData.topUIMargin });
	if (m_data->numOfPlayer % 2 == 0) {
		ui.playerBorder.at(0).set({ window.x / 2, gameData.topUIMargin }, { window.x / 2, window.y });
	}
	if (m_data->numOfPlayer == 4) {
		double tmp_y = gameData.topUIMargin + (window.y - gameData.topUIMargin) / 2;
		ui.playerBorder.at(1).set({ 0, tmp_y }, { window.x, tmp_y });
	}
}


void UIComponents::draw() const {
	topUIBorder.draw();
	for (const auto& line : playerBorder) {
		line.draw();
	}
}