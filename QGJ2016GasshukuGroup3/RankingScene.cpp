#include "Scenes.h"
#include "Ranking.h"
#include "Asset.h"
#include "Input.h"

// 初期化されているかどうか
static bool IsInitialized = false;
// 表示するランキングの件数
static const int RankingNum = 3;

// 死亡回数のランキング
static RankingData DeathCountRanking[RankingNum];
// クリア時間のランキング
static RankingData ClearTimeRanking[RankingNum];
// 死亡回数のランキングのうち、有効なスコアの数
static int ValidDeathCountRankingNum = 0;
// クリア時間のランキングのうち、有効なスコアの数
static int ValidClearTimeRankingNum = 0;

// 死亡回数のランキング
static RankingData DeathCountRanking_Hard[RankingNum];
// クリア時間のランキング
static RankingData ClearTimeRanking_Hard[RankingNum];
// 死亡回数のランキングのうち、有効なスコアの数
static int ValidDeathCountRankingNum_Hard = 0;
// クリア時間のランキングのうち、有効なスコアの数
static int ValidClearTimeRankingNum_Hard = 0;

// 死亡回数を表示するための数字
static int DeathCountNumberImages[10] = { -1 };
// クリア時間を表示するための数字
static int ClearTimeNumberImages[10] = { -1 };

// 内部カウンタ
static int Flames = 0;

// ランキングシーンの初期化処理を行います。
void InitializeRankingScene() {
	// 初期化されていない時だけ初期化を行う
	if (!IsInitialized) {
		Flames = 0;

		// ゲーム実行中一度だけグラフィック読み込む
		if (DeathCountNumberImages[0] == -1) {
			LoadDivGraph("Graphic/死亡回数リザルト数字.png", 10, 10, 1, 32, 36, DeathCountNumberImages);
			LoadDivGraph("Graphic/秒数リザルト数字.png", 10, 10, 1, 32, 36, ClearTimeNumberImages);
			AddGraphicHandle("ストーリ2", "Graphic/ストーリー2.png");
			AddGraphicHandle("score", "Graphic/score.png");
		}

		ValidDeathCountRankingNum = GetRankingData(DeathCountRanking, RankingNum, "Data/DeathCountRankPath.txt");
		ValidClearTimeRankingNum = GetRankingData(ClearTimeRanking, RankingNum, "Data/ClearTimeRankPath.txt");

		ValidDeathCountRankingNum_Hard = GetRankingData(DeathCountRanking_Hard, RankingNum, "Data/DeathCountRankPath_Hard.txt");
		ValidClearTimeRankingNum_Hard = GetRankingData(ClearTimeRanking_Hard, RankingNum, "Data/ClearTimeRankPath_Hard.txt");
	}

	IsInitialized = true;
}

// ランキングシーンの終了処理を行います。
void TerminateRankingScene() {
	IsInitialized = false;
}

STATE ranking() {
	// 初期化されていない時だけ初期化を行う
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

	DrawGraph(0, 0, GetHandle("ストーリ2"), FALSE);
	DrawGraph(0, 0, GetHandle("score"), TRUE);

	// 最初は EasyMode のランキングを表示
	if (Flames <= 10 * 60) {
		DrawGraph(350, 400, GetHandle("イージーモード"), TRUE);

		for (int i = 0; i < ValidDeathCountRankingNum; i++) {
			DrawNumber(250, 200 + 70 * i, DeathCountRanking[i].DeathCount, DeathCountNumberImages);
		}

		for (int i = 0; i < ValidClearTimeRankingNum; i++) {
			DrawNumber(-50, 200 + 70 * i, ClearTimeRanking[i].ClearTime, ClearTimeNumberImages);
		}
	} else {
		DrawGraph(350, 400, GetHandle("ハードモード"), TRUE);

		for (int i = 0; i < ValidDeathCountRankingNum; i++) {
			DrawNumber(250, 200 + 70 * i, DeathCountRanking_Hard[i].DeathCount, DeathCountNumberImages);
		}

		for (int i = 0; i < ValidClearTimeRankingNum; i++) {
			DrawNumber(-50, 200 + 70 * i, ClearTimeRanking_Hard[i].ClearTime, ClearTimeNumberImages);
		}
	}

	return RANKING;
}