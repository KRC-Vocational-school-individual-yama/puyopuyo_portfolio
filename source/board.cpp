#include "board.h"
#include<array>
#include"libs/scene.h"
#include"mouseEdit.h"
#include"libs/keyManager.h"

namespace {
	std::string graphPass = "data/texture/puyo/";
	KeyManager* keys=nullptr;
}

Board::Board(Scene* sc)
	:Object(sc)
	,puyos(BOARD_WIDTH* BOARD_HEIGHT, Puyo{})
	,hImage(static_cast<int>(PuyoColor::max),-1)
	,fontHandle(-1)
	,labelList(BOARD_WIDTH * BOARD_HEIGHT,-1)
	,labelCountList()
{
	keys = new KeyManager();
}

Board::~Board(){
	for (int i = 0; i < hImage.size(); i++)
		DeleteGraph(hImage.at(i));

	DeleteFontToHandle(fontHandle);
	delete keys;
}

void Board::Init(){
	fontHandle = CreateFontToHandle("", 64, -1);
	assert(fontHandle > 0);

	for (int i = 0; i < hImage.size(); i++) {
		char buf[40];
		sprintf_s<40>(buf, (graphPass+"puyo_%d.png").c_str(), i+1);
		hImage.at(i) = LoadGraph(buf);
		assert(hImage.at(i) > 0);
	}


#ifdef FAST_RAND_BOARD
	for (int i = 0; i < puyos.size(); i++) {
		puyos.at(i).color=GetRand(static_cast<int>(PuyoColor::max) - 1);
	}
#endif //FAST_RAND_BOARD
}

void Board::Update() {
	//ぷよを編集の為
	{
		MouseEdit* edit = GetScene()->Invoke<MouseEdit>();
		int color = 0;
		int number = 0;
		if (edit->GetPuyoNumber(color, number)) {
			puyos.at(number).color = color;
			//debug用
			SetLabeling();
		}
	}

	//ぷよ消す
	if (keys->Down(KEY_INPUT_C)) {
		PuyoErase();
	}
	//ぷよ落とす
	if (keys->Down(KEY_INPUT_F)) {
		PuyoFoll();
	}
	
}

void Board::Draw() {
	SetDrawArea(BOARD_FAST_X, BOARD_FAST_Y + PUYO_SIZE / 3, BOARD_FAST_X + BOARD_WIDTH * PUYO_SIZE, BOARD_FAST_Y + BOARD_HEIGHT * PUYO_SIZE);
	//ボード内背景色
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255/1.1);
	DrawBox(BOARD_FAST_X,BOARD_FAST_Y, BOARD_FAST_X+ BOARD_WIDTH*PUYO_SIZE, BOARD_FAST_Y+BOARD_HEIGHT*PUYO_SIZE, 0x6d3625, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	////　１枠描画 ＆ ぷよ描画
	//for (int x = 0; x < BOARD_WIDTH; x++) {
	//	for (int y = 0; y < BOARD_HEIGHT; y++) {
	//		Pos pos = { BOARD_FAST_X + x * PUYO_SIZE,BOARD_FAST_Y + y * PUYO_SIZE };
	//		//枠線描画
	//		DrawBox(pos.x,pos.y,pos.x+PUYO_SIZE,pos.y+PUYO_SIZE,0x1a1a1a,FALSE);
	//
	//		//数字
	//		DrawFormatStringToHandle(pos.x,pos.y,0,fontHandle,"%2d",labelList.at(GetIndex(x,y)));
	//
	//		//ぷよ描画
	//		int colorNumber = puyos.at(GetIndex(x,y)).color;
	//		if (colorNumber < 0)
	//			continue;
	//		
	//		DrawExtendGraph(pos.x, pos.y, pos.x + PUYO_SIZE, pos.y + PUYO_SIZE, hImage.at(colorNumber), true);
	//
	//		//数字
	//		DrawFormatStringToHandle(pos.x, pos.y, 0, fontHandle, "%2d", labelList.at(GetIndex(x, y)));
	//
	//	}
	//}

	//　１枠描画 ＆ ぷよ描画
	for (int i = 0; i < (BOARD_WIDTH * BOARD_HEIGHT); i++) {
		int x = i%BOARD_WIDTH;
		int y = i/BOARD_WIDTH;

		Pos pos = { BOARD_FAST_X + x * PUYO_SIZE,BOARD_FAST_Y + y * PUYO_SIZE };
		//枠線描画
		DrawBox(pos.x, pos.y, pos.x + PUYO_SIZE, pos.y + PUYO_SIZE, 0x1a1a1a, FALSE);

		//数字
		DrawFormatStringToHandle(pos.x, pos.y, 0, fontHandle, "%2d", labelList.at(GetIndex(x, y)));

		//ぷよ描画
		int colorNumber = puyos.at(GetIndex(x, y)).color;
		if (colorNumber < 0)
			continue;

		DrawExtendGraph(pos.x, pos.y, pos.x + PUYO_SIZE, pos.y + PUYO_SIZE, hImage.at(colorNumber), true);

		//数字
		DrawFormatStringToHandle(pos.x, pos.y, 0, fontHandle, "%2d", labelList.at(GetIndex(x, y)));


	}
	
	SetDrawAreaFull();
	//ボード外枠
	DrawBoxAA(BOARD_FAST_X,BOARD_FAST_Y+PUYO_SIZE/3, BOARD_FAST_X+ BOARD_WIDTH*PUYO_SIZE, BOARD_FAST_Y+BOARD_HEIGHT*PUYO_SIZE, 0xf8f4e6, FALSE,5);
	
	
	for (int i = 0; i < labelCountList.size(); i++) {
		char buf[40];
		sprintf_s<40>(buf,u8"%2d塊 :",i);
		DrawFormatString(10/*900*/, 500 + (i*16), 0xffffff, (buf + std::to_string(labelCountList.at(i))+u8"個").c_str());
	}

}

