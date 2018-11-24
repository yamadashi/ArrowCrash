
#include <Siv3D.hpp>
#include "Title/Title.h"
#include "Explain/Explain.h"
#include "Game/Game.h"
#include "ymdsLib/FileRegisterer/FileRegisterer.h"

void Main()
{
	//最大解像度を取得
	const auto maxSize = Graphics::GetFullScreenSize().back();
	Window::SetFullscreen(true, maxSize);

	ymds::AssetRegisterer().fileRegister(L"Files/").preload();
	SoundAsset(L"bgm").setLoop(true);
	SoundAsset(L"title_bgm").setLoop(true);

	FontManager::Register(L"Files/font/font_1_kokumr_1.00_rls.ttf");
	FontManager::Register(L"Files/font/dash_digital-7.ttf");
	FontAsset::Register(L"kokumincho", maxSize.y / 36, L"刻明朝 Regular");
	FontAsset::Register(L"digital", maxSize.y / 40, L"Dash Digital-7");

	MySceneManager sceneManager;
	sceneManager.add<Title>(SceneName::Title);
	sceneManager.add<Game>(SceneName::Game);
	sceneManager.add<Explain>(SceneName::Explain);

	while (System::Update())
	{
		sceneManager.updateAndDraw();
	}
}
