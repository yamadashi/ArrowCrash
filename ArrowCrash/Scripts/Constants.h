#pragma once
#include <Siv3D.hpp>


namespace constants {

	//フィールドの大きさ
	const int row_len = 20;
	const int col_len = 14;

	//プレイヤーカラー
	const Color playerColor[4] {
		Palette::Red, Palette::Blue, Palette::Yellow, Palette::Green
	};

	//次ブロックの表示数
	const int numOfNextBlocks = 2;
	
	//アイテムタイプの数
	const int numOfItemType = 4;
}
