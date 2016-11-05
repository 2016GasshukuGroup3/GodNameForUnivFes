#include "Effects.h"
#include "Asset.h"
#include "DxLib.h"
#include <cmath>

void InitializeAsteroidEffect(AsteroidEffect* effect) {
	// �P���ȏ������B
	effect->x = effect->y = effect->dx = effect->dy = 0;
	effect->IsEnabled = false;
}

void DrawAsteroidEffect(const AsteroidEffect* effect) {
	// �O���t�B�b�N��ǂݍ��݁A"覐�" �Ƃ������O�Ŏ��o����悤�ɂ���B
	// �i����֐����ĂԂ��A�O���t�B�b�N��ǂݍ��ނ͈̂�񂾂��ɂȂ�悤�ɂȂ��Ă��܂��B
	// �����ɂ���̂��C�ɓ���Ȃ���΁A�D���ȏꏊ�Ɉڂ��Ă��������B�j
	// ��. DX���C�u�����ɂ͂Ȃ��֐��ł��B�i"Asset.h"�Œ�`�j
	AddGraphicHandle("覐�", "Graphic/覐�.png");

	// �ǂݍ���ł������O���t�B�b�N�n���h�����擾
	// ��. DX���C�u�����ɂ͂Ȃ��֐��ł��B�i"Asset.h"�Œ�`�j
	int GraphHandle = GetHandle("覐�");

	// �O���t�B�b�N���ǂꂮ�炢��]���ĕ`�悷�邩
	double Rotation = DX_PI - atan2(effect->dy, effect->dx);

	// �i�s�����ɐi��ł���悤�Ɍ����邽�߂ɁA�O���t�B�b�N����]�����ĕ`�悵�܂��B
	// ��]�ʂ́A�i�s�����x�N�g���� x ���ƂȂ��p�x����o����̂ł����A
	// 覐΂̃O���t�B�b�N���������ɂȂ��Ă��邽�߁A DX_PI - atan2(effect->dy, effect->dx)
	// �Ƃ����v�Z���ɂȂ��Ă��܂��B

	// �摜��`��B�i1.0 �͊g�嗦�ł��j
	DrawRotaGraph(effect->x, effect->y, 1.0, -Rotation, GraphHandle, TRUE);
}