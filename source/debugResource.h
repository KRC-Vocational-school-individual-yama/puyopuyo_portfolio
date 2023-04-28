#pragma once
#include"libs/object.h"
class KeyManager;

class DebugResource :public Object {
public:
	DebugResource(Scene* sc);
	~DebugResource()override;
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	KeyManager* keys;
	int count;
	int hFontBig;
	int hFontSmall;
	bool debugStart;
};