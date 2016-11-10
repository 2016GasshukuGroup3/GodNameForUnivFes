#pragma once

/**
* @file Ranking.h
* �����L���O�o�^����у����L���O�擾�@�\��񋟂��܂��B
*/

/// <summary>�X�R�A�����L���O��o�^�ł���ő匏��</summary>
static const int MaxScoreNum = 100;

/// <summary>�X�R�A�����L���O�̃f�[�^���i�[����\����</summary>
struct RankingData {
	/// <summary>���S��</summary>
	int DeathCount;
	/// <summary>�N���A����</summary>
	int ClearTime;
	/// <summary>���O</summary>
	char Name[128];
};

/// <summary>�����L���O��o�^���܂��B</summary>
/// <param name="Name">�o�^���閼�O</param>
/// <param name="DeathCount">���S��</param>
/// <param name="ClearTime">�b�P�ʂ̃N���A����</param>
/// <returns>�o�^�ɐ�������� 0, �����łȂ���� -1</returns>
/// <remarks>
/// �g�p��:
/// <code>
/// // ���S�񐔂� 20��, �N���A���Ԃ� 80�b��, ���O��""(�󕶎�)�ɂ������Ƃ�
/// RegisterRanking("", 20, 80);
/// </code>
/// </remarks>
int RegisterRanking(const char Name[], int DeathCount, int ClearTime);

/// <summary>�����L���O�̃X�R�A���擾���܂��B</summary>
/// <param name="Data">���S�񐔂��i�[���邽�߂� RankingData �^�̔z��</param>
/// <param name="DataNum">Data �̗v�f��</param>
/// <param name="RankPath">
/// �����L���O�t�@�C���̑��݂���p�X���������e�L�X�g�t�@�C���̃p�X�B
/// �����L���O�t�@�C�����v���O�����O����ύX�ł���悤�ɂ��邽�߁A�����L���O�t�@�C���̑��݂���p�X��
/// �O���t�@�C������ǂݍ��ނ悤�ɂ��Ă��܂��B���̂��߁A���̈����Ƀ����L���O�t�@�C���̑��݂���p�X��
/// �n���Ă�����ɏ�������Ȃ��̂ŋC��t���Ă��������B�Ȃ��A���̈����ɂ́A���S�񐔂̃����L���O��
/// �擾�������ꍇ�́A"Data/DeathCountRankPath.txt" ���A�N���A���Ԃ̃����L���O���擾�������ꍇ��
/// "Data/ClearTimeRankPath.txt" ��n���Ă��������B
/// </param>
/// <returns>�����L���O�̎擾�ɐ�������΃����L���O�ɓo�^����Ă��錏��, �����łȂ���� -1</returns>
/// <remarks>
/// �g�p��:
/// <code>
/// // �����L���O������z��
/// RankingData Data[MaxScoreNum];
/// // ���S�񐔂̃����L���O���擾
/// int RankingNum = GetRankingData(Data, MaxScoreNum, "Data/DeathCountRankPath.txt");
/// 
/// // �����L���O��\��
/// for (int i = 0; i < RankingNum; i++) {
///		DrawFormatString(20, 20 + 20 * i, GetColor(255, 255, 255), "%3d�� %s %d �� (%d �b)", i, Data[i].Name, Data[i].DeathCount, Data[i].ClearTime);
/// }
/// </code>
/// </remarks>
int GetRankingData(RankingData Data[], int DataNum, const char RankPath[] = "Data/DeathCountRankPath.txt");
