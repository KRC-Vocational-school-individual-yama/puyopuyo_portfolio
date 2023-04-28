#include "boardSave.h"
#include"libs/scene.h"
#include"save.h"
#include"board.h"
#include"libs/keyManager.h"

namespace {
	std::string fileName = "boardData";
}

BoardSave::BoardSave(Scene* sc)
	:Object(sc)
	,keys(nullptr)
	,dispString()
{

}

BoardSave::~BoardSave(){
	delete keys;
}

void BoardSave::Init(){
	keys = new KeyManager();
	std::vector<int>sendKey;
	sendKey.reserve(10);
	sendKey.emplace_back(KEY_INPUT_LCONTROL);
	sendKey.emplace_back(KEY_INPUT_S);
	sendKey.emplace_back(KEY_INPUT_R);
	sendKey.emplace_back(KEY_INPUT_D);
	sendKey.shrink_to_fit();
	keys->SetKey(sendKey);

	dispString.emplace_back(u8"====マウス操作====\n");
	dispString.emplace_back(u8"　左クリックで設置\n");
	dispString.emplace_back(u8"　右クリックで消去\n");
	dispString.emplace_back(u8"　中クリックで色取得\n");
	dispString.emplace_back(u8"　ホイールで色選択\n");
	dispString.emplace_back(u8"====盤面保存====\n");
	dispString.emplace_back(u8"　LCtrl+Sで保存\n");
	dispString.emplace_back(u8"　LCtrl+Rで読み込み\n");
	dispString.emplace_back(u8"　LCtrl+Dで全消去\n");
	dispString.emplace_back(u8"====シーケンス操作====\n");
	dispString.emplace_back(u8"　Cで塊消去\n");
	dispString.emplace_back(u8"　Fで落下\n");
	dispString.emplace_back(u8"====ぷよ操作====\n");
	dispString.emplace_back(u8"　WASDで移動\n");
	dispString.emplace_back(u8"　QEで左右回転\n");
	dispString.emplace_back(u8"　Enterで設置\n");


	if (!Reload())
		return;

	//ボードのラベリング更新
	{
		Board* board = GetScene()->Invoke<Board>();
		if (board)
			board->SetLabeling();
	}
}

void BoardSave::Update(){
	bool lctrlKey	= keys->DownKeep(KEY_INPUT_LCONTROL);
	bool saveKey	= lctrlKey && keys->Down(KEY_INPUT_S) ;
	bool loadKey	= lctrlKey && keys->Down(KEY_INPUT_R) ;
	bool deleteKey	= lctrlKey && keys->Down(KEY_INPUT_D) ;

	if (saveKey ) {
		Save();
	}
	if (loadKey ) {
		LoadKeyFunc();
	}
	if (deleteKey ) {
		DeleteKeyFunc();
	}

}

void BoardSave::Draw(){
	Vec2 pos = {600,200-150};
	int distance = 50;
	int width = 200;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 1.5);
	DrawBox(pos.x - distance, pos.y, pos.x+ width, pos.y + (dispString.size() * distance - distance) + GetFontSize(), 0xe0e0e0, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int i = 0;
	for (const auto& str : dispString) {
		DrawString(pos.x, pos.y + i * distance, str.c_str(), 0x1a1a1a);
		++i;
	}
}

void BoardSave::Save(){
	std::vector<std::string>sendData=GetScene()->Invoke<Board>()->GetBoardData_String();
	CsvSave::Save("data/save/"+fileName+".csv",sendData,Board::BOARD_WIDTH);
}

bool BoardSave::Reload(){
	std::vector<std::string>data= CsvSave::Load("data/save/" + fileName + ".csv");
	if (data.empty())return false;
	std::vector<int>colors(data.size(), -1);

	for (int i = 0; i < colors.size(); i++) {
		colors.at(i) = std::stoi(data.at(i));
	}
	GetScene()->Invoke<Board>()->SetBoardData(colors);
	return true;
}

void BoardSave::AllErase(){
	std::vector<int>non(Board::BOARD_WIDTH*Board::BOARD_HEIGHT, -1);
	GetScene()->Invoke<Board>()->SetBoardData(non);
}

void BoardSave::LoadKeyFunc(){
	if(Reload()){
		//ボードのラベリング更新
			Board* board = GetScene()->Invoke<Board>();
			if (board)
				board->SetLabeling();
		}
}

void BoardSave::DeleteKeyFunc(){
		AllErase();
		//ボードのラベリング更新
		{
			Board* board = GetScene()->Invoke<Board>();
			if (board)
				board->SetLabeling();
		}

}
