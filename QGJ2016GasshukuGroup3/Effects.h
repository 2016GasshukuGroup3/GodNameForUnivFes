#pragma once

/**
* @file Effects.h
* �e��G�t�F�N�g��p�ӂ��܂��B
*/

// ------------------------------------------------------------------------------------------
// ���������ɂ��� <...> �͊֐��̃q���g��\�������邽�߂ɕK�v�Ȃ��̂ł��B
// �i�֐����Ƀ}�E�X�|�C���^�����킹����A�֐��̈�������͂���Ƃ��Ȃǂɂ��̃q���g���o�܂��j
// �܂��A�R�����g���� &amp; �� & ���Ӗ����܂��B
// �i������ƌ����ď��������Ȃ��ł��������B�֐��̃q���g������ɕ\������Ȃ������ɂȂ�܂��B�j
// ------------------------------------------------------------------------------------------

/// <summary>覐΂̃G�t�F�N�g���������܂��B</summary>
struct AsteroidEffect {
	/// <summary>�z�u���W</summary>
	int x, y;
	/// <summary>�P�t���[�����Ƃ̈ړ���</summary>
	int dx, dy;
	/// <summary>�G�t�F�N�g���L�����ǂ���</summary>
	bool IsEnabled;
};

/// <summary>覐΂̃G�t�F�N�g�����������܂��B�K���������̊֐����g���ď���������K�v�͂���܂���B</summary>
/// <param name="effect">����������覐΂̃G�t�F�N�g��\���\���̂̃C���X�^���X�ւ̃|�C���^�B�g�p��̂悤�Ɏg���Ă��������B</param>
/// <remarks>
/// �g�p��:
/// <code>
/// AsteroidEffect MyEffect;
/// InitializeAsteroidEffect(&amp;MyEffect);
/// </code>
/// </remarks>
void InitializeAsteroidEffect(AsteroidEffect* effect);

/// <summary>覐΂̃G�t�F�N�g��`�悵�܂��B</summary>
/// <param name="effect">����������覐΂̃G�t�F�N�g��\���\���̂̃C���X�^���X�ւ̃|�C���^�B�g�p��̂悤�Ɏg���Ă��������B</param>
/// <remarks>
/// �g�p��:
/// <code>
/// AsteroidEffect MyEffect;
/// DrawAsteroidEffect(&amp;MyEffect);
/// </code>
/// </remarks>
void DrawAsteroidEffect(const AsteroidEffect* effect);