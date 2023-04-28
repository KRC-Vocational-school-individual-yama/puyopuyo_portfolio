#include "player.h"
#include"libs/scene.h"
#include"board.h"
#include"libs/keyManager.h"


namespace {
	KeyManager* keys=nullptr;
}

Player::Player(Scene* sc)
	:Object(sc)
	,puyo()
	,mainPos{2,0}
	,puyoDirection{0}
	,follCount{1}
	,isDisp{true}
{
	keys = new KeyManager;
}

Player::~Player(){

}

void Player::Init(){
	InitPlayerPuyo();

	puyoDirection = EnumCast(Direction::down);
	Vec2 second = GetDirectionPuyo(puyoDirection);
	puyo.at(1).pos = second;
}

void Player::Update(){
	follCount++;

	//PuyoFoll();

	//debug 色変え
	if (keys->Down(KEY_INPUT_TAB)) {
		InitPlayerPuyo();
	}
	
	//移動
	//Playable();

	//設置
	if (keys->Down(KEY_INPUT_RETURN)) {
		SetBoard();
		InitPlayerPuyo();
		GetScene()->Invoke<Board>()->PuyoFoll();//Labelingも兼ねてる
	}


}

void Player::Draw(){
	if ( ! isDisp)return;
	using config = Board;
	SetDrawArea(config::BOARD_FAST_X, config::BOARD_FAST_Y + config::PUYO_SIZE / 3 +4, config::BOARD_FAST_X + config::BOARD_WIDTH * config::PUYO_SIZE, config::BOARD_FAST_Y + config::BOARD_HEIGHT * config::PUYO_SIZE);

	for (int i = 0; i < puyo.size(); i++) {
		int color = puyo.at(i).color;
		if (color <= -1)continue;

		int size = Board::PUYO_SIZE;
		Vec2 boardPos =  mainPos + puyo.at(i).pos;//盤面の番号を求める
		Vec2 fastPos = {Board::BOARD_FAST_X,Board::BOARD_FAST_Y};
		Vec2 dispPos = fastPos + boardPos * Vec2{size,size};//画面の座標に変換
		int graph = GetScene()->Invoke<Board>()->GethImages(color);

		DrawExtendGraph(dispPos.x, dispPos.y, dispPos.x + size, dispPos.y + size, graph, TRUE);
	}
	SetDrawAreaFull();

	printfDx("posx= %d\n",mainPos.x);
	printfDx("posy= %d\n",mainPos.y);
	Vec2 dir = GetDirectionPuyo(puyoDirection);
	printfDx("dir.x= %d\n", dir.x);
	printfDx("dir.y= %d\n", dir.y);
	printfDx("dirnum= %d\n", puyoDirection);


}

bool Player::PuyoFoll(){
	Playable();

	if (follCount% 60 != 0)
		return false;

	Vec2 nextMove = Vec2{ 0, 1 };
	Vec2 checkMain = mainPos + nextMove;
	Vec2 checkPair = mainPos + GetDirectionPuyo(MyDirection()) + nextMove;
	if (CheckMove(checkMain)&&CheckMove(checkPair)) {
		mainPos += nextMove;
		
		return false;
	}
	//SetBoard();
		//InitPlayerPuyo();
		//GetScene()->Invoke<Board>()->PuyoFoll();//Labelingも兼ねてる
		

	return true;
}

void Player::InitPlayerPuyo(){
	puyoDirection = EnumCast(Direction::down);
	mainPos= {2,0};
	puyo.clear();
	puyo.resize(2,Puyo{});
	puyo.at(1).pos = GetDirectionPuyo(puyoDirection);
	int maxCol = GetScene()->Invoke<Board>()->GetColorMax();

	for (int i = 0; i < puyo.size(); i++) {
		puyo.at(i).color = GetRand(maxCol);
	}
}

void Player::SetBoard() {

	for (int i = 0; i < puyo.size(); i++) {
		int posX=mainPos.x+puyo.at(i).pos.x;
		int posY=mainPos.y+puyo.at(i).pos.y;
		GetScene()->Invoke<Board>()->SetBoardPos(puyo.at(i).color, posX,posY);
	}

}

Vec2 Player::GetDirectionPuyo(int dir){

	int degree = dir*90 + 90;//右から始める為 +270
	double radian = DX_PI_F / 180 * degree;

	Vec2 ans;
	ans.x = cos(radian) - sin(radian);
	ans.y = sin(radian) + cos(radian);


	return ans;
}

bool Player::CheckMove(int x, int y){
	Board* board = GetScene()->Invoke<Board>();
	
	return board->IsEmptyGrid(x,y);
}
int Player::MyDirection(){
	return (puyoDirection % EnumCast(Direction::max));
}
void Player::Playable(){
	//移動
	Movement();

	//回転
	Rotation();
}
void Player::Movement(){	
	//移動

	
	Vec2 nextMove{
		(keys->Down(KEY_INPUT_RIGHT) || keys->Down(KEY_INPUT_D))-
		(keys->Down(KEY_INPUT_LEFT) || keys->Down(KEY_INPUT_A)), 
		(keys->Down(KEY_INPUT_DOWN) || keys->Down(KEY_INPUT_S)) -
		(keys->Down(KEY_INPUT_UP) || keys->Down(KEY_INPUT_W))
	};

	//int moveDir=EnumCast(Direction::non);
	//
	//auto upAndDown = [&]() {
	//if (keys->Down(KEY_INPUT_UP) || keys->Down(KEY_INPUT_W))
	//	moveDir = EnumCast(Direction::up);
	//
	//if (keys->Down(KEY_INPUT_DOWN) || keys->Down(KEY_INPUT_S))
	//	moveDir = EnumCast(Direction::down);
	//
	//};
	//auto leftAndRight = [&]() {
	//if (keys->Down(KEY_INPUT_RIGHT) || keys->Down(KEY_INPUT_D))
	//	moveDir = EnumCast(Direction::right);
	//
	//if (keys->Down(KEY_INPUT_LEFT) || keys->Down(KEY_INPUT_A))
	//	moveDir = EnumCast(Direction::left);
	//
	//};
	//
	//
	//
	//upAndDown();
	//leftAndRight();
	//
	//
	//Vec2 nextMove = GetDirectionPuyo(moveDir);

	Vec2 checkMain = mainPos + nextMove;
	Vec2 checkPair = mainPos + nextMove + GetDirectionPuyo(MyDirection());
	if (CheckMove(checkMain) && CheckMove(checkPair))
		mainPos += nextMove;

}
void Player::Rotation() {

	//回転

	bool leftLot = keys->Down(KEY_INPUT_Q);
	if (leftLot) {
		puyoDirection = (puyoDirection - 1) < 0 ? EnumCast(Direction::max) - 1 : puyoDirection - 1;
		int sendDir = puyoDirection % EnumCast(Direction::max);
		Vec2 second = GetDirectionPuyo(sendDir);
		puyo.at(1).pos = second;
	}
	bool rightLot = keys->Down(KEY_INPUT_E);
	if (rightLot) {
		puyoDirection = (puyoDirection + 1 > EnumCast(Direction::max)) ? EnumCast(Direction::up) : puyoDirection + 1;
		int sendDir = puyoDirection % EnumCast(Direction::max);
		Vec2 second = GetDirectionPuyo(sendDir);
		puyo.at(1).pos = second;
	}

}
void Player::Reset() {
	InitPlayerPuyo();
}

void Player::OnDisp(){
	isDisp = true;
}

void Player::OffDisp(){
	isDisp = false;
}
