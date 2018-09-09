
# include <Siv3D.hpp>
#include "Title.h"
#include "Game.h"

void Main()
{
	//Window::Resize(1280, 960);

	MySceneManager sceneManager;
	sceneManager.add<Title>(SceneName::Title);
	sceneManager.add<Game>(SceneName::Game);

	while (System::Update())
	{
		sceneManager.updateAndDraw();
	}
}
