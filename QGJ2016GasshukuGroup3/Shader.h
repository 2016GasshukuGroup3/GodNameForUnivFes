#pragma once

/**
* @file Shader.h
* シェーダーを用いて描画する機能を提供します。
*/

/// <summary>シェーダーなどのリソースを初期化します。</summary>
int InitializeShaderResourse();

/// <summary>画像の透過部分と非透過部分の境界を塗りつぶしたグラフィックを作成します。</summary>
/// <param name="fileName">読み込むファイル名</param>
int LoadGraphEdge(const char fileName[]);

/// <summary>画像の透過部分と非透過部分の境界を指定した色で塗ります。</summary>
/// <param name="x">画像を描画する x 座標</param>
/// <param name="y">画像を描画する y 座標</param>
/// <param name="grHandle">境界を塗る画像へのハンドル</param>
/// <param name="edgeColor">境界色</param>
/// <returns>描画に成功すれば 0, そうでなければ -1</returns>
int DrawGraphEdge(int x, int y, int grHandle, int edgeColor);