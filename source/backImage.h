#pragma once
#include"libs/object.h"

class BackImages :public Object {
public:
	BackImages(Scene*sc);
	~BackImages()	override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;
private:
	std::vector<int>hImage;
	int count;
	float alpha;
	struct {
		int now;
		int next;
	}imgNumber;
	bool keystop;

	static const int MAX_IMG = 4;
};