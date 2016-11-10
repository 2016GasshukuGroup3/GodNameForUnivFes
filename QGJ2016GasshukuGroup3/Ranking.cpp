#include "Ranking.h"
#include "DxLib.h"
#include <vector>
#include <numeric>
#include <algorithm>



int SetRankingData(RankingData Data[], std::size_t IndexList[], int DataNum, const char RankPath[]) {
	// �܂��A�X�R�A�����L���O���i�[����Ă���t�@�C�����������B
	int FileHandle = FileRead_open(RankPath);

	// �ǂݍ��߂Ȃ���΂�����߂�B
	if (FileHandle == -1) {
		return -1;
	}

	// �X�R�A�����L���O���i�[����t�@�C���ւ̃p�X
	char FilePath[512];
	FileRead_gets(FilePath, _countof(FilePath), FileHandle);
	FileRead_close(FileHandle);

	// �X�R�A�����L���O���i�[����t�@�C�����J��
	FILE* FilePointer;
	errno_t Err = fopen_s(&FilePointer, FilePath, "w");
	
	// �ǂݍ��߂Ȃ���΂�����߂�B
	if (FilePointer == NULL) {
		return -1;
	}

	for (int i = 0; i < DataNum; i++) {
		const RankingData& CurrentData = Data[IndexList[i]];
		fprintf(FilePointer, "%d,%d,%s\n", CurrentData.DeathCount, CurrentData.ClearTime, CurrentData.Name);
	}

	fclose(FilePointer);

	return 0;
}

template <class PredicateType>
/// <summary>�w�肵���q��֐��I�u�W�F�N�g��p���ă����L���O��o�^����э쐬���܂��B</summary>
/// <param name="Name">�o�^���閼�O</param>
/// <param name="DeathCount">���S��</param>
/// <param name="ClearTime">�b�P�ʂ̃N���A����</param>
/// <param name="Predicate">�\�[�g����Ƃ��Ɏg���q��֐��I�u�W�F�N�g</param>
/// <param name="RankPath">�����L���O�t�@�C���̑��݂���p�X���������e�L�X�g�t�@�C���̃p�X�B</param>
int RegisterRankingInternal(const char Name[], int DeathCount, int ClearTime, PredicateType Predicate, const char RankPath[]) {
	// �����L���O�̏����i�[����z��
	RankingData Data[MaxScoreNum + 1];
	// �����L���O�̏��̐�
	int DataNum = GetRankingData(Data, MaxScoreNum, RankPath);

	// �G���[���������Ă���ΐV�K�쐬�̏���
	if (DataNum == -1) {
		DataNum = 0;
	}

	// �V���������L���O�f�[�^���i�[���邽�߂̕ϐ��ւ̎Q��
	// 1, 2, ... , (DataNum - 1) �Ԗڂɂ͊��Ƀf�[�^�������Ă��邽�߁A
	// DataNum �ԖڂɃf�[�^���������ށB
	// (����100���o�^����Ă��邱�Ƃ����܂��āAData �̗v�f���͂킴��1���������B)
	RankingData &NewData = Data[DataNum];
	
	// �V�����f�[�^�����
	NewData.DeathCount = DeathCount;
	NewData.ClearTime = ClearTime;
	strcpy_s(NewData.Name, Name);

	// �q��֐��̏���
	Predicate.SetData(Data);

	// �\�[�g�̌��ʂ��L�^���郊�X�g
	std::size_t RankingIndex[_countof(Data)];
	// 0, 1, ..., DataNum �̐����Ŗ��߂�B
	std::iota(RankingIndex, RankingIndex + DataNum + 1, 0);

	// �����L���O���\�[�g���A���̌��ʂ� RankingIndex �Ɋi�[
	// ���̃\�[�g�̂����ɂ��ẮA�uc++ �C���f�b�N�X�\�[�g�v
	// �Ō������Ă݂Ă��������B
	std::sort(RankingIndex, RankingIndex + DataNum + 1, Predicate);

	// �������������L���O�f�[�^��ۑ�����B
	SetRankingData(Data, RankingIndex, DataNum + 1, RankPath);

	return RankingIndex[DataNum] + 1;
}

int RegisterDeathCountRanking(const char Name[], int DeathCount, int ClearTime, const char RankPath[]) {

	/// <summary>���S�񐔂Ń\�[�g���邽�߂̏q��֐��I�u�W�F�N�g</summary>
	class DeathCountComparar {
		RankingData *m_Data;
	public:
		typedef bool result_type;

		void SetData(RankingData data[]) {
			m_Data = data;
		}

		bool operator()(std::size_t a, std::size_t b) {
			if (m_Data[a].DeathCount != m_Data[b].DeathCount) {
				return m_Data[a].DeathCount < m_Data[b].DeathCount;
			} else {
				return m_Data[a].ClearTime < m_Data[b].ClearTime;
			}
		}
	};

	// ���S�񐔃����L���O���X�V
	return RegisterRankingInternal(Name, DeathCount, ClearTime, DeathCountComparar(), RankPath);
}

int RegisterClearTimeRanking(const char Name[], int DeathCount, int ClearTime, const char RankPath[]) {

	/// <summary>�N���A���ԂŃ\�[�g���邽�߂̏q��֐��I�u�W�F�N�g</summary>
	class ClearTimeComparar {
		RankingData *m_Data;
	public:
		typedef bool result_type;

		void SetData(RankingData data[]) {
			m_Data = data;
		}

		bool operator()(std::size_t a, std::size_t b) {
			if (m_Data[a].ClearTime != m_Data[b].ClearTime) {
				return m_Data[a].ClearTime < m_Data[b].ClearTime;
			}
			else {
				return m_Data[a].DeathCount < m_Data[b].DeathCount;
			}
		}
	};

	// �N���A���ԃ����L���O���X�V
	return RegisterRankingInternal(Name, DeathCount, ClearTime, ClearTimeComparar(), RankPath);
}

int GetRankingData(RankingData Data[], int DataNum, const char RankPath[]) {
	// �܂��A�X�R�A�����L���O���i�[����Ă���t�@�C�����������B
	FILE* FilePointer;
	fopen_s(&FilePointer, RankPath, "r");

	// �ǂݍ��߂Ȃ���΂�����߂�B
	if (FilePointer == NULL) {
		return -1;
	}

	// �X�R�A�����L���O���i�[����t�@�C���ւ̃p�X
	char FilePath[512];
	fgets(FilePath, _countof(FilePath), FilePointer);
	fclose(FilePointer);

	// �X�R�A�����L���O���i�[����t�@�C�����J��
	errno_t Err = fopen_s(&FilePointer, FilePath, "r");

	// �ǂݍ��߂Ȃ���΂�����߂�B
	if (FilePointer == NULL) {
		return -1;
	}

	int ScoreNum = 0;
	
	// �����L���O�̓ǂݍ���
	for (int i = 0; i < DataNum && i < MaxScoreNum; i++) {
		// �����L���O�ɖ��O���o�^����Ă��Ȃ����ƂɑΏ����邽�߁A���炩���ߖ��O�ɋ󕶎����Z�b�g
		strcpy_s(Data[i].Name, _countof(Data[i].Name), "");

		// �����A����ȏ�ǂݍ��߂Ȃ������炻���œǂݍ��݂�ł��؂�B
		if (fscanf_s(FilePointer, "%d,%d,%[^\n]", &Data[i].DeathCount, &Data[i].ClearTime, Data[i].Name, _countof(Data[i].Name)) == EOF) {
			break;
		};
		ScoreNum++;
	}

	fclose(FilePointer);
	return ScoreNum;
}