std::vector<std::string> Board::GetBoardData_String(){
	std::vector<std::string> data(puyos.size(),"");
	for (int x = 0; x < puyos.size(); x++) {
		data.at(x) = std::to_string(puyos.at(x).color);
	}

	return data;
}

const std::vector<int> Board::GetBoardData(){
	std::vector<int>sendData(puyos.size(), -1);
	for (int i = 0; i < puyos.size(); i++) {
		sendData.at(i) = puyos.at(i).color;
	}

	return sendData;
}

void Board::SetBoardData(std::vector<int> color){
	for (int x = 0; x < puyos.size(); x++) {
		puyos.at(x).color = color.at(x);
	}
}

void Board::SetLabeling() { 
	Labeling(); 

	//使わない
	//Recursion();
}

void Board::PuyoFoll() {
	

	//このループが終わったら全部ぷよが落ちている。
	
	while (true) {
		//全て落ちたなら抜け出す
		if (PuyoFoll_Method())
			break;
	}

	//ラベリングを貼り直す
	SetLabeling();
}

bool Board::PuyoErase(){
	bool erased = false;
	for (int i = 0; i < puyos.size(); i++) {
		if (puyos.at(i).color == -1)//ぷよが空なら次のループへ
			continue;
		int countIndex = labelList.at(i);

		if (4 > labelCountList.at(countIndex))//ぷよが４つ以上
			continue;


		puyos.at(i).color = -1;
		erased = true;
	}
	//ラベリングを貼り直す
	SetLabeling();
	return erased;
}

void Board::Reset(){
	std::vector<int>non(BOARD_WIDTH * BOARD_HEIGHT, -1);
	SetBoardData(non);
}

bool Board::IsEmptyGrid(int x, int y){
	int index= GetIndex(x, y);
	//範囲外対策
	{
		if (index < 0)
			return false;
		if (index >= puyos.size())
			return false;

		if (x <  0			 )
			return false;
		if (x >= BOARD_WIDTH )
			return false;

		if (y <  0			 )
			return false;
		if (y >= BOARD_HEIGHT)
			return false;
	}

	//-1はぷよがない
	if (puyos.at(index).color != static_cast<int>(PuyoColor::non))
		return false;


	return true;
}

bool Board::GameOver(){
	int index = GetIndex(2, 1);
	bool flag = (puyos.at(index).color) != (-1);
	return flag;
}

//全て落ちたなら抜け出す
bool Board::PuyoFoll_Method(){
	bool loopEnd = true;

	//ぷよを1つ下に落とす
	for (int i = puyos.size() - 1; i >= 0; i--) {
		int x = i % BOARD_WIDTH;
		int y = i / BOARD_WIDTH;

		int nextIndex = GetIndex(x, y + 1);
		if (nextIndex >= puyos.size())//範囲外対策 一番下の列かどうか
			continue;

		if (puyos.at(nextIndex).color != -1)//下のぷよが空なら次のループへ
			continue;

		//1つ下へ　入れ替え
		puyos.at(nextIndex).color = puyos.at(i).color;
		puyos.at(i).color = -1;

		//全て落ちたか確認
		loopEnd = loopEnd && ((puyos.at(nextIndex).color) == -1);
	}
	return loopEnd;
}

void Board::Labeling(){
	std::vector<int>deleteList(puyos.size(), 0);
	int massNumber = 0;//ラベルの塊 個数

	for (int i = 0; i < puyos.size(); i++) {
		int target = GetBoardPuyo(i); 

		//空のマスなら次のループへ
		if (target == -1) {
			deleteList.at(i) = -1;
			continue;
		}

		int x = i % BOARD_WIDTH;
		int y = i / BOARD_WIDTH;

		int targetUp = (y <= 0) ? -1 : GetBoardPuyo(x, y - 1);
		int targetLeft = (x <= 0) ? -1 : GetBoardPuyo(x-1,y);

		if (target == targetUp) {//上が同じ色なら
			UpMatch(deleteList,x,y,i,target,targetLeft);
			continue;
		}
		
		if (target == targetLeft) {
			deleteList.at(i) = deleteList.at(GetIndex(x - 1, y));
			continue;
		}
		
		
		deleteList.at(i) = massNumber;
		massNumber++;
		


	}

	Labeling_Method(deleteList,massNumber);
}

void Board::UpMatch(std::vector<int>& deleteList, int& x, int& y, int& i, int& target, int& targetLeft) {
	int label = deleteList.at(GetIndex(x, y - 1));
	deleteList.at(i) = label;


	//左の数字を上の数字にする
	if (target != targetLeft)
		return;
	int oldLabel = deleteList.at(GetIndex(x - 1, y));

	for (int j = 0; j < puyos.size(); j++) {

		if (puyos.at(j).color != targetLeft)//左が同じ色
			continue;
		if (oldLabel != deleteList.at(j))//昔の番号を入れ変え
			continue;
		deleteList.at(j) = label;
	}


}

void Board::Labeling_Method(std::vector<int>&deleteList, int& massNumber){

	labelList = deleteList;
	labelCountList.clear();
	labelCountList.resize(massNumber);

	for (int i = 0; i < labelList.size(); i++) {
		int countIndex = labelList.at(i);
		if (countIndex < 0)
			continue;

		++labelCountList.at(countIndex);
	}

}

void Board::Recursion(){
	std::vector<int>sell(puyos.size(), 0);//捜索検査
	int labelCount=0;//ラベル数
	std::vector<int>labelNum(puyos.size(),0);//塊個数

	for (int i = 0; i < puyos.size(); i++) {
		labelCount = 0;
		RecursionInLabeling(i,sell,labelCount,labelNum);
	}

	//for(int i=0;i<puyos.size();i++)
	//nums.at(i) = labelNum.at(i);
	labelList = labelNum;
}

void Board::RecursionInLabeling(int index, std::vector<int>& sell, int& labelCount, std::vector<int>& labelNum) {
	if (GetBoardPuyo(index) == -1) {
		labelNum.at(index) = -1;
		return;
	}

	if (sell.at(index) == 2)
		return;

	labelNum.at(index) = labelCount;
	sell.at(index) = 2;

	int x = index % BOARD_WIDTH;
	int y = index / BOARD_WIDTH;

	enum class Direction {
		left
		, up
		, right
		, down
		, max
	};
	//範囲外対策
	std::array<bool, static_cast<int>(Direction::max)> outOfRange;
	outOfRange.at(0) = x > 0;
	outOfRange.at(1) = y > 0;
	outOfRange.at(2) = x < (BOARD_WIDTH - 1);
	outOfRange.at(3) = y < (BOARD_HEIGHT - 1);

	//同じ色なら確認予約し、再帰へ
	for (int i = 0; i < static_cast<int>(Direction::max); i++) {

		//範囲外対策

		bool isOutOfRange = false;
		for (int directionNumber = 0; directionNumber < outOfRange.size(); directionNumber++) {
			bool byTheWall = (i == directionNumber);
			bool allFlag = (!outOfRange.at(directionNumber) && byTheWall);

			isOutOfRange = (isOutOfRange || allFlag);
		}
		if (isOutOfRange)
			continue;

		//if (!(x > 0) && (i == 0))continue;
		//if (!(y > 0) && (i == 1))continue;
		//if (!(x < BOARD_WIDTH-1) && (i == 2))continue;
		//if (!(y < BOARD_HEIGHT-1) && (i == 3))continue;

	//範囲外対策__________________________________________________________



		int degree = i * 90 + 90;//左から始める為 +90
		double radian = DX_PI_F / 180 * degree;

		Vec2 dir;
		dir.x = cos(radian) - sin(radian);
		dir.y = sin(radian) + cos(radian);

		int nextIndex = GetIndex(x + dir.x, y + dir.y);

		if (GetBoardPuyo(index) != GetBoardPuyo(nextIndex))
			continue;
		if (sell.at(nextIndex) == 2)
			continue;

		++labelCount;
		RecursionInLabeling(nextIndex, sell, labelCount, labelNum);
	}
}

int Board::GetBoardPuyo(int index){
	if (index < 0)
		return -1;
	if (index >= puyos.size())
		return -1;
	return puyos.at(index).color;
}
void Board::SetBoardPos(int color, int x, int y) {
	if (x < 0)
		return;
	if (y < 0)
		return;
	if (x >= BOARD_WIDTH)
		return;
	if (y >= BOARD_HEIGHT)
		return;

	SetBoardPos(color, GetIndex(x, y)); 
}

void Board::SetBoardPos(int color,int index) {
	if (index >= puyos.size())
		return;
	if (index < 0)
		return;
	puyos.at(index).color = color;

	SetLabeling();
}