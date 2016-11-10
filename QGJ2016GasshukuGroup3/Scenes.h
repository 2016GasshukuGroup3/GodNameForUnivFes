#pragma once
#include "Particles.h"
#include "Object.h"
// 各種エフェクトを使えるようにします。
#include "Effects.h"

enum STATE {
	EXIT = -1,
	TITLE = 0,
	GAME,
	BOSS,
	RESULT,
	GAMEOVER,
	SETSUMEI,
	RANKING
};

class Boss : public Rect{
	//Handle系
	const int W = 20, H = 15;
	int ax, ay;
	int hp,maxhp;
	int pattern;
	bool gameover,flag = false;
	Rect p;
	vector<vector<Rect>> tile;

	static const int MaxAsteroidNum = 20;
	AsteroidEffect Asteroids[MaxAsteroidNum];

	// 残り時間を取得します。
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
STATE ranking();

/// <summary>指定した座標に数字を描画します。</summary>
/// <param name="x">数字を描画する原点のx座標</param>
/// <param name="y">数字を描画する原点のy座標</param>
/// <param name="Number">描画する数字</param>
/// <param name="Images">描画する時に使う数字のグラフィック</param>
void DrawNumber(int x, int y, int Number, int Images[]);