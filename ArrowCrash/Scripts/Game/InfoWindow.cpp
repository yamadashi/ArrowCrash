#include "InfoWindow.h"


InfoWindow::InfoWindow(int playerNum, GameData& data, const int& score_)
	:playerNumber(playerNum),
	score(score_),
	size(1.2 * data.fieldSize.x, 0.2 * data.fieldSize.x),
	//分かりにくくてごめんやで
	centerPos(data.stdPositions[playerNumber].movedBy(0.5 * data.fieldSize.x, data.fieldSize.y + (Window::Height() - (data.stdPositions[playerNumber].y + data.fieldSize.y)) * 0.5))
{}

void InfoWindow::draw() const {
	TextureAsset(L"infoFrame").resize(size).drawAt(centerPos);
	(*font)(playerNumber, L"P socre:", score).drawAt(centerPos);
}

void InfoWindow::setFont(String handler, int size) {
	font.reset(new Font(size, handler));
}

std::unique_ptr<Font> InfoWindow::font(nullptr);