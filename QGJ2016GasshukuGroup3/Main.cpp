#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include "Shader.h"
#include <crtdbg.h>
#include <cmath>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ChangeWindowMode(TRUE);
	SetMainWindowText("神の名は。 -God name.-");

	SetUseDirect3DVersion(DX_DIRECT3D_9EX);
	
	DxLib_Init();
	if (CheckHitKey(KEY_INPUT_F2)) {
		ChangeWindowMode(FALSE);
	}
	InitializeShaderResourse();
	SetDrawScreen(DX_SCREEN_BACK);
	STATE nextstate = TITLE;
	// メインループ
	while (true) {
		if (ProcessMessage() == -1 || ClearDrawScreen() == -1 || gpUpdatePad() != 0 || gpUpdateKey() != 0) {
			// return -1;
			break;
		}
		switch (nextstate)
		{
		case EXIT:
			break;
		case TITLE:
			nextstate = title();
			break;
		case GAME:
			nextstate = game();
			break;
		case BOSS:
			nextstate = boss();
			break;
		case RESULT:
			nextstate = result();
			break;
		case GAMEOVER:
			nextstate = gameover();
			break;
		case SETSUMEI:
			nextstate = setsumei();
			break;
		case RANKING:
			nextstate = ranking();
			break;
		case PROMOTION:
			nextstate = promotion();
			break;
		default:
			break;
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}