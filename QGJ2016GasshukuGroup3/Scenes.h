#pragma once
#include "Particles.h"
#include "Object.h"
// �e��G�t�F�N�g���g����悤�ɂ��܂��B
#include "Effects.h"

enum STATE {
	EXIT = -1,
	TITLE = 0,
	GAME,
	BOSS,
	RESULT,
	GAMEOVER,
	SETSUMEI
};

class Boss : public Rect{
	//Handle�n
	const int W = 20, H = 15;
	int ax, ay;
	int hp,maxhp;
	int pattern;
	bool gameover,flag = false;
	Rect p;
	vector<vector<Rect>> tile;

	static const int MaxAsteroidNum = 20;
	AsteroidEffect Asteroids[MaxAsteroidNum];

	// �c�莞�Ԃ��擾���܂��B
	int GetLeftTime() const;
public:
	int body, arm, bgm, time,time2;
	Boss();
	void Init();
	void Update();
	void Draw();
	bool IsOver();
	bool IsEnd();
};

STATE title();
STATE game();
STATE boss();
STATE result();
STATE gameover();
STATE setsumei();