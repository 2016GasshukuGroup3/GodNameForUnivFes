#pragma once

#include "BaseUnit.h"

class Lift {
	int Flames;
	CMap MyMap;
	int GraphHandle;
public:
	// �����ʒu
	__declspec(property(get = GetX, put = SetX))
	int X;
	// �����ʒu
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

	// �����p�^�[��
	enum MovePattern {
		Side,
		UpAndDown,
		Rotation
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