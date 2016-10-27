#include "Scenes.h"
#include "Input.h"
#include <string>

static int SetsumeiImage[2] = { -1 };
static int StoryImages[10] = { -1 };
static int StoryBackImage = -1;
static bool SetsumeiInitialized = false;
static int FontHandle = -1;
static int Phase = 0;
static int Flames = 0;

static enum InternalState {
	StoryPhase,
	SetsumeiPhase,
	SceneEnded
} InternalCurrentState;

InternalState StoryScene() {
	if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) {
		++Phase;

		if (Phase >= 10) {
			Phase = 1;
			return InternalState::SetsumeiPhase;
		}
	}

	DrawGraph(0, 0, StoryBackImage, FALSE);
	DrawGraph(27, 394, StoryImages[Phase], FALSE);

	return InternalState::StoryPhase;
}

InternalState SetsumeiScene() {
	Flames++;

	if (Phase == 0) {
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			Phase = 1;
		}

		DrawGraph(0, 0, SetsumeiImage[0], FALSE);
	} else {
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			return InternalState::SceneEnded;
		}

		DrawGraph(0, 0, SetsumeiImage[1], FALSE);
	}

	int Brightness = Flames % 256 <= 128 ? Flames % 256 : 128 - (Flames - 128) % 256;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness);
	DrawStringToHandle(300 - 3, 425 - 3, "[Space] �Ŏ��ɐi��...", GetColor(0, 0, 156), FontHandle);
	DrawStringToHandle(300, 425, "[Space] �Ŏ��ɐi��...", GetColor(96, 96, 255), FontHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return InternalState::SetsumeiPhase;
}

STATE setsumei() {
	if (!SetsumeiInitialized) {
		Phase = 0;
		Flames = 0;
		InternalCurrentState = InternalState::StoryPhase;

		if (SetsumeiImage[0] == -1) {
			SetsumeiImage[0] = LoadGraph("Graphic/�X�g�[���[.png");
			SetsumeiImage[1] = LoadGraph("Graphic/����������ǔ�.png");

			for (int i = 0; i < _countof(StoryImages); i++) {
				StoryImages[i] = LoadGraph((std::string("Graphic/���炷��") + std::to_string(i + 1) + ".png").c_str());
			}

			StoryBackImage = LoadGraph("Graphic/���炷���̃C���[�W.png");

			FontHandle = CreateFontToHandle("�l�r ����", 32, 3, DX_FONTTYPE_ANTIALIASING);
		} 

		SetsumeiInitialized = true;
	}
	
	switch (InternalCurrentState) {
		case StoryPhase:
			InternalCurrentState = StoryScene();
			break;
		case SetsumeiPhase:
			InternalCurrentState = SetsumeiScene();
			break;
		case SceneEnded:
			SetsumeiInitialized = false;
			return GAME;
	}

	return SETSUMEI;
}