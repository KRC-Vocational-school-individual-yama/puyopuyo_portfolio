#pragma once
#include"libs/object.h"
class KeyManager;

class BoardSave :public Object {
public:
	BoardSave(Scene* sc);
	~BoardSave()	override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;
private:
	KeyManager* keys;
	std::vector<std::string> dispString;

	void Save();
	//�֐������s�ł����� true;
	[[nodiscard]]bool Reload();
	void AllErase();
	void LoadKeyFunc();
	void DeleteKeyFunc();
};