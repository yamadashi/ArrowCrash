#include "Game.h"


//マージンとかUI(次に落ちてくるやつの表示窓とか何Pなのかとか)はまだ考えてない
void Game::setData() {

	// フィールドの横/縦の比
	float aspactRatio = constants::col_len / constants::row_len;
	auto&& window = Window::Size();

	//泥臭いのでなんかスマートな方法あればリファクタリングしたい
	switch (m_data->numOfPlayer)
	{
	case 1:
		data.cellSize = window.y / constants::row_len; // calculate cell size
		int fieldWidth = data.cellSize*constants::col_len; // calculate field width
		
		data.stdPositions.emplace_back((window.x - fieldWidth) / 2, 0);
		break;
	case 2:
		data.cellSize = window.y / constants::row_len; // calculate cell size
		int fieldWidth = data.cellSize*constants::col_len; // calculate field width
		int regionWidth = window.x / 2; //各プレイヤー領域の幅

		for (int i = 0; i < 2; i++) {
			data.stdPositions.emplace_back(regionWidth*i + (regionWidth - fieldWidth) / 2, 0);
		}
		break;
	case 3:
		//要工夫
		//data.cellSize = (window.y / 2) / constants::row_len; // calculate cell size
		//int fieldWidth = data.cellSize*constants::col_len; // calculate field width
		//float regionWidth = ((float)window.x / 2); //1P,2Pの領域の幅
		//
		//data.stdPositions.emplace_back(regionWidth*i + (regionWidth - fieldWidth) / 2, 0);
		break;
	case 4:
		data.cellSize = (window.y / 2) / constants::row_len; // calculate cell size
		int fieldWidth = data.cellSize*constants::col_len; // calculate field width
		auto&& region = window / 2; //各プレイヤー領域

		for (int i = 0; i < 4; i++) {
			int stdPos_x = (i % 2)*region.x + (region.x - fieldWidth) / 2;
			int stdPos_y = (i / 2)*region.y;
			data.stdPositions.emplace_back(stdPos_x, stdPos_y);
		}
		break;
	default:
		break;
	}
}