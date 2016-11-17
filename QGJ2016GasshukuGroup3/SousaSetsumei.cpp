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
// Flames < 0: �t�F�[�h�C����, Flames > 0: �t�F�[�h�A�E�g��, Flames == 0: ����ȊO
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
	//DrawStringToHandle(300 - 3, 425 - 3, "[Space] �Ŏ��ɐi��...", GetColor(0, 0, 156), FontHandle);
	//DrawStringToHandle(300, 425, "[Space] �Ŏ��ɐi��...", GetColor(96, 96, 255), FontHandle);
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

		// �摜�Ȃǂ��܂��ǂݍ��܂�Ă��Ȃ����Ɉ�x����������
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
			// �I������
			SetsumeiInitialized = false;
			// �^�C�g����BGM���~
			StopSoundMem(GetHandle("Sound1"));
			return GAME;
	}

	return SETSUMEI;
}

bool PromotionIsInitialized = false;

STATE promotion() {
	if (!PromotionIsInitialized) {
		AddGraphicHandle("Promotion Movie", "Graphic/�Љ��.ogv");
		SeekMovieToGraph(GetHandle("Promotion Movie"), 0);
		PlayMovieToGraph(GetHandle("Promotion Movie"));
		StopSoundMem(GetHandle("Sound1"));

		PromotionIsInitialized = true;
	}

	if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
		PromotionIsInitialized = false;
		PauseMovieToGraph(GetHandle("Promotion Movie"));
		PlaySoundMem(GetHandle("KetteiSound"), DX_PLAYTYPE_BACK);
		PlaySoundMem(GetHandle("Sound1"), DX_PLAYTYPE_LOOP);
		return TITLE;
	}

	if (GetMovieStateToGraph(GetHandle("Promotion Movie")) != 1) {
		PromotionIsInitialized = false;
		PlaySoundMem(GetHandle("Sound1"), DX_PLAYTYPE_LOOP);
		return TITLE;
	}
	
	DrawGraph(0, 0, GetHandle("Promotion Movie"), FALSE);

	return PROMOTION;
}