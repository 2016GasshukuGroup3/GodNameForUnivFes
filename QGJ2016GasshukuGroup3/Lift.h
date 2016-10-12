#pragma once

#include "BaseUnit.h"

class Lift {
	int Flames;
	CMap MyMap;
	int GraphHandle;
public:
	// 初期位置
	__declspec(property(get = GetX, put = SetX))
	int X;
	// 初期位置
	__declspec(property(get = GetY, put = SetY))
	int Y;

	void SetX(int x) {
		MyMap.X = x - 20 * MapTile::MapSize;
	}

	int GetX() {
		return MyMap.X + 20 * MapTile::MapSize;
	}

	void SetY(int y) {
		MyMap.Y = y - 25 * MapTile::MapSize;
	}

	int GetY() {
		return MyMap.Y + 25 * MapTile::MapSize;
	}

	// 動くパターン
	enum MovePattern {
		Side,
		UpAndDown,
		Rotation
	} MyPattern;

	Lift();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// リセット
	void Reset();

	void ApplyParticles(class Particle&);

	// あたり判定オブジェクト
	CMap& GetCollider() {
		return MyMap;
	}
};