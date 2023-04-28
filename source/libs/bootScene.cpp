#include "bootScene.h"

#include"scenemanager.h"
#include"bootTestObject.h"

#include"../debugResource.h"


BootScene::BootScene() {
	Create<BootTestObject>();

#define  RESOURCE_DEBUG
#ifdef  RESOURCE_DEBUG
	SceneManager::GetCommonScene()->Create<DebugResource>();

#endif //  RESOURCE_DEBUG
}
BootScene::~BootScene(){
}

