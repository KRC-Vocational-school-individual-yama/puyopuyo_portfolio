#include "backImage.h"

namespace {
	std::string pass = "data/texture/backImages/";

	
	//êF
	//https://www.colordic.org/w

	//ÉåÉCÉAÉEÉgéQçl
	//https://store-jp.nintendo.com/dw/image/v2/BFGJ_PRD/on/demandware.static/-/Sites-all-master-catalog/ja_JP/dw7a6c2370/products/D70010000029306/screenShot/a899998afd59d8ab6c10e575a2be3dc807d4b2aca64cd19da0a6e436e2a0abdd.jpg?sw=1368&strip=false

}

BackImages::BackImages(Scene* sc)
	:Object(sc)
	,hImage()
	,count(1)
	,alpha(255)
	,imgNumber{0,1}
{
}

BackImages::~BackImages()
{
	for (auto& img : hImage) {
		DeleteGraph(img);
		img = -1;
	}
	hImage.clear();
}

void BackImages::Init(){
	for (int i = 0; i < MAX_IMG; i++) {
		char buf[40];
		sprintf_s<40>(buf, "backimage_%d.jpg", i+1);
		int img = LoadGraph((pass + buf).c_str());
		assert(img > 0);
		hImage.emplace_back(img);
	}

}

void BackImages::Update(){
	bool nextKey = CheckHitKey(KEY_INPUT_N);
	count++;
	if (alpha <= 255)
		alpha+=5;
	else
		imgNumber.now = imgNumber.next;


	if ((count % (60 * 10) == 0) || (nextKey && !keystop)) {
		imgNumber.next++;
		imgNumber.next = imgNumber.next % MAX_IMG;
		alpha = 0;
		count = 0;
		if (nextKey&&!keystop) {
			alpha = 255;
		}
	}

	keystop = nextKey;
}

void BackImages::Draw(){
	//îwåiï`âÊ
	DrawExtendGraph(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,hImage.at(imgNumber.now),true);

	//éüÇÃîwåiï`âÊ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,alpha);
	DrawExtendGraph(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,hImage.at(imgNumber.next),true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ê‡ñæï∂èë
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255/1.5);
	DrawBox(600-25,SCREEN_HEIGHT-GetFontSize()*2,800,SCREEN_HEIGHT,0xe0e0e0,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawString(600,SCREEN_HEIGHT-GetFontSize()*2, u8"NkeyÇâüÇ∑Ç∆éüÇÃîwåiÇ÷",0x1a1a1a);
}

