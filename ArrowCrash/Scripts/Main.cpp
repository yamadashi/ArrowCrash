﻿
#include <Siv3D.hpp>
#include "Title/Title.h"
#include "Game/Game.h"
#include "ymdsLib/FileRegisterer.h"

void Main()
{
	ymds::TextureRegisterer().fileRegister(L"Files/").preload();
	FontAsset::Register(L"kokumincho30", 30, L"font_1_kokumr_1.00_rls");

	//最大解像度を取得
	const auto maxSize = Graphics::GetFullScreenSize().back();
	
	Window::SetFullscreen(true, maxSize);

	MySceneManager sceneManager;
	sceneManager.add<Title>(SceneName::Title);
	sceneManager.add<Game>(SceneName::Game);

	while (System::Update())
	{
		sceneManager.updateAndDraw();
	}
}
