
#include <Siv3D.hpp>
#include "Title.h"
#include "Game.h"

void Main()
{
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
