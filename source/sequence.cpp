#include "sequence.h"
#include"libs/scene.h"
#include"board.h"
#include"player.h"
#include"libs/keyManager.h"

namespace {
	std::string pass = "data/texture/puyo/batankyu.png";
}

Sequence::Sequence(Scene* sc)
	:Object(sc)
	,sequenser{State::start}
	,board{nullptr}
	,player{nullptr}
	,keys{nullptr}
	,gameSpeedCount{0}
	,hImage{-1}
{}
Sequence::~Sequence(){
	delete keys;
	DeleteGraph(hImage);
}
void Sequence::Init(){
	hImage= LoadGraph(pass.c_str());
	assert(hImage > 0);
	board = GetScene()->Invoke<Board>();
	player = GetScene()->Invoke<Player>();
	keys = new KeyManager();
}
void Sequence::Update(){
	gameSpeedCount++;

	switch (sequenser) {
	case Sequence::State::start:		State_Start();			break;
	case Sequence::State::playerFoll:	State_PlayerFoll();		break;
	case Sequence::State::fieldSet:		State_FieldSet();		break;
	case Sequence::State::fieldFoll:	State_FieldFoll();		break;
	case Sequence::State::fieldErase:	State_FieldErase();		break;
	case Sequence::State::gameOver:		sequenser = State::end;	break;
	case Sequence::State::end:			State_End();			break;
	default:		break;
	}
}
void Sequence::Draw(){
	if ( !board->GameOver())
		return;

	DrawGraph(0, 200, hImage, true);

}

void Sequence::State_Start() {
	board->Reset();

	sequenser = State::playerFoll;
}
void Sequence::State_PlayerFoll() {
	if (player->PuyoFoll()) {
		sequenser = State::fieldSet;
	}
}
void Sequence::State_FieldSet() {
	if (gameSpeedCount % gameSpeed != 0)
		return;

	player->SetBoard();
	player->Reset();
	player->OffDisp();

	if (board->GameOver()) {
		sequenser = State::gameOver;
	}
	else {
		sequenser = State::fieldFoll;
	}
}
void Sequence::State_FieldFoll() {
	if (gameSpeedCount % gameSpeed != 0)
		return;

	board->PuyoFoll();
	sequenser = State::fieldErase;
}
void Sequence::State_FieldErase() {
	if (gameSpeedCount % gameSpeed != 0)
		return;

	if (board->PuyoErase()) {
		sequenser = State::fieldFoll;
	}
	else {
		player->OnDisp();
		sequenser = State::playerFoll;
	}

}
void Sequence::State_End() {
	if (keys->Down(KEY_INPUT_R)) {
		sequenser = State::start;
	}

}
