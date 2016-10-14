#pragma once

#include "BaseUnit.h"

class Lift {
	int Flames;
	CMap MyMap;
	int GraphHandle;
	int CurrentWidth;
public:
	// 初期位置
	__declspec(property(get = GetX, put = SetX))
	int X;
	// 初期位置
	__declspec(property(get = GetY, put = SetY))
	int Y;
	// リフトの幅（タイル単位）
	__declspec(property(get = GetWidth, put = SetWidth))
	int Width;

	bool IsEnabled;

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

	void SetWidth(int width) {
		MyMap.Fill(-1);
		CurrentWidth = width;

		for (int i = 20; i < 20 + width; i++) {
			MyMap[i][25] = 0;
		}
	}

	int GetWidth() {
		return CurrentWidth;
	}

	// 動くパターン
	enum MovePattern {
		Side,		// 左右に移動
		UpAndDown,	// 上下に移動
		Rotation,	// 回転
		DoNotMove,	// 動かない
		Fall		// 落ちる
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