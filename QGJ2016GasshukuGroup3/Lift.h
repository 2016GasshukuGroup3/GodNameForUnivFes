#pragma once

#include "BaseUnit.h"

class Lift {
	int Flames;
	CMap MyMap;
	int GraphHandle;
	int CurrentWidth;
public:
	// �����ʒu
	__declspec(property(get = GetX, put = SetX))
	int X;
	// �����ʒu
	__declspec(property(get = GetY, put = SetY))
	int Y;
	// ���t�g�̕��i�^�C���P�ʁj
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

	// �����p�^�[��
	enum MovePattern {
		Side,		// ���E�Ɉړ�
		UpAndDown,	// �㉺�Ɉړ�
		Rotation,	// ��]
		DoNotMove,	// �����Ȃ�
		Fall		// ������
	} MyPattern;

	Lift();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// ���Z�b�g
	void Reset();

	void ApplyParticles(class Particle&);

	// �����蔻��I�u�W�F�N�g
	CMap& GetCollider() {
		return MyMap;
	}
};