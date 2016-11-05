#pragma once

/**
* @file Effects.h
* 各種エフェクトを用意します。
*/

// ------------------------------------------------------------------------------------------
// あちこちにある <...> は関数のヒントを表示させるために必要なものです。
// （関数名にマウスポインタを合わせたり、関数の引数を入力するときなどにそのヒントが出ます）
// また、コメント中の &amp; は & を意味します。
// （だからと言って書き換えないでください。関数のヒントが正常に表示されない原因になります。）
// ------------------------------------------------------------------------------------------

/// <summary>隕石のエフェクトを実装します。</summary>
struct AsteroidEffect {
	/// <summary>配置座標</summary>
	int x, y;
	/// <summary>１フレームごとの移動量</summary>
	int dx, dy;
	/// <summary>エフェクトが有効かどうか</summary>
	bool IsEnabled;
};

/// <summary>隕石のエフェクトを初期化します。必ずしもこの関数を使って初期化する必要はありません。</summary>
/// <param name="effect">初期化する隕石のエフェクトを表す構造体のインスタンスへのポインタ。使用例のように使ってください。</param>
/// <remarks>
/// 使用例:
/// <code>
/// AsteroidEffect MyEffect;
/// InitializeAsteroidEffect(&amp;MyEffect);
/// </code>
/// </remarks>
void InitializeAsteroidEffect(AsteroidEffect* effect);

/// <summary>隕石のエフェクトを描画します。</summary>
/// <param name="effect">初期化する隕石のエフェクトを表す構造体のインスタンスへのポインタ。使用例のように使ってください。</param>
/// <remarks>
/// 使用例:
/// <code>
/// AsteroidEffect MyEffect;
/// DrawAsteroidEffect(&amp;MyEffect);
/// </code>
/// </remarks>
void DrawAsteroidEffect(const AsteroidEffect* effect);