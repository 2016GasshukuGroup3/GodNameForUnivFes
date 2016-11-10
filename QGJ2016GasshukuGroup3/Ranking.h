#pragma once

/**
* @file Ranking.h
* ランキング登録およびランキング取得機能を提供します。
*/

/// <summary>スコアランキングを登録できる最大件数</summary>
static const int MaxScoreNum = 100;

/// <summary>スコアランキングのデータを格納する構造体</summary>
struct RankingData {
	/// <summary>死亡回数</summary>
	int DeathCount;
	/// <summary>クリア時間</summary>
	int ClearTime;
	/// <summary>名前</summary>
	char Name[128];
};

/// <summary>死亡回数ランキングを登録します。</summary>
/// <param name="Name">登録する名前</param>
/// <param name="DeathCount">死亡回数</param>
/// <param name="ClearTime">秒単位のクリア時間</param>
/// <param name="RankPath">
/// ランキングファイルの存在するパスを書いたテキストファイルのパス。
/// ランキングファイルをプログラム外から変更できるようにするため、ランキングファイルの存在するパスを
/// 外部ファイルから読み込むようにしています。このため、この引数にランキングファイルの存在するパスを
/// 渡しても正常に処理されないので気を付けてください。なお、この引数には、死亡回数のランキングを
/// 取得したい場合は、"Data/DeathCountRankPath.txt" を、クリア時間のランキングを取得したい場合は
/// "Data/ClearTimeRankPath.txt" を渡してください。
/// </param>
/// <returns>登録に成功すれば 0, そうでなければ -1</returns>
/// <remarks>
/// 使用例:
/// <code>
/// // 死亡回数が 20回, クリア時間が 80秒で, 名前を""(空文字)にしたいとき
/// RegisterDeathCountRanking("", 20, 80);
/// </code>
/// </remarks>
int RegisterDeathCountRanking(const char Name[], int DeathCount, int ClearTime, const char RankPath[] = "Data/DeathCountRankPath.txt");

/// <summary>死亡回数ランキングを登録します。</summary>
/// <param name="Name">登録する名前</param>
/// <param name="DeathCount">死亡回数</param>
/// <param name="ClearTime">秒単位のクリア時間</param>
/// <param name="RankPath">
/// ランキングファイルの存在するパスを書いたテキストファイルのパス。
/// ランキングファイルをプログラム外から変更できるようにするため、ランキングファイルの存在するパスを
/// 外部ファイルから読み込むようにしています。このため、この引数にランキングファイルの存在するパスを
/// 渡しても正常に処理されないので気を付けてください。なお、この引数には、死亡回数のランキングを
/// 取得したい場合は、"Data/DeathCountRankPath.txt" を、クリア時間のランキングを取得したい場合は
/// "Data/ClearTimeRankPath.txt" を渡してください。
/// </param>
/// <returns>登録に成功すれば登録されたデータの順位, そうでなければ -1</returns>
/// <remarks>
/// 使用例:
/// <code>
/// // 死亡回数が 20回, クリア時間が 80秒で, 名前を""(空文字)にしたいとき
/// RegisterClearTimeRanking("", 20, 80);
/// </code>
/// </remarks>
int RegisterClearTimeRanking(const char Name[], int DeathCount, int ClearTime, const char RankPath[] = "Data/ClearTimeRankPath.txt");

/// <summary>ランキングのスコアを取得します。</summary>
/// <param name="Data">死亡回数を格納するための RankingData 型の配列</param>
/// <param name="DataNum">Data の要素数</param>
/// <param name="RankPath">
/// ランキングファイルの存在するパスを書いたテキストファイルのパス。
/// ランキングファイルをプログラム外から変更できるようにするため、ランキングファイルの存在するパスを
/// 外部ファイルから読み込むようにしています。このため、この引数にランキングファイルの存在するパスを
/// 渡しても正常に処理されないので気を付けてください。なお、この引数には、死亡回数のランキングを
/// 取得したい場合は、"Data/DeathCountRankPath.txt" を、クリア時間のランキングを取得したい場合は
/// "Data/ClearTimeRankPath.txt" を渡してください。
/// </param>
/// <returns>ランキングの取得に成功すればランキングに登録されている件数, そうでなければ -1</returns>
/// <remarks>
/// 使用例:
/// <code>
/// // ランキングを入れる配列
/// RankingData Data[MaxScoreNum];
/// // 死亡回数のランキングを取得
/// int RankingNum = GetRankingData(Data, MaxScoreNum, "Data/DeathCountRankPath.txt");
/// 
/// // ランキングを表示
/// for (int i = 0; i &lt; RankingNum; i++) {
///		DrawFormatString(20, 20 + 20 * i, GetColor(255, 255, 255), "%3d位 %s %d 回 (%d 秒)", i, Data[i].Name, Data[i].DeathCount, Data[i].ClearTime);
/// }
/// </code>
/// </remarks>
int GetRankingData(RankingData Data[], int DataNum, const char RankPath[] = "Data/DeathCountRankPath.txt");
