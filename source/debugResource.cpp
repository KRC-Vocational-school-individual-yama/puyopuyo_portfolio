#include "debugResource.h"
#include"libs/scenemanager.h"
#include"libs/keyManager.h"
#include"libs/oneCharacterSpacing.h"
using namespace SceneManager;

namespace {
	//一往復で　２回
	static const int DEBUG_NUM_MAX = 100;
}

DebugResource::DebugResource(Scene* sc)
	:Object(sc)
	,keys(nullptr)
	,count{0}
	,hFontBig(-1)
	,hFontSmall(-1)
	,debugStart(false)
{
}

DebugResource::~DebugResource(){
	DeleteFontToHandle(hFontBig);
	DeleteFontToHandle(hFontSmall);
	delete keys;
}

void DebugResource::Init(){
	keys = new KeyManager();
	hFontBig= CreateFontToHandle("", 128, -1);
	assert(hFontBig > 0);
	hFontSmall = CreateFontToHandle("", 32, -1);
	assert(hFontSmall > 0);
}

void DebugResource::Update(){
	if (keys->Down(KEY_INPUT_P)) {
		debugStart = true;
		count = 0;
	}
	if ( !debugStart)return;

	if (count >= DEBUG_NUM_MAX) {
		debugStart = false;
		count = 0;
		return;
	}
	std::string sceneName = GetCurrentSceneName();
	if (sceneName == "Title") {
		SceneChange("Game");
		count++;
	}
	else if (sceneName == "Game") {
		SceneChange("Title");
		count++;
	}
}

void DebugResource::Draw(){
	std::string sizeStr =   "pを押すとタイトルとゲーム画面の切替デバッグをします";
	std::string dispStr = u8"pを押すとタイトルとゲーム画面の切替デバッグをします";
	float widthOne = GetFontMaxWidthToHandle(hFontSmall);
	float sizeNum = OneCharacterSpacing::StringToWString(sizeStr).size()/2;
	float distance = sizeNum * widthOne;
	DrawFormatStringToHandle(SCREEN_WIDTH - distance, 0, 0x222222, hFontSmall, dispStr.c_str());

	if (!debugStart)return;
	float width= GetFontMaxWidthToHandle(hFontBig);
	float height = GetFontSizeToHandle(hFontBig);
	DrawFormatStringToHandle(SCREEN_WIDTH/2 - width/2, SCREEN_HEIGHT/2 - height/2, 0x222222, hFontBig, "%d", (DEBUG_NUM_MAX /2 - count/2));
}
