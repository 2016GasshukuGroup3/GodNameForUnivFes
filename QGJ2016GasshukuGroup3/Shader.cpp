#include "Shader.h"
#include "DxLib.h"

static bool IsInitialized = false;
static int EdgeDrawingShader = -1;
static int BufferScreen = -1;

int InitializeShaderResourse() {
	if (!IsInitialized) {
		EdgeDrawingShader = LoadPixelShader("EdgeDrawer.pso");
		BufferScreen = MakeScreen(640, 480, TRUE);
	} else {
		return -1;
	}

	IsInitialized = true;
	return 0;
}

int LoadGraphEdge(const char fileName[]) {
	int TempGraph = LoadGraph(fileName);
	int GraphWidth, GraphHeight;
	GetGraphSize(TempGraph, &GraphWidth, &GraphHeight);

	int NewGraphicHandle = MakeScreen(GraphWidth, GraphHeight, TRUE);
	SetDrawScreen(NewGraphicHandle);
	DrawGraphEdge(0, 0, TempGraph, GetColor(0, 0, 0));
	SetDrawScreen(DX_SCREEN_BACK);

	DeleteGraph(TempGraph);

	return NewGraphicHandle;
}

int DrawGraphEdge(int x, int y, int grHandle, int edgeColor) {
	if (!IsInitialized) {
		return -1;
	}

	VERTEX2DSHADER Vertexes[4];
	int GraphWidth, GraphHeight;
	GetGraphSize(grHandle, &GraphWidth, &GraphHeight);

	// 今の画面を保存
	int PrevScreen = GetDrawScreen();

	SetDrawScreen(BufferScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, grHandle, TRUE);
	SetDrawScreen(PrevScreen);

	// 頂点の設定
	Vertexes[0].pos = VGet(static_cast<float>(x), static_cast<float>(y), 0.0f);
	Vertexes[1].pos = VGet(static_cast<float>(x + GraphWidth), static_cast<float>(y), 0.0f);
	Vertexes[2].pos = VGet(static_cast<float>(x), static_cast<float>(y + GraphHeight), 0.0f);
	Vertexes[3].pos = VGet(static_cast<float>(x + GraphWidth), static_cast<float>(y + GraphHeight), 0.0f);

	// テクスチャ座標の設定
	Vertexes[0].u = 0.0f; Vertexes[0].v = 0.0f;
	Vertexes[1].u = GraphWidth / 640.0f; Vertexes[1].v = 0.0f;
	Vertexes[2].u = 0.0f; Vertexes[2].v = GraphHeight / 480.0f;
	Vertexes[3].u = GraphWidth / 640.0f; Vertexes[3].v = GraphHeight / 480.0f;

	// 色の設定
	int Red, Green, Blue;
	GetColor2(edgeColor, &Red, &Green, &Blue);
	COLOR_U8 Color = GetColorU8(Red, Green, Blue, 255);

	Vertexes[0].dif = Color;
	Vertexes[1].dif = Color;
	Vertexes[2].dif = Color;
	Vertexes[3].dif = Color;

	// 定数の設定
	FLOAT4 GraphSize;

	GraphSize.x = static_cast<float>(640);
	GraphSize.y = static_cast<float>(480);
	GraphSize.z = 1.f / static_cast<float>(640);
	GraphSize.w = 1.f / static_cast<float>(480);

	SetUsePixelShader(EdgeDrawingShader);
	SetUseTextureToShader(0, BufferScreen);
	SetPSConstF(22, GraphSize);

	unsigned short Indexes[6] = { 0, 1, 3, 0, 3, 2 };
	DrawPolygonIndexed2DToShader(Vertexes, 6, Indexes, 2);

	SetUseTextureToShader(0, -1);
}