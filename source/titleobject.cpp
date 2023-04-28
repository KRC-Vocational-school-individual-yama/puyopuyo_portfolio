#include "titleobject.h"

#include"libs/scenemanager.h"
#include"libs/keyBoard.h"
#include"libs/keyManager.h"

TitleObject::TitleObject(Scene* sc)
	:Object				(sc)
	,keys				(nullptr)
	,hImage(-1)
	,keystop			{true}
	,hFont(-1)
{}

TitleObject::~TitleObject(){
	DeleteFontToHandle(hFont);
	DeleteGraph(hImage);
	delete keys;
}

void TitleObject::Init(){
	hImage=LoadGraph("data/texture/backimages2/25th_pc03.png");
	assert(hImage > 0);
	hFont = CreateFontToHandle("",64,-1);
	assert(hFont > 0);
	
	keys = new KeyManager();
	std::vector<int>keyNumberList;
	keyNumberList.reserve(10);
	keyNumberList.emplace_back(KEY_INPUT_SPACE);
	keyNumberList.emplace_back(KEY_INPUT_RETURN);
	keyNumberList.emplace_back(KEY_INPUT_TAB);
	keyNumberList.emplace_back(KEY_INPUT_ESCAPE);
	keyNumberList.shrink_to_fit();
	keys->SetKey(keyNumberList);

}

void TitleObject::Update(){

	ChangeScene();
}

void TitleObject::Draw(){
	DrawExtendGraph(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,hImage,FALSE);

	DrawFormatStringToHandle(200,200,0x7fbfff, hFont, u8"Puyo puyo");

	DrawFormatString(300,400,0x1,u8"スペースキーを押してGameScene");
}



bool TitleObject::EndKeyInput(){
	return keys->Down(KEY_INPUT_ESCAPE);
}
	//bool endKey = keys->Down(KEY_INPUT_ESCAPE);
	//if (endKey&&!endKeyStop) 
	//	return true;
	//
	//endKeyStop = endKey;
	//return false;

void TitleObject::ChangeScene() {
	bool mouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	bool sceneKey =  keys->Down(KEY_INPUT_SPACE)
				  || keys->Down(KEY_INPUT_RETURN)
				  || (mouseLeft&&!keystop);
	//nextScene キー
	if (sceneKey) 
		SceneManager::SceneChange("Game");
	
	//back キー
	if (keys->Down(KEY_INPUT_TAB))
		SceneManager::SceneChange("boot");

	keystop = mouseLeft ;
}

