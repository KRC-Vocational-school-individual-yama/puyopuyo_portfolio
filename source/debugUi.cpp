#include "debugUi.h"
#include"libs/keyManager.h"

DebugUi::DebugUi(Scene* sc)
	:Object(sc)
	,keys(nullptr)
	,mouse(Vec2::zero())
{}

DebugUi::~DebugUi(){
	delete keys;
}

void DebugUi::Init(){
	keys = new KeyManager();
	GetMousePoint(&mouse.x, &mouse.y);

}

void DebugUi::Update(){
	GetMousePoint(&mouse.x, &mouse.y);

}

void DebugUi::Draw(){
	printfDx("mousex= %d\n",mouse.x);
	printfDx("mousey= %d\n",mouse.y);
}
