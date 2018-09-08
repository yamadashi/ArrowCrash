
# include <Siv3D.hpp>
#include "Title.h"
#include "Game.h"

void Main()
{
	MySceneManager sceneManager;
	sceneManager.add<Title>(SceneName::Title);
	sceneManager.add<Game>(SceneName::Game);

	while (System::Update())
	{
		sceneManager.updateAndDraw();
	}
}
