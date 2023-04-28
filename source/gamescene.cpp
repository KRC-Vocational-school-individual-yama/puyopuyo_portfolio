#include "gamescene.h"

#include<DxLib.h>
#include"libs/scenemanager.h"
#include"board.h"
#include"mouseEdit.h"
#include"boardSave.h"
#include"backImage.h"
#include"debugUi.h"
#include"player.h"
#include"sequence.h"


GameScene::GameScene()
	:keystop{true}
{
	SetBackgroundColor(0xe5,0xff,0xff);
	Sequence* seq=Create<Sequence>("Sequence");
	ChangeDispRank(seq, 101);

	Create<Board>("Board");
	Create<MouseEdit>("MouseEdit");
	Create<BoardSave>("BoardSave");
	BackImages* img= Create<BackImages>("BackImages");
	ChangeDispRank(img, -1);
	Create<DebugUi>("DebugUi");
	Create<Player>("Player");
}

GameScene::~GameScene(){
}

void GameScene::Update(){
	Scene::Update();

	bool backKey = CheckHitKey(KEY_INPUT_ESCAPE);
	if (backKey && !keystop) {
		SceneManager::SceneChange("Title");
	}keystop = backKey;
}

void GameScene::Draw() {
	Scene::Draw();

	DrawFormatString(100,100,0x0,"GameScene\nescape‚Å–ß‚é");
}