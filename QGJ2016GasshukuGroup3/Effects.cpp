#include "Effects.h"
#include "Asset.h"
#include "DxLib.h"
#include <cmath>

void InitializeAsteroidEffect(AsteroidEffect* effect) {
	// 単純な初期化。
	effect->x = effect->y = effect->dx = effect->dy = 0;
	effect->IsEnabled = false;
}

void DrawAsteroidEffect(const AsteroidEffect* effect) {
	// グラフィックを読み込み、"隕石" という名前で取り出せるようにする。
	// （毎回関数を呼ぶが、グラフィックを読み込むのは一回だけになるようになっています。
	// ここにあるのが気に入らなければ、好きな場所に移してください。）
	// 注. DXライブラリにはない関数です。（"Asset.h"で定義）
	AddGraphicHandle("隕石", "Graphic/隕石.png");

	// 読み込んでおいたグラフィックハンドルを取得
	// 注. DXライブラリにはない関数です。（"Asset.h"で定義）
	int GraphHandle = GetHandle("隕石");

	// グラフィックをどれぐらい回転して描画するか
	double Rotation = DX_PI - atan2(effect->dy, effect->dx);

	// 進行方向に進んでいるように見せるために、グラフィックを回転させて描画します。
	// 回転量は、進行方向ベクトルが x 軸となす角度から出せるのですが、
	// 隕石のグラフィックが左向きになっているため、 DX_PI - atan2(effect->dy, effect->dx)
	// という計算式になっています。

	// 画像を描画。（1.0 は拡大率です）
	DrawRotaGraph(effect->x, effect->y, 1.0, -Rotation, GraphHandle, TRUE);
}