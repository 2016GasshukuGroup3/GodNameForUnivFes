#include "Lift.h"
#include "DxLib.h"
#include "Particles.h"
#include <cmath>
#include <assert.h>

Lift::Lift() :
	MyMap(60, 45, -1),
	GraphHandle(-1) {
	
	for (int i : { 20, 21, 22, 23, 24, 25 }) {
		MyMap[i][25] = 0;
	}
}

void Lift::Initialize() {
	if (GraphHandle == -1) {
		GraphHandle = MakeScreen(32 * 6, 32);
		int TempGraph = LoadGraph("Graphic/Jimen.png");
		SetDrawScreen(GraphHandle);

		for (int i = 0; i < 6; i++) {
			assert(DrawGraph(32 * i, 0, TempGraph, FALSE) == 0 || TempGraph == -1);
		}
		DeleteGraph(TempGraph);
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void Lift::Reset() {
	Flames = 0;
}

void Lift::Update() {

	switch (MyPattern) {
		case Lift::Side:
			if (Flames == 0) {
				MyMap.DeltaX = 2;
			} else if (Flames == 80) {
				MyMap.DeltaX = -2;
			} else if (Flames == 160 - 1) {
				Flames = -1;
			}
			break;
		case Lift::UpAndDown:
			if (Flames == 0) {
				MyMap.DeltaY = 2;
			} else if (Flames == 80) {
				MyMap.DeltaY = -2;
			} else if (Flames == 160 - 1) {
				Flames = -1;
			}
			break;
		case Lift::Rotation:
			MyMap.DeltaX = static_cast<int>(3 * cos(Flames * 2 * DX_PI / 360));
			MyMap.DeltaY = static_cast<int>(3 * sin(Flames * 2 * DX_PI / 360));
			
			if (Flames == 360 - 1) {
				Flames = -1;
			}
		default:
			break;
	}

	Flames++;

	MyMap.X += MyMap.DeltaX;
	MyMap.Y += MyMap.DeltaY;
}

void Lift::Draw() {
	assert(DrawGraph(MyMap.X + 20 * 32, MyMap.Y + 25 * 32, GraphHandle, FALSE) == 0);
}

void Lift::ApplyParticles(Particle& partile) {
	SetDrawScreen(GraphHandle);
	partile.DrawParticles(MyMap.X + 20 * 32, MyMap.Y + 25 * 32);
	SetDrawScreen(DX_SCREEN_BACK);
}