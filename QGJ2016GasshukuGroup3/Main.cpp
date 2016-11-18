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
	SetMainWindowText("�_�̖��́B -God name.-");

	SetUseDirect3DVersion(DX_DIRECT3D_9EX);
	
	DxLib_Init();
	
	// ���s���ɉ�ʂ��Â��Ȃ�����A�X���[�v���[�h�Ɉڍs�����肵�Ȃ��悤�ɂ���B
	SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS);
	
	// if (CheckHitKey(KEY_INPUT_F2)) {
	if (MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H", "�_�̖���", MB_YESNO | MB_ICONQUESTION) == IDYES) {
		ChangeWindowMode(FALSE);
	}
	InitializeShaderResourse();
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(640, 480, 32);
	STATE nextstate = TITLE;
	// ���C�����[�v
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