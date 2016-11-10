#include "Ranking.h"
#include "DxLib.h"
#include <vector>
#include <numeric>
#include <algorithm>



int SetRankingData(RankingData Data[], std::size_t IndexList[], int DataNum, const char RankPath[]) {
	// まず、スコアランキングが格納されているファイルをさがす。
	int FileHandle = FileRead_open(RankPath);

	// 読み込めなければあきらめる。
	if (FileHandle == -1) {
		return -1;
	}

	// スコアランキングを格納するファイルへのパス
	char FilePath[512];
	FileRead_gets(FilePath, _countof(FilePath), FileHandle);
	FileRead_close(FileHandle);

	// スコアランキングを格納するファイルを開く
	FILE* FilePointer;
	errno_t Err = fopen_s(&FilePointer, FilePath, "w");
	
	// 読み込めなければあきらめる。
	if (FilePointer == NULL) {
		return -1;
	}

	for (int i = 0; i < DataNum; i++) {
		const RankingData& CurrentData = Data[IndexList[i]];
		fprintf(FilePointer, "%d, %d, %s\n", CurrentData.DeathCount, CurrentData.ClearTime, CurrentData.Name);
	}

	fclose(FilePointer);

	return 0;
}

template <class PredicateType>
/// <summary>指定した述語関数オブジェクトを用いてランキングを登録および作成します。</summary>
/// <param name="Name">登録する名前</param>
/// <param name="DeathCount">死亡回数</param>
/// <param name="ClearTime">秒単位のクリア時間</param>
/// <param name="Predicate">ソートするときに使う述語関数オブジェクト</param>
/// <param name="RankPath">ランキングファイルの存在するパスを書いたテキストファイルのパス。</param>
int RegisterRankingInternal(const char Name[], int DeathCount, int ClearTime, PredicateType Predicate, const char RankPath[]) {
	// ランキングの情報を格納する配列
	RankingData Data[MaxScoreNum + 1];
	// ランキングの情報の数
	int DataNum = GetRankingData(Data, MaxScoreNum, RankPath);

	// エラーが発生していれば何もしない
	if (DataNum == -1) {
		return -1;
	}

	// 新しいランキングデータを格納するための変数への参照
	// 1, 2, ... , (DataNum - 1) 番目には既にデータが入っているため、
	// DataNum 番目にデータを書き込む。
	// (既に100券登録されていることも踏まえて、Data の要素数はわざと1多くした。)
	RankingData &NewData = Data[DataNum];
	
	// 新しいデータを入力
	NewData.DeathCount = DeathCount;
	NewData.ClearTime = ClearTime;
	strcpy_s(NewData.Name, Name);

	// 述語関数の準備
	Predicate.SetData(Data);

	// ソートの結果を記録するリスト
	std::size_t RankingIndex[_countof(Data)];
	// 0, 1, ..., DataNum の数字で埋める。
	std::iota(RankingIndex, RankingIndex + DataNum + 1, 0);

	// ランキングをソートし、その結果を RankingIndex に格納
	// このソートのやり方については、「c++ インデックスソート」
	// で検索してみてください。
	std::sort(RankingIndex, RankingIndex + DataNum + 1, Predicate);

	// 完成したランキングデータを保存する。
	SetRankingData(Data, RankingIndex, DataNum + 1, RankPath);

	return 0;
}

int RegisterRanking(const char Name[], int DeathCount, int ClearTime) {

	/// <summary>死亡回数でソートするための述語関数オブジェクト</summary>
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

	/// <summary>クリア時間でソートするための述語関数オブジェクト</summary>
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

	// 死亡回数ランキングを更新
	int RetVal1 = RegisterRankingInternal(Name, DeathCount, ClearTime, DeathCountComparar(), "Data/DeathCountRankPath.txt");
	// クリア時間ランキングを更新
	int RetVal2 = RegisterRankingInternal(Name, DeathCount, ClearTime, ClearTimeComparar(), "Data/ClearTimeRankPath.txt");

	// もし一方でもエラーが発生していれば -1 を戻り値としてセット
	if (RetVal1 == -1 || RetVal2 == -1) {
		return -1;
	} else {
		return 0;
	}
}

int GetRankingData(RankingData Data[], int DataNum, const char RankPath[]) {
	// まず、スコアランキングが格納されているファイルをさがす。
	FILE* FilePointer;
	fopen_s(&FilePointer, RankPath, "r");

	// 読み込めなければあきらめる。
	if (FilePointer == NULL) {
		return -1;
	}

	// スコアランキングを格納するファイルへのパス
	char FilePath[512];
	fgets(FilePath, _countof(FilePath), FilePointer);
	fclose(FilePointer);

	// スコアランキングを格納するファイルを開く
	errno_t Err = fopen_s(&FilePointer, FilePath, "r");

	// 読み込めなければあきらめる。
	if (FilePointer == NULL) {
		return -1;
	}

	int ScoreNum = 0;
	
	// ランキングの読み込み
	for (int i = 0; i < DataNum && i < MaxScoreNum; i++) {
		// ランキングに名前が登録されていないことに対処するため、あらかじめ名前に空文字をセット
		strcpy_s(Data[i].Name, _countof(Data[i].Name), "");

		// もし、それ以上読み込めなかったらそこで読み込みを打ち切る。
		if (fscanf_s(FilePointer, "%d, %d, %s", &Data[i].DeathCount, &Data[i].ClearTime, Data[i].Name, _countof(Data[i].Name)) == EOF) {
			break;
		};
		ScoreNum++;
	}

	fclose(FilePointer);
	return ScoreNum;
}