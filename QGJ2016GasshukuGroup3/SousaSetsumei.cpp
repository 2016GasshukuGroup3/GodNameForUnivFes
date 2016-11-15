#include "Scenes.h"
#include "Input.h"
#include "Asset.h"
#include <string>

static int SetsumeiImage[2] = { -1 };
static int StoryImages[10] = { -1 };
static int StoryBackImage = -1;
static bool SetsumeiInitialized = false;
static int FontHandle = -1;
static int Phase = 0;
// Flames < 0: フェードイン中, Flames > 0: フェードアウト中, Flames == 0: それ以外
static int Flames = 0;
static bool SpacePressed = false;

static enum InternalState {
	StoryPhase,
	SetsumeiPhase,
	SceneEnded
} InternalCurrentState;

InternalState StoryScene() {
	if (Flames < 0) {
		Flames--;

		if (Flames <= -64) {
			Flames = 0;
		}
	} else if (Flames > 0) {
		Flames++;

		if (Flames >= 64) {
			Flames = -1;
			Phase = 1;
			return InternalState::SetsumeiPhase;
		}
	} 
	
	if (Flames <= 0) {
		int PressedTime = getKey(KEY_INPUT_SPACE);

		if (PressedTime > 120) {
			SpacePressed = false;
			return InternalState::SceneEnded;
		} else if (PressedTime > 0) {
			SpacePressed = true;
		} else if (SpacePressed) {
			SpacePressed = false;

			++Phase;

			if (Phase >= 10) {
				Phase = 9;
				Flames = 1;
			}
		}
	}

	DrawGraph(0, 0, StoryBackImage, FALSE);
	DrawGraph(27, 394, StoryImages[Phase], FALSE);

	if (Flames != 0) {
		if (Flames > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Flames * 4);
		} else {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 + Flames * 4);
		}

		DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	return InternalState::StoryPhase;
}

InternalState SetsumeiScene() {
	if (Flames < 0) {
		Flames--;

		if (Flames <= -64) {
			Flames = 0;
		}
	}
	else if (Flames > 0) {
		Flames++;

		if (Flames >= 64) {
			Flames = 0;
			Phase = 1;
			return InternalState::SceneEnded;
		}
	} else {
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			if (Phase == 0) {
				Phase = 1;
			} else {
				Flames = 1;
			}
		}
	}

	if (Phase == 0) {
		DrawGraph(0, 0, SetsumeiImage[0], FALSE);
	} else {
		DrawGraph(0, 0, SetsumeiImage[1], FALSE);
	}

	//int Brightness = Flames % 256 <= 128 ? Flames % 256 : 128 - (Flames - 128) % 256;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness);
	//DrawStringToHandle(300 - 3, 425 - 3, "[Space] で次に進む...", GetColor(0, 0, 156), FontHandle);
	//DrawStringToHandle(300, 425, "[Space] で次に進む...", GetColor(96, 96, 255), FontHandle);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (Flames != 0) {
		if (Flames > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Flames * 4);
		}
		else {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 + Flames * 4);
		}

		DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	return InternalState::SetsumeiPhase;
}

STATE setsumei() {
	if (!SetsumeiInitialized) {
		Phase = 0;
		Flames = -1;
		InternalCurrentState = InternalState::StoryPhase;

		// 画像などがまだ読み込まれていない時に一度だけ初期化
		if (SetsumeiImage[0] == -1) {
			SetsumeiImage[0] = LoadGraph("Graphic/ストーリー.png");
			SetsumeiImage[1] = LoadGraph("Graphic/操作説明改良版.png");

			for (int i = 0; i < _countof(StoryImages); i++) {
				StoryImages[i] = LoadGraph((std::string("Graphic/あらすじ") + std::to_string(i + 1) + ".png").c_str());
			}

			StoryBackImage = LoadGraph("Graphic/あらすじのイメージ.png");

			FontHandle = CreateFontToHandle("ＭＳ 明朝", 32, 3, DX_FONTTYPE_ANTIALIASING);
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
			// 終了処理
			SetsumeiInitialized = false;
			// タイトルのBGMを停止
			StopSoundMem(GetHandle("Sound1"));
			return GAME;
	}

	return SETSUMEI;
}