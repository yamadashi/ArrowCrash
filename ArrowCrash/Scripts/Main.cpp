
#include <Siv3D.hpp>
#include "Title/Title.h"
#include "Explain/Explain.h"
#include "Game/Game.h"
#include "Result/Result.h"
#include "ymdsLib/FileRegisterer/FileRegisterer.h"

void Main()
{
	//最大解像度を取得
	const auto maxSize = Graphics::GetFullScreenSize().back();
	Println(maxSize);
	Window::SetFullscreen(true, maxSize);
	Graphics::SetBackground(Palette::Black);

	ymds::TextureRegisterer().fileRegister(L"Files/").preload();
	const int fontSize = maxSize.y / 36;
	FontAsset::Register(L"kokumincho", fontSize, L"font_1_kokumr_1.00_rls");

	MySceneManager sceneManager;
	sceneManager.add<Title>(SceneName::Title);
	sceneManager.add<Game>(SceneName::Game);
	sceneManager.add<Result>(SceneName::Result);
	sceneManager.add<Explain>(SceneName::Explain);

	while (System::Update())
	{
		sceneManager.updateAndDraw();
	}
}
