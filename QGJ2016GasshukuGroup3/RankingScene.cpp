#include "Scenes.h"
#include "Ranking.h"
#include "Asset.h"
#include "Input.h"

// ����������Ă��邩�ǂ���
static bool IsInitialized = false;
// �\�����郉���L���O�̌���
static const int RankingNum = 3;

// ���S�񐔂̃����L���O
static RankingData DeathCountRanking[RankingNum];
// �N���A���Ԃ̃����L���O
static RankingData ClearTimeRanking[RankingNum];
// ���S�񐔂̃����L���O�̂����A�L���ȃX�R�A�̐�
static int ValidDeathCountRankingNum = 0;
// �N���A���Ԃ̃����L���O�̂����A�L���ȃX�R�A�̐�
static int ValidClearTimeRankingNum = 0;

// ���S�񐔂̃����L���O
static RankingData DeathCountRanking_Hard[RankingNum];
// �N���A���Ԃ̃����L���O
static RankingData ClearTimeRanking_Hard[RankingNum];
// ���S�񐔂̃����L���O�̂����A�L���ȃX�R�A�̐�
static int ValidDeathCountRankingNum_Hard = 0;
// �N���A���Ԃ̃����L���O�̂����A�L���ȃX�R�A�̐�
static int ValidClearTimeRankingNum_Hard = 0;

// ���S�񐔂�\�����邽�߂̐���
static int DeathCountNumberImages[10] = { -1 };
// �N���A���Ԃ�\�����邽�߂̐���
static int ClearTimeNumberImages[10] = { -1 };

// �����J�E���^
static int Flames = 0;

// �����L���O�V�[���̏������������s���܂��B
void InitializeRankingScene() {
	// ����������Ă��Ȃ����������������s��
	if (!IsInitialized) {
		Flames = 0;

		// �Q�[�����s����x�����O���t�B�b�N�ǂݍ���
		if (DeathCountNumberImages[0] == -1) {
			LoadDivGraph("Graphic/���S�񐔃��U���g����.png", 10, 10, 1, 32, 36, DeathCountNumberImages);
			LoadDivGraph("Graphic/�b�����U���g����.png", 10, 10, 1, 32, 36, ClearTimeNumberImages);
			AddGraphicHandle("�X�g�[��2", "Graphic/�X�g�[���[2.png");
			AddGraphicHandle("score", "Graphic/score.png");
		}

		ValidDeathCountRankingNum = GetRankingData(DeathCountRanking, RankingNum, "Data/DeathCountRankPath.txt");
		ValidClearTimeRankingNum = GetRankingData(ClearTimeRanking, RankingNum, "Data/ClearTimeRankPath.txt");

		ValidDeathCountRankingNum_Hard = GetRankingData(DeathCountRanking_Hard, RankingNum, "Data/DeathCountRankPath_Hard.txt");
		ValidClearTimeRankingNum_Hard = GetRankingData(ClearTimeRanking_Hard, RankingNum, "Data/ClearTimeRankPath_Hard.txt");
	}

	IsInitialized = true;
}

// �����L���O�V�[���̏I���������s���܂��B
void TerminateRankingScene() {
	IsInitialized = false;
}

STATE ranking() {
	// ����������Ă��Ȃ����������������s��
	if (!IsInitialized) {
		InitializeRankingScene();
	}

	Flames++;
	
	if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
		PlaySoundMem(GetHandle("KetteiSound"), DX_PLAYTYPE_BACK);
		TerminateRankingScene();
		return TITLE;
	} else if (Flames > 20 * 60) {
		TerminateRankingScene();
		return TITLE;
	}

	DrawGraph(0, 0, GetHandle("�X�g�[��2"), FALSE);
	DrawGraph(0, 0, GetHandle("score"), TRUE);

	// �ŏ��� EasyMode �̃����L���O��\��
	if (Flames <= 10 * 60) {
		DrawGraph(350, 400, GetHandle("�C�[�W�[���[�h"), TRUE);

		for (int i = 0; i < ValidDeathCountRankingNum; i++) {
			DrawNumber(250, 200 + 70 * i, DeathCountRanking[i].DeathCount, DeathCountNumberImages);
		}

		for (int i = 0; i < ValidClearTimeRankingNum; i++) {
			DrawNumber(-50, 200 + 70 * i, ClearTimeRanking[i].ClearTime, ClearTimeNumberImages);
		}
	} else {
		DrawGraph(350, 400, GetHandle("�n�[�h���[�h"), TRUE);

		for (int i = 0; i < ValidDeathCountRankingNum; i++) {
			DrawNumber(250, 200 + 70 * i, DeathCountRanking_Hard[i].DeathCount, DeathCountNumberImages);
		}

		for (int i = 0; i < ValidClearTimeRankingNum; i++) {
			DrawNumber(-50, 200 + 70 * i, ClearTimeRanking_Hard[i].ClearTime, ClearTimeNumberImages);
		}
	}

	return RANKING;
}