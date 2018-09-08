#pragma once

#define FOR(n) for(int i = 0; i < n; i++)

namespace constants {
	//フィールドの大きさ
	//適当
	const int row_len = 20;
	const int col_len = 7;
}

using BlockGrid = std::vector<std::vector<Block>>;