#include "scenechanger.h"

#include<Windows.h>
#include<assert.h>

#include"bootScene.h"
#include"scene.h"
#include"../titlescene.h"
#include"../gamescene.h"


Scene* SceneChanger::Changer(std::string scene) {
	if		(scene == "boot")		return new BootScene()		;
	else if	(scene == "Title")		return new TitleScene()		;
	else if (scene == "Game")		return new GameScene()		;
	else {
		MessageBox(NULL, ("次のシーンはありません\n" + scene+"\n"+"無視を押すと続行できます").c_str(), "SceneFactory", MB_ICONERROR | MB_OK);
		assert(false);
		return nullptr;
	}

}
