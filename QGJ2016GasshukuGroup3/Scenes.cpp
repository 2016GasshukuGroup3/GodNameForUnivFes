#include "DxLib.h"
#include "Collider.h"
#include "MapEditor.h"
#include "Scenes.h"
#include "Lift.h"
// �O���t�B�b�N�n���h���Ȃǂ̊Ǘ�������֐����g����悤�ɂ��܂��B
#include "Asset.h"
#include <complex>
#include <cmath>
#include<algorithm>

using namespace std;

int Sound1, Sound2, Sound3;
// ���艹
int KetteiSound;


bool titleflag = false;
int titleHandle;
int TitleFlames;
int GameMode_EasyImage = -1, GameMode_EasySelectedImage, GameMode_HardImage, GameMode_HardSelectedImage;
int PressStartImage, CloudImage, SkyImage;

int FontHandle;
// ���݂�EasyMode/HardMode�I����
enum GameMode {
	// �Q�[����Փx�͑I������Ă��܂���B�܂�A�^�C�g����ʂ̃{�^���������Ă˂̏�Ԃ�\���܂��B
	GameMode_None,
	// �Q�[����Փx�Ƃ��āAEasy ���I�΂�Ă��܂��B
	GameMode_Easy,
	// �Q�[����Փx�Ƃ��āAHard ���I�΂�Ă��܂��B
	GameMode_Hard
} CurrentSelection;

STATE title() {
	if (!titleflag) {
		titleHandle = LoadGraph("Graphic/�^�C�g���L�������^�C�g��.png");
		CurrentSelection = GameMode_None;

		//���y�̂��߂̕ϐ��Ɠǂݍ���
		// Sound1 = LoadSoundMem("���y/���hQGJ_�^�C�g��.ogg");
		AddMusicHandle("Sound1", "���y/���hQGJ_�^�C�g��.ogg");
		Sound1 = GetHandle("Sound1");
		// ChangeVolumeSoundMem(216, Sound1);
		// Sound2 = LoadSoundMem("���y/���hQGJ_���C��.ogg");
		AddMusicHandle("Sound2", "���y/���hQGJ_���C��.ogg");
		Sound2 = GetHandle("Sound2");
		// ChangeVolumeSoundMem(216, Sound2);
		// Sound3 = LoadSoundMem("���y/���hQGJ_���U���g.ogg");
		AddMusicHandle("Sound3", "���y/���hQGJ_���U���g.ogg");
		Sound3 = GetHandle("Sound3");
		// ChangeVolumeSoundMem(216, Sound3);
		// KetteiSound = LoadSoundMem("���y/���hQGJ_SE_���艹.ogg");
		AddMusicHandle("KetteiSound", "���y/���hQGJ_SE_���艹.ogg");
		KetteiSound = GetHandle("KetteiSound");

		// ��x����������
		if (GameMode_EasyImage == -1) {
			SkyImage = LoadGraph("Graphic/�^�C�g����.png");
			CloudImage = LoadGraph("Graphic/�^�C�g���_.png");

			GameMode_EasyImage = LoadGraph("Graphic/�C�[�W�[���[�h.png");
			GameMode_EasySelectedImage = LoadGraph("Graphic/�C�[�W�[���[�h�I��.png");
			GameMode_HardImage = LoadGraph("Graphic/�n�[�h���[�h.png");
			GameMode_HardSelectedImage = LoadGraph("Graphic/�n�[�h���[�h�I��.png");

			PressStartImage = LoadGraph("Graphic/PRESS_START.png");
		}

		PlaySoundMem(Sound1, DX_PLAYTYPE_LOOP);

		// �쐬�����f�[�^�̎��ʔԍ���ϐ� FontHandle �ɕۑ�����
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		titleflag = true;
		TitleFlames = 0;
	}
	else {
		if (CurrentSelection == GameMode_None) {
		// �L�[�̓��͑҂�
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
				CurrentSelection = GameMode_Easy;
				PlaySoundMem(KetteiSound, DX_PLAYTYPE_BACK);
			}
		} else {
			// �L�[�̓��͑҂�
			if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
				// �쐬�����t�H���g�f�[�^���폜����
				DeleteFontToHandle(FontHandle);
					// StopSoundMem(Sound1);
				PlaySoundMem(KetteiSound, DX_PLAYTYPE_BACK);
				return SETSUMEI;
			}

			if (getKeyPress(KEY_INPUT_UP, PRESS_ONCE) || getKeyPress(KEY_INPUT_DOWN, PRESS_ONCE)) {
				if (CurrentSelection == GameMode_Easy) {
					CurrentSelection = GameMode_Hard;
				} else {
					CurrentSelection = GameMode_Easy;
				}
			}

			if (getKeyPress(KEY_INPUT_ESCAPE, PRESS_ONCE)) {
				CurrentSelection = GameMode_None;
			}
		}

		TitleFlames++;

		//�^�C�g���`��


		// �ǂ݂��񂾃O���t�B�b�N����ʍ���ɕ`��
		DrawGraph(0, 0, SkyImage, TRUE);
		DrawRotaGraph(320, -100, 1.0, TitleFlames / 180.0, CloudImage, TRUE);
		DrawGraph(0, 0, titleHandle, TRUE);

		if (CurrentSelection != GameMode_None) {
			if (CurrentSelection == GameMode_Easy) {
				DrawGraph(300, 260, GameMode_EasySelectedImage, TRUE);
				DrawGraph(300, 340, GameMode_HardImage, TRUE);
			} else {
				DrawGraph(300, 260, GameMode_EasyImage, TRUE);
				DrawGraph(300, 340, GameMode_HardSelectedImage, TRUE);
			}
		} else {
			DrawGraph(180, 400, PressStartImage, TRUE);
			// DrawStringToHandle(200, 400, "PRESS SPACE !!", GetColor(0, 255, 255), FontHandle);
		//ScreenFlip();//�`��̔��f
	}
	}

	return TITLE;
}

struct Player {
	int x, y, width, height, dx, dy, fly, deathcount1, deathcount2,deathcount3;
	int FloorDeltaX;
	int InvulnerableTime;

	// �����蔻��̏k����
	static const int CollisionOffset = 8;

	// ������\���܂��B
	enum Direction {
		// ������
		Direction_Left = 1,
		// �E����
		Direction_Right = 2,
	} FaceDirection;
	int AnimationFlame;
	int CollidedDirection;

	bool OnCollideFromSide(int& tileid, int, int);
	bool OnCollideFromBottom(int& tileid, int, int);
	bool OnCollideFromTop(int& tileid, int, int);

	bool OnCollideFromSide(Rect& tileid, int i, int j) {
		return OnCollideFromSide(tileid.pattern, i, j);
	}

	bool OnCollideFromBottom(Rect& tileid, int i, int j) {
		return OnCollideFromBottom(tileid.pattern, i, j);
	}

	bool OnCollideFromTop(Rect& tileid, int i, int j) {
		return OnCollideFromTop(tileid.pattern, i, j);
	}
};

Player player;

bool Player::OnCollideFromSide(int& tileid, int i, int j) {
	x = 0;

	if (tileid == 3 || tileid == 4) {
		return true;
	}

	for (int id : { 5, 6, 7, 8 }) {
		if (tileid == id) {
			if (Shape::Rectangle_t(i * MapTile::MapSize + Player::CollisionOffset, j * MapTile::MapSize + Player::CollisionOffset, MapTile::MapSize - Player::CollisionOffset * 2, MapTile::MapSize - Player::CollisionOffset * 2).
				IntersectWith(Rectangle_t{ x, y, width, height })) {

				// ���S
				deathcount2++;
			}
			else {
				return true;
			}
		}
	}

	// �Ԃ�l�� false �ɂ���ƁA�����蔻��𖳎����Ȃ��B
	return false;
}

bool Player::OnCollideFromBottom(int& tileid, int i, int j) {
	if (tileid == 3 || tileid == 4) {
		return true;
	}

	for (int id : { 5, 6, 7, 8 }) {
		if (tileid == id) {
			if (Shape::Rectangle_t(i * MapTile::MapSize + Player::CollisionOffset, j * MapTile::MapSize + Player::CollisionOffset, MapTile::MapSize - Player::CollisionOffset * 2, MapTile::MapSize - Player::CollisionOffset * 2).
				IntersectWith(Rectangle_t{ x, y, width, height })) {

				// ���S
				deathcount2++;
			}
			else {
				return true;
			}
		}
	}

	fly = 0;//0�̂Ƃ���ׂ�
	return false;
}

bool Player::OnCollideFromTop(int& tileid, int i, int j) {

	if (tileid == 4) {
		// �u���b�N�����̉�
		tileid = 0;
		//int *tileobjptr = &tileid;
		//*(tileobjptr + 1) = 2;
		//*(tileobjptr - 1) = 2;
		//*(tileobjptr - 15) = 2;
		//*(tileobjptr + 15) = 2;
	}
	else if (tileid == 3) {
		return true;
	}

	for (int id : { 5, 6, 7, 8 }) {
		if (tileid == id) {
			if (Shape::Rectangle_t(i * MapTile::MapSize + Player::CollisionOffset, j * MapTile::MapSize + Player::CollisionOffset, MapTile::MapSize - Player::CollisionOffset * 2, MapTile::MapSize - Player::CollisionOffset * 2).
				IntersectWith(Rectangle_t{ x, y, width, height })) {

				// ���S
				deathcount2++;
			}
			else {
				return true;
			}
		}
	}

	return false;
}

struct Tile {
	int x, y, dx, dy, width, height;
	bool flag, flag2;
	int dir;
};
const int TILE_MAX = 40;
Tile normalfloor[TILE_MAX];
Tile ball[TILE_MAX];
Tile bridge[TILE_MAX];
Tile drill[TILE_MAX];
Tile invis[TILE_MAX];
Lift Lifts[TILE_MAX];

int stagenum = 1;

static int normalfloorcount = 0;
static int ballcount = 0;
static int bcount = 0;
static int drillcount = 0;
static int inviscount = 0;
static int LiftCount = 0;

//1�͓G�Q�͎��@�A�����蔻��
bool Checkhitchery(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
	if (x1 < (x2 + width2)) {
		if ((x1 + width1) > x2) {
			if (y1 < (y2 + height2)) {
				if ((y1 + height1) >y2) {
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}

void moveBall(Tile* ball) {
	for (int i = 0; i < ballcount; ++i) {
		if ((player.x + player.width) >= ball[i].x) {
			ball[i].dy = 30;
			ball[i].flag = true;
		}
		ball[i].y += ball[i].dy;
	}
	for (int i = 0; i < ballcount; ++i) {
		if (Checkhitchery(ball[i].x, ball[i].y, ball[i].width, ball[i].height, player.x + Player::CollisionOffset, player.y + Player::CollisionOffset, player.width - Player::CollisionOffset * 2, player.height - Player::CollisionOffset)) {
			player.deathcount2++;
		}
	}
}
const int dx[] = { 0,1,0,-1 };
const int dy[] = { 1,0,-1,0 };
const int MapTilesWidth = 20;
const int MapTilesHeight = 15;
const int drillsuf = 9;
int MapTiles[MapTilesWidth][MapTilesHeight];

bool IsDrillHit(Player p, Tile t) {
	int dir = t.dir;
	if (dir == 0 || dir == 2) {
		if (t.x - 1 <= p.x + p.width) {
			return true;
		}
	}
	else if ((dir == 1) && p.y <= t.y + t.height && t.x - p.x >= 0 && t.x - p.x < 32 * 3) {
		return true;
	}
	else if (dir == 3 && p.y <= t.y + t.height && p.x - t.x >= 0) {
		return true;
	}
	return false;
}

void drillAttack(Tile* drill) {
	for (int i = 0; i < drillcount; ++i) {
		int dir = drill[i].dir;
		if (drill[i].flag &&  IsDrillHit(player, drill[i])) {
			MapTiles[drill[i].x / 32][drill[i].y / 32] = -1;
			drill[i].dx = -10 * dx[dir];
			drill[i].dy = -10 * dy[dir];
			drill[i].flag = false;
		}
	}
	for (int i = 0; i < drillcount; ++i) {
		drill[i].x += drill[i].dx;
		drill[i].y += drill[i].dy;
	}
	for (int i = 0; i < drillcount; ++i) {
		if (Checkhitchery(drill[i].x, drill[i].y, drill[i].width, drill[i].height, player.x + Player::CollisionOffset, player.y + Player::CollisionOffset, player.width - Player::CollisionOffset * 2, player.height - Player::CollisionOffset)) {
			player.deathcount2++;
		}
	}
}

void invisManifestation(Tile* t) {
	for (int i = 0; i < inviscount; ++i) {
		if (t[i].x <= player.x && player.x + player.width <= t[i].x + t[i].width) {
			if (player.y - t[i].y >= 0 && player.y - t[i].y <= 68) {
				t[i].flag = true;
			}
		}
	}
}

void moveBridge(Tile *b) {
	for (int i = 0; i < bcount; ++i) {
		if (b[i].flag2 && (player.x + player.width / 2) >= b[i].x && abs(player.y - b[i].y) < 100 && b[i].flag) {
			b[i].dy = 50;
			b[i].flag = false;
		}
	}
}

bool gameflag = false;
int BackImageHandle[5] = { -1 }, jimen, toge[4], hasi, ballHandle, InvisibleBlockImage;
int NumberImages[10], StageImage, TimeImage, MinImage, SecImage, DeathCountImage, ExtendedTimeImage;
int JumpSound, KilledSound;
int timer;
int PlayerImageHandles[3];
CMap MyMap;
MapViewer mv;
vector<vector<int>> tmp(MapTilesHeight, vector<int>(MapTilesWidth, -1));
Particle particle;

//����������֐�
void Initialization(int map, MapViewer &mv) {
	player.x = 0;
	player.y = 200;
	player.width = 32;
	player.height = 64;
	player.dx = 0;
	player.dy = 0;
	//player.deathcount1 = 0;

	player.fly = 0;
	ballcount = 0;
	mv.SetData(map);

	for (auto& Item : Lifts) {
		Item.Initialize();
		Item.Width = 6;
	}
}


void DrawNumber(int x, int y, int Number, int Images[] = NumberImages) {
	int TempNum = Number;
	int Counter = 0;
	bool Appeared = false;

	// �ꕶ���̕��ƍ���
	int TextWidth, TextHeight;
	GetGraphSize(Images[0], &TextWidth, &TextHeight);

	// �傫���ʂ̒l�i�疜�̈ʁj���珇�Ԃɕ`�悵�Ă���
	for (int i = 10000000; i >= 1; i /= 10) {
		int RankNum = TempNum / i;

		if (!(RankNum == 0 && !Appeared) || i == 1) {
			Appeared = true;
			DrawGraph(x + (TextWidth + 4) * Counter, y, Images[RankNum], TRUE);
		}

		Counter++;
		TempNum -= i * RankNum;
	}
}

STATE game() {
	if (!gameflag) {
		// �^�C���}�b�v�Ƃ��Ďg���Q�����z��

		// �^�C���}�b�v�� -1 (�����Ȃ�) �Ŗ��߂�
		for (int i = 0; i < MapTilesWidth; i++) {
			for (int j = 0; j < MapTilesHeight; j++) {
				MapTiles[i][j] = -1;

				// ��ԉ��͏��Ƃ��� 0 �����Ă���
				if (j == MapTilesHeight - 1) { //&& (i < 5 || i > 7)) {
					MapTiles[i][j] = 0;
				}
			}
		}

		//�^�C�}�[
		timer = 0;
		//�X�e�[�W�̏�����
		stagenum = 1;

		if (BackImageHandle[0] == -1) {
			// �w�i�̓ǂݍ���
			BackImageHandle[0] = LoadGraph("Graphic/stage1.png");
			BackImageHandle[1] = LoadGraph("Graphic/stage2.png");
			BackImageHandle[2] = LoadGraph("Graphic/stage3.png");
			BackImageHandle[3] = LoadGraph("Graphic/stage4.png");
			BackImageHandle[4] = LoadGraph("Graphic/stage5.png");

			// �v���C���[�̉摜�̓ǂݍ���
			LoadDivGraph("Graphic/Character.png", 3, 3, 1, 32, 64, PlayerImageHandles);

			LoadDivGraph("Graphic/num.png", 10, 10, 1, 16, 32, NumberImages);
			StageImage = LoadGraph("Graphic/STAGE.png");
			TimeImage = LoadGraph("Graphic/TIME.png");
			MinImage = LoadGraph("Graphic/MIN.png");
			SecImage = LoadGraph("Graphic/SEC.png");
			DeathCountImage = LoadGraph("Graphic/DEATH_COUNT.png");
			ExtendedTimeImage = LoadGraph("Graphic/������.png");

		jimen = LoadGraph("Graphic/Jimen.png");
		hasi = LoadGraph("Graphic/Hasi.png");
		InvisibleBlockImage = LoadGraph("Graphic/Kakusi_block.png");
		for (int i = 0; i < 4; ++i) {
			toge[i] = LoadGraph((string("Graphic/toge") + to_string(i) + ".png").c_str());
		}
		ballHandle = LoadGraph("Graphic/ball.png");
		JumpSound = LoadSoundMem("���y/���hQGJ_SE_�W�����v.ogg");
		KilledSound = LoadSoundMem("���y/���hQGJ_SE_���S.ogg");
		}

		for (auto& item : Lifts) {
			item.Reset();
		}

		Lifts[0].MyPattern = Lift::Side;
		Lifts[0].X = 0;
		Lifts[0].Y = 32 * 12;
		Lifts[1].MyPattern = Lift::Side;
		Lifts[1].X = 32 * 6;
		Lifts[1].Y = 32 * 6;
		Lifts[2].MyPattern = Lift::Rotation;
		Lifts[2].X = 32 * 10;
		Lifts[2].Y = 32 * 4;
		Lifts[3].MyPattern = Lift::UpAndDown;
		Lifts[3].X = 32;
		Lifts[3].Y = 32 * 6;
		LiftCount = 0;

		// player ��������
		// mv = MapViewer(1);
		if (CurrentSelection == GameMode_Easy) {
			mv = MapViewer("Data/Map/SaveData", 1);
		} else {
			mv = MapViewer("Data/HardMap/SaveData", 1);
		}
		Initialization(stagenum, mv);

		mv.SetTileKind(tmp);
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				MapTiles[j][i] = tmp[i][j];
			}
		}

		// �ʓ|�Ȃ̂ŁAtmp ���ė��p�i�{���́A��̕ϐ����Q�ȏ�̈Ⴄ�p�r�Ɏg���Ă͂����Ȃ��B�j
		// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l���擾����B
		mv.SetTileAttribute(tmp);

		//Ball�̃Z�b�g
		normalfloorcount = 0;
		ballcount = 0;
		bcount = 0;
		drillcount = 0;
		inviscount = 0;
		//Bridge�̃Z�b�g�i��������j
		for (int i = 0; i < MapTilesHeight; ++i) {
			for (int j = 0; j < MapTilesWidth; ++j) {
				// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l�� 0�Ȃ�΁A�ʏ�̏��������s���B
				
				if (MapTiles[j][i] == 0) {
					normalfloor[normalfloorcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32 };
					++normalfloorcount;
				} else if (MapTiles[j][i] == 1) {
					bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true };
					++bcount;
				}
				else if (MapTiles[j][i] == 2) {
					bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,false };
					++bcount;
				}
				else if (MapTiles[j][i] == 3) {
					ball[ballcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false };
					++ballcount;
				}
				else if (MapTiles[j][i] == 4) {
					invis[inviscount] = Tile{ j * 32,i * 32,0,0,32,36,false };
					++inviscount;
				}
				else if (drillsuf - 4 <= MapTiles[j][i] && MapTiles[j][i] < drillsuf) {
					drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false,true, MapTiles[j][i] - (drillsuf - 4) };
					++drillcount;
				}
				else if (MapTiles[j][i] >= drillsuf) {//�����g�Q
					drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
					++drillcount;
				}
				
				// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l�� 1�Ȃ�΁AMapTiles �ɂ�邠���蔻��𖳌����B
				if (tmp[i][j] == 1) {
					MapTiles[j][i] = -1;
				}
			}
		}
		gameflag = true;
	}
	else {
		// �����I���R�}���h
		if (getResetRequestStatus()) {
			player.deathcount1 = 0;
			player.deathcount2 = 0;
			StopSoundMem(Sound2);
			PlaySoundMem(Sound1, DX_PLAYTYPE_LOOP);
			gameflag = false;
			return TITLE;
		}

		// ���C�����[�v
		mv.Update();
		particle.UpdateParticles();


		//���y�̍Đ�
		if (CheckSoundMem(Sound2) == 0) {
			PlaySoundMem(Sound2, DX_PLAYTYPE_LOOP, TRUE);
		}

		// ���͂ɉ����āA�v���C���[�̃X�s�[�h��ς���
		if (getKeyPress(KEY_INPUT_LEFT, PRESS)) {
			player.FaceDirection = Player::Direction::Direction_Left;
			player.AnimationFlame++;
			player.dx = -2 + player.FloorDeltaX;
		}
		else if (getKeyPress(KEY_INPUT_RIGHT, PRESS)) {
			player.FaceDirection = Player::Direction::Direction_Right;
			player.AnimationFlame++;
			player.dx = 2 + player.FloorDeltaX;
		}
		else {
			player.AnimationFlame = 0;
			player.dx = player.FloorDeltaX;
		}

		if ((getKeyPress(KEY_INPUT_SPACE,PRESS_ONCE) || CheckHitKey(KEY_INPUT_UP)) && player.fly == 0) { // && player.dy == 0) {
			PlaySoundMem(JumpSound, DX_PLAYTYPE_BACK);
			player.dy = -20;
			player.fly = 1;
		}

		// �d�͉����x��������B
		player.dy += 1;
		if (player.dy > 10) {
			player.dy = 10;
		}

		//��������������
		if (player.y > 480) {
			player.x = 0;
			player.y = 382;
			player.fly = 0;
			player.deathcount2++;
		}
		//Tile�𓮂���
		//if () {//map�ɂ���ĕύX������

		moveBall(ball);
		moveBridge(bridge);
		drillAttack(drill);
		invisManifestation(invis);

		//}

		for (int i = 0; i < LiftCount; i++) {
			Lifts[i].Update();
		}

		// �����蔻����s���B
		player.FloorDeltaX = 0;
		player.CollidedDirection = Direction::None;

		int DefX = player.x, DefY = player.y;
		int DefDeltaX = player.dx, DefDeltaY = player.dy;
		CollisionCheck(player, MapTiles, 32, -1);
		int NewX = player.x, NewY = player.y;
		int TempCollideDirection = Direction::None;
		TempCollideDirection = player.CollidedDirection;

		for (int i = 0; i < LiftCount; i++) {
			player.CollidedDirection = Direction::None;
			player.x = DefX; player.y = DefY;
			CollisionCheck(player, Lifts[i].GetCollider(), -1);

			if (DefDeltaX - Lifts[i].GetCollider().DeltaX > 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Right) && player.x <= NewX) {
					NewX = player.x;
				}
			}
			else if (DefDeltaX - Lifts[i].GetCollider().DeltaX < 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Left) && player.x >= NewX) {
					NewX = player.x;
				}
			}

			if (DefDeltaY - Lifts[i].GetCollider().DeltaY > 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Down) && player.y <= NewY) {
					NewY = player.y;
				}
			}
			else if (DefDeltaY - Lifts[i].GetCollider().DeltaY < 0) {
				if (static_cast<bool>(player.CollidedDirection & Direction::Up) && player.y >= NewY) {
					NewY = player.y;
				}
			}

			TempCollideDirection |= player.CollidedDirection;
		}

		player.x = NewX;
		player.y = NewY;
		//�{�X��gameover�̂��߂̐ݒ�
		player.deathcount3 = player.deathcount2 + 5;
		// ���܂蔻��
		if ((TempCollideDirection & Direction::LeftAndRight) == Direction::LeftAndRight || (TempCollideDirection & Direction::UpAndDown) == Direction::UpAndDown) {
			player.deathcount2++;
		}

		// clsDx();
		// printfDx("Player\nCollideDirection : %d\nx : %d\ndx : %d\ndy : %d", player.CollidedDirection, player.x, player.dx, player.dy);

		//���񂾂�deathcount�𑝂₵�d�|�������ɖ߂�Bplayer�͒��Ԃɔ��(���S����)
		if (player.deathcount1 < player.deathcount2) {
			PlaySoundMem(KilledSound, DX_PLAYTYPE_BACK);
			//�O���o������Q�[���I�[�o�[
			if (timer >= 180 * 60) {
				titleflag = false;
				gameflag = false;
				if (CheckSoundMem(Sound2) == 1) {
					StopSoundMem(Sound2);
				}
				return GAMEOVER;
			}
			player.deathcount1 = player.deathcount2;
			for (int i = 0; i < 30; ++i) {
				auto p = (new Particle(player.x, player.y));
				particle.Factory(p);
			}
			Initialization(stagenum, mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			// �ʓ|�Ȃ̂ŁAtmp ���ė��p�i�{���́A��̕ϐ����Q�ȏ�̈Ⴄ�p�r�Ɏg���Ă͂����Ȃ��B�j
			// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l���擾����B
			mv.SetTileAttribute(tmp);

			normalfloorcount = 0;
			ballcount = 0;
			bcount = 0;
			drillcount = 0;
			inviscount = 0;
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l�� 0�Ȃ�΁A�ʏ�̏��������s���B

					if (MapTiles[j][i] == 0) {
						normalfloor[normalfloorcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32 };
						++normalfloorcount;
					}
					else if (MapTiles[j][i] == 1) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true };
						++bcount;
					}
					else if (MapTiles[j][i] == 2) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,false };
						++bcount;
					}
					else if (MapTiles[j][i] == 3) {
						ball[ballcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false };
						++ballcount;
					}
					else if (MapTiles[j][i] == 4) {
						invis[inviscount] = Tile{ j * 32,i * 32,0,0,32,36,false };
						++inviscount;
					}
					else if (drillsuf - 4 <= MapTiles[j][i] && MapTiles[j][i] < drillsuf) {
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false,true, MapTiles[j][i] - (drillsuf - 4) };
						++drillcount;
					}
					else if (MapTiles[j][i] >= drillsuf) {//�����g�Q
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
						++drillcount;
					}

					// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l�� 1�Ȃ�΁AMapTiles �ɂ�邠���蔻��𖳌����B
					if (tmp[i][j] == 1) {
						MapTiles[j][i] = -1;
					}
				}
			}
		}


		// �w�i�̕`��
		DrawGraph((timer / 10) % 640, 0, BackImageHandle[stagenum - 1], FALSE);
		DrawGraph((timer / 10) % 640 - 640, 0, BackImageHandle[stagenum - 1], FALSE);

		if (player.FaceDirection == Player::Direction::Direction_Left) {
			DrawTurnGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
		}
		else {
			DrawGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
		}
		
		//for (int i = 0; i < MapTilesHeight; ++i) {
		//	for (int j = 0; j < MapTilesWidth; ++j) {
		//		if (MapTiles[j][i] == 0) {
		//			DrawGraph(j * 32, i * 32, jimen, TRUE);
		//		}
		//	}
		//}

		// ���ʂ̏���`��
		for (int i = 0; i < normalfloorcount; ++i) {
			DrawGraph(normalfloor[i].x, normalfloor[i].y, jimen, TRUE);
		}

		//�����Ă��鋅
		for (int i = 0; i < ballcount; ++i) {
			//if (ball[i].flag)
			DrawGraph(ball[i].x, ball[i].y, ballHandle, TRUE);
		}

		//�����鋴
		for (int i = 0; i < bcount; ++i) {
			if (bridge[i].flag) {
				DrawGraph(bridge[i].x, bridge[i].y, hasi, TRUE);
			}
			else if (bridge[i].flag2) {
				int X = bridge[i].x / 32, Y = bridge[i].y / 32;
				MapTiles[X][Y] = -1;
				bridge[i].flag2 = false;
			}
		}
		for (int i = 0; i < drillcount; ++i) {
			DrawGraph(drill[i].x, drill[i].y, toge[drill[i].dir], TRUE);
		}
		for (int i = 0; i < inviscount; ++i) {
			if (invis[i].flag)
				DrawGraph(invis[i].x, invis[i].y, InvisibleBlockImage, TRUE);
		}

		for (int index = 0; index < LiftCount; index++) {
			Lifts[index].ApplyParticles(particle);
			Lifts[index].Draw();
			//for (int i = 0; i < Lifts[index].GetCollider().Cols(); i++) {
			//	for (int j = 0; j < Lifts[index].GetCollider().Rows(); j++) {
			//		if (Lifts[index].GetCollider()[i][j] != -1 && Lifts[index].GetCollider()[i][j] != 1) {
			//			DrawGraph(Lifts[index].GetCollider().X + i * 32, Lifts[index].GetCollider().Y + j * 32, jimen, TRUE);
			//			// DrawBox(MyMap.X + i * 32, MyMap.Y + j * 32, MyMap.X + i * 32 + 32, MyMap.Y + j * 32 + 32, GetColor(0, 216, 0), TRUE);
			//		}
			//	}
			//}
		}

		// ���F�̒l���擾
		unsigned Cr;
		Cr = GetColor(0, 0, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(355, 0, 640, 100, Cr, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//���S�񐔁A�X�e�[�W�A�c�莞�Ԃ̕\��
		// DrawFormatString(500, 0, Cr, "Death Count %d", player.deathcount1);
		DrawGraph(360, 0, DeathCountImage, TRUE);
		DrawNumber(470, 0, player.deathcount1);
		// DrawFormatString(500, 20, Cr, "Stage %d", stagenum);
		DrawGraph(365, 30, StageImage, TRUE);
		DrawNumber(470, 30, stagenum);

		// �������ԓ��Ȃ�A���ʂɎc�莞�Ԃ�`��
		if (timer / 60 >= 0 && timer / 60 <= 180) {
			// DrawFormatString(500, 40, Cr, "time %dmin %02dsec", (180 - timer / 60) / 60, 60 - (timer / 60) % 60 == 60 ? 0 : 60 - (timer / 60) % 60);
			DrawGraph(450, 60, MinImage, TRUE);
			DrawNumber(280, 60, (180 - timer / 60) / 60);
			DrawGraph(570, 60, SecImage, TRUE);
			DrawNumber(410, 60, 60 - (timer / 60) % 60 == 60 ? 0 : 60 - (timer / 60) % 60);
		} // �������Ԃ��߂�����i���������ԁj�A�������̕�����`��
		else {
			DrawGraph(450, 60, ExtendedTimeImage, TRUE);
		}

		if (timer / 60 > 170) {
			if(timer/60<181)
			DrawRotaGraph(320, 240, 5, 0, NumberImages[180-timer/60], TRUE, FALSE);
		}
		if (player.x >= 608) {
			if (stagenum >= 5) {
				gameflag = false;
				if (CheckSoundMem(Sound2) == 1) {
					StopSoundMem(Sound2);
				}
				return BOSS;
			}

			//�}�b�v�ړ�
			player.x = 0;
			++stagenum;
			Initialization(stagenum, mv);
			mv.SetTileKind(tmp);
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					MapTiles[j][i] = tmp[i][j];
				}
			}
			// �ʓ|�Ȃ̂ŁAtmp ���ė��p�i�{���́A��̕ϐ����Q�ȏ�̈Ⴄ�p�r�Ɏg���Ă͂����Ȃ��B�j
			// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l���擾����B
			mv.SetTileAttribute(tmp);
			normalfloorcount = 0;
			ballcount = 0;
			bcount = 0;
			drillcount = 0;
			for (int i = 0; i < MapTilesHeight; ++i) {
				for (int j = 0; j < MapTilesWidth; ++j) {
					// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l�� 0�Ȃ�΁A�ʏ�̏��������s���B

					if (MapTiles[j][i] == 0) {
						normalfloor[normalfloorcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32 };
						++normalfloorcount;
					}
					else if (MapTiles[j][i] == 1) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true };
						++bcount;
					}
					else if (MapTiles[j][i] == 2) {
						bridge[bcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,false };
						++bcount;
					}
					else if (MapTiles[j][i] == 3) {
						ball[ballcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false };
						++ballcount;
					}
					else if (MapTiles[j][i] == 4) {
						invis[inviscount] = Tile{ j * 32,i * 32,0,0,32,36,false };
						++inviscount;
					}
					else if (drillsuf - 4 <= MapTiles[j][i] && MapTiles[j][i] < drillsuf) {
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,false,true, MapTiles[j][i] - (drillsuf - 4) };
						++drillcount;
					}
					else if (MapTiles[j][i] >= drillsuf) {//�����g�Q
						drill[drillcount] = Tile{ j * 32, i * 32, 0, 0, 32, 32,true,true, MapTiles[j][i] - drillsuf };
						++drillcount;
					}

					// �}�b�v�G�f�B�^�ł̃^�C�����Ƃ̑����l�� 1�Ȃ�΁AMapTiles �ɂ�邠���蔻��𖳌����B
					if (tmp[i][j] == 1) {
						MapTiles[j][i] = -1;
					}
				}
			}
		}
		++timer;
		//�O���o������Q�[���I�[�o�[
		//if (timer >= 180 * 60) {
			//titleflag = false;
			//gameflag = false;
			//if (CheckSoundMem(Sound2) == 1) {
			//	StopSoundMem(Sound2);
			//}
			//return GAMEOVER;
		//}
		mv.Draw();
		particle.DrawParticles();
	}
	return GAME;
}

bool bossflag = false;

Boss::Boss() {
	// Init();
}

void drillAttack2(Tile* drill) {
	for (int i = 0; i < drillcount; ++i) {
		int dir = drill[i].dir;
		if (drill[i].flag) {
			// MapTiles[drill[i].x / 32][drill[i].y / 32] = -1;
			drill[i].dx = -5 * dx[dir];
			drill[i].dy = -5 * dy[dir];
			drill[i].flag = false;
		}
	}
	for (int i = 0; i < drillcount; ++i) {
		drill[i].x += drill[i].dx;
		drill[i].y += drill[i].dy;
	}

	if (player.InvulnerableTime < 0) {
		for (int i = 0; i < drillcount; ++i) {
			if (Checkhitchery(drill[i].x, drill[i].y, drill[i].width, drill[i].height, player.x + Player::CollisionOffset, player.y + Player::CollisionOffset, player.width - Player::CollisionOffset * 2, player.height - Player::CollisionOffset)) {
				player.deathcount2++;
			}
		}
	}

}
void Boss::Init() {
	body = LoadGraph("Graphic/God.png");
	arm = LoadGraph("Graphic/GodArm.png");
	bgm = LoadSoundMem("���y/���hQGJ_�{�X��.ogg");
	AddGraphicHandle("�w�i2","Graphic/stage -boss-.png");
	ax = -40, ay = -100;
	gameover = false;
	time = 0;
	maxhp = hp = 6;
	player.x = 60, player.y = 100;

	for (auto& Item : Lifts) {
		Item.Width = 1;
		Item.IsEnabled = false;
		Item.Initialize();
	}

	//�g�Q
	tile = vector<vector<Rect>>(W, vector<Rect>(H));
	for (int i = 0; i < TILE_MAX; ++i) {
		drill[i] = {};
	}
	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < H; ++j) {
			MapTiles[i][j] = -1;
		}
	}
	for (int j = 0; j < H; ++j) {
		int i = 0;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[0][j].SetHandle(toge[3]);
		tile[0][j].kind = 3;
		tile[0][j].pattern = 0;
		MapTiles[0][j] = 8;
	}
	for (int j = 0; j < H; ++j) {
		int i = W - 1;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(toge[1]);
		tile[i][j].kind = 5;
		tile[i][j].pattern = 0;
		MapTiles[i][j] = 6;
	}
	for (int i = 0; i < W; ++i) {
		int j = 0;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(toge[2]);
		tile[i][j].kind = 5;
		tile[i][j].pattern = 0;
		MapTiles[i][j] = 7;
	}
	for (int i = 0; i < W; ++i) {
		int j = H - 1;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(toge[0]);
		tile[i][j].kind = 5;
		tile[i][j].pattern = 0;
		MapTiles[i][j] = 5;
	}
	//����
	for (int i : {1, 3, 5, 13, 15, 17}) {
		int j = 9;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(1, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {7, 9, 11}) {
		int j = 10;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(1, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {2, 4, 14, 16}) {
		int j = 9;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(2, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {6, 8, 10, 12}) {
		int j = 10;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(2, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	for (int i : {3, 15}) {
		int j = 6;
		tile[i][j].PosSet(Pos(i * 32, j * 32));
		tile[i][j].SetHandle(jimen);
		tile[i][j].SetPattern(0, 0);
		tile[i][j].kind = 0;
		MapTiles[i][j] = 0;
	}
	int x = 9, y = 7;
	tile[x][y].PosSet(Pos(x * 32, y * 32));
	tile[x][y].SetHandle(jimen);
	tile[x][y].SetPattern(0, 0);
	tile[x][y].kind = 0;
	MapTiles[x][y] = 0;

	// 覐΂̃G�t�F�N�g�̏�����
	for (auto& Asteroid : Asteroids) {
		InitializeAsteroidEffect(&Asteroid);
	}
	// ����́A���̂悤�ɏ����̂Ɠ����B
	// for (int i = 0; i < MaxAsteroidNum; i++) {
	//		InitializeAsteroidEffect(&Asteroids[i]);
	// }
}

int ThrowSound = -1;

void UpdateLift() {
	// ���t�g�̍X�V
	for (int i = 0; i < TILE_MAX; i++) {
		//if (Lifts[i].IsEnabled) {
		Lifts[i].Update();

		if (Lifts[i].Y > MapTile::MapSize * 15) {
			Lifts[i].IsEnabled = false;
			Lifts[i].MyPattern = Lift::DoNotMove;
		}
		//}
	}
}

void SetLift(int x, int y) {
	for (auto& Item : Lifts) {
		if (!Item.IsEnabled) {
			Item.X = x * MapTile::MapSize;
			Item.Y = y * MapTile::MapSize;
			Item.MyPattern = Lift::Fall;
			Item.IsEnabled = true;
			break;
		}
	}
}

/// <summary>覐΂̃G�t�F�N�g��o�ꂳ���܂��B</summary>
/// <param name="Effects">覐΂̃G�t�F�N�g�̔z��</param>
/// <param name="EffectsNum">覐΂̃G�t�F�N�g�̔z��̗v�f��</param>
/// <param name="SpawnNum">�o�ꂳ����覐΂̃G�t�F�N�g�̐��B�����w�肵�Ȃ���΁A5���w�肳��܂��B</param>
/// <remarks>
/// 覐΂̓o��ʒu�͊֐������Ń����_���Ɍ��߂��܂��B
/// �g�p��:
/// <code>
/// const int AsteroidEffectNum = 20;
/// AsteroidEffect Effects[AsteroidEffectNum];
///
/// SpawnAsteroidEffect(Effects, AsteroidEffectNum);
/// </code>
/// </remarks>
void SpawnAsteroidEffect(AsteroidEffect Effects[], int EffectNum, int SpawnNum = 5) {
	// �G�t�F�N�g�̔z�������m�F���āA�g���Ă��Ȃ����̂�T���B
	for (int i = 0, SpawnCounter = 0; i < EffectNum && SpawnCounter < SpawnNum; i++) {
		if (!Effects[i].IsEnabled) {
			// �g���ĂȂ��G�t�F�N�g�ɑ΂��ēK���ɏ���������B
			Effects[i].x = 50 + GetRand(64) * 10;
			Effects[i].y = -100 - GetRand(10) * 10;
			Effects[i].dx = -1;
			Effects[i].dy = 2;
			Effects[i].IsEnabled = true;
			SpawnCounter++;
		}
	}
}

/// <summary>�o�ꂵ�Ă���覐΂̃G�t�F�N�g���X�V���܂��B</summary>
/// <param name="Effects">覐΂̃G�t�F�N�g�̔z��</param>
/// <param name="EffectsNum">覐΂̃G�t�F�N�g�̔z��̗v�f��</param>
/// <remarks>
/// �g�p��:
/// <code>
/// const int AsteroidEffectNum = 20;
/// AsteroidEffect Effects[AsteroidEffectNum];
///
/// UpdateAsteroidEffect(Effects, AsteroidEffectNum);
/// </code>
/// </remarks>
void UpdateAsteroidEffect(AsteroidEffect Effects[], int EffectNum) {
	for (int i = 0; i < EffectNum; i++) {
		if (Effects[i].IsEnabled) {
			// �G�t�F�N�g�̕\���ʒu���X�V
			Effects[i].x += Effects[i].dx;
			Effects[i].y += Effects[i].dy;

			// ��ʊO�ɍs�����G�t�F�N�g�𖳌�������B
			if (Effects[i].y > 480 + 50) {
				Effects[i].IsEnabled = false;
			}
		}
	}
}

/// <summary>�o�ꂵ�Ă���覐΂̃G�t�F�N�g��`�悵�܂��B</summary>
/// <param name="Effects">覐΂̃G�t�F�N�g�̔z��</param>
/// <param name="EffectsNum">覐΂̃G�t�F�N�g�̔z��̗v�f��</param>
/// <remarks>
/// �g�p��:
/// <code>
/// const int AsteroidEffectNum = 20;
/// AsteroidEffect Effects[AsteroidEffectNum];
///
/// DrawAsteroidEffect(Effects, AsteroidEffectNum);
/// </code>
/// </remarks>
void DrawAsteroidEffect(AsteroidEffect Effects[], int EffectNum) {
	for (int i = 0; i < EffectNum; i++) {
		if (Effects[i].IsEnabled) {
			// �G�t�F�N�g��`��
			DrawAsteroidEffect(&Effects[i]);
		}
	}
}

void Boss::Update() {
	++time;
	// ���Ԍo�߂ɂ���ď��̔z�u��ς���
	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < H; ++j) {
			int PreviousPattern = tile[i][j].pattern;

			tile[i][j].Update();
			if (tile[i][j].pattern == 2) {
				MapTiles[i][j] = -1;

				if (PreviousPattern == 1) {
					SetLift(i, j);
				}
			}
			if (tile[i][j].pattern == 1) {
				MapTiles[i][j] = 0;
			}
			if (MapTiles[i][j] == 5) {
				if (Checkhitchery(player.x, player.y, 32, 32, i * 32, j * 32, 32, 32)) {
					++player.deathcount1;
					break;
				}
			}
		}
	}
	// �����Ƃ��̍X�V
	drillAttack2(drill);

	if (player.InvulnerableTime >= 0) {
		--player.InvulnerableTime;
	}

	// ���͂ɉ����āA�v���C���[�̃X�s�[�h��ς���
	if (getKeyPress(KEY_INPUT_LEFT, PRESS)) {
		player.FaceDirection = Player::Direction::Direction_Left;
		player.AnimationFlame++;
		player.dx = -2 + player.FloorDeltaX;
	}
	else if (getKeyPress(KEY_INPUT_RIGHT, PRESS)) {
		player.FaceDirection = Player::Direction::Direction_Right;
		player.AnimationFlame++;
		player.dx = 2 + player.FloorDeltaX;
	}
	else {
		player.AnimationFlame = 0;
		player.dx = player.FloorDeltaX;
	}
	// �W�����v�̏���
	if (getKeyPress(KEY_INPUT_SPACE,PRESS_ONCE) && player.fly == 0) {
		player.dy = -20;
		player.fly = 1;
	}

	// �d�͉����x��������B
	player.dy += 1;
	if (player.dy > 10) {
		player.dy = 10;
	}

	UpdateLift();

	// �����蔻����s���B
	player.FloorDeltaX = 0;
	player.CollidedDirection = Direction::None;

	int DefX = player.x, DefY = player.y;
	int DefDeltaX = player.dx, DefDeltaY = player.dy;
	CollisionCheck(player, MapTiles, 32, -1);
	// CollisionCheck(player, tile, [](Rect& rect) { return rect.pattern != 1; });
	int NewX = player.x, NewY = player.y;
	int TempCollideDirection = Direction::None;
	TempCollideDirection = player.CollidedDirection;

	for (int i = 0; i < TILE_MAX; i++) {
		if (!Lifts[i].IsEnabled) {
			continue;
		}

		player.CollidedDirection = Direction::None;
		player.x = DefX; player.y = DefY;
		CollisionCheck(player, Lifts[i].GetCollider(), -1);

		if (DefDeltaX - Lifts[i].GetCollider().DeltaX > 0) {
			if (static_cast<bool>(player.CollidedDirection & Direction::Right) && player.x <= NewX) {
				NewX = player.x;
			}
		}
		else if (DefDeltaX - Lifts[i].GetCollider().DeltaX < 0) {
			if (static_cast<bool>(player.CollidedDirection & Direction::Left) && player.x >= NewX) {
				NewX = player.x;
			}
		}

		if (DefDeltaY - Lifts[i].GetCollider().DeltaY > 0) {
			if (static_cast<bool>(player.CollidedDirection & Direction::Down) && player.y <= NewY) {
				NewY = player.y;
			}
		}
		else if (DefDeltaY - Lifts[i].GetCollider().DeltaY < 0) {
			if (static_cast<bool>(player.CollidedDirection & Direction::Up) && player.y >= NewY) {
				NewY = player.y;
			}
		}

		TempCollideDirection |= player.CollidedDirection;
	}

	player.x = NewX;
	player.y = NewY;

	// �v���C���[����ʂ̗̈�O�ɍs������A�E�g
	//if (player.y > 375 || player.x < 32 || player.x > 596 || player.y < 32) {
	//	for (int i = 0; i < 50; ++i) {
	//		auto pt = new Particle(player.x, player.y);
	//		particle.Factory(pt);
	//	}
	//	player.deathcount1 = player.deathcount2;
	//	// maxhp = hp = 6;
	//	player.x = 60, player.y = 100;
	//	player.fly = 0;
	//	player.deathcount2++;
	//}
	// ���S���u
	if (player.deathcount1 < player.deathcount2) {
		player.deathcount1 = player.deathcount2;
		for (int i = 0; i < 50; ++i) {
			auto pt = new Particle(player.x, player.y);
			particle.Factory(pt);
		}
		// maxhp = hp = 6;
		player.x = 60, player.y = 100;
		// time = 0;
		player.InvulnerableTime = 60;
		return;
	}
	// �����Ԃ��̃G�t�F�N�g�̍X�V
	particle.UpdateParticles();
	if (time >= 270 && !flag) {
		flag = true;
		time2 = 0;
		PlaySoundMem(ThrowSound, DX_PLAYTYPE_BACK);
	}
	// ���Ԍo�߂ɂ���ăg�Q���΂�
	if (time >= 300) {
		int i, j, k = 0, dir;
		switch (hp)
		{
		case 6:
		case 5:
			drillcount = 4;
			i = (GetRand(1)) ? 0 : W - 1;
			dir = (i == 0) ? 3 : 1;
			for (j = 0; j < H; ++j) {
				if (MapTiles[i][j] >= drillsuf - 4 && GetRand(1)) {//�����g�Q
					drill[k] = Tile{ (i + (k + 8) * dx[dir]) * 32, j * 32, 0, 0, 32, 32,true,true, dir };
					++k;
					if (k == drillcount) {
						break;
					}
				}
			}
			time = 0;
			break;
		case 4:
		case 3:
			drillcount = 4;
			j = (GetRand(1)) ? 0 : H - 1;
			dir = (j == 0) ? 2 : 0;
			for (i = 0; i < W; ++i) {
				if (MapTiles[i][j] >= drillsuf - 4 && GetRand(1)) {//�����g�Q
					drill[k] = Tile{ i * 32, (j + (k + 8)*dy[dir]) * 32, 0, 0, 32, 32,true,true, dir };
					++k;
					if (k == drillcount) {
						break;
					}
				}
			}
			time = 0;
			break;
		case 2:
		case 1:
			drillcount = 5;
			for (i = 0; i < W; ++i) {
				for (j = 0; j < H; ++j) {
					if (MapTiles[i][j] >= 5 && GetRand(1)) {//�����g�Q
						int dir = MapTiles[i][j] - 5;
						drill[k] = Tile{ (i + dx[dir] * (k + 8)) * 32, (j + dy[dir] * k) * 32, 0, 0, 32, 32,true,true, dir };
						++k;
						if (k == drillcount) {
							i = W;
							break;
						}
					}
				}
			}
			time = 0;
			break;
		default:
			break;
		}
		--hp;

		// 覐΂̃G�t�F�N�g��o�ꂳ����
		SpawnAsteroidEffect(Asteroids, MaxAsteroidNum, 10);
	}

	// 覐΂̃G�t�F�N�g���X�V����
	UpdateAsteroidEffect(Asteroids, MaxAsteroidNum);
}

void Boss::Draw() {
	// �{�X�̘r�̓���
	if (!flag) {
		ax = -40, ay = -100;
	}
	else {
		++time2;
		// ���f�����g������]����
		complex<double> ci(-40, -100);
		if (time2 < 30) {
			ci = polar(abs(ci), arg(ci) + DTOR(45.0));
			ax = real(ci), ay = imag(ci);
		}
		else {
			ci = polar(abs(ci), arg(ci) + DTOR(-45.0));
			ax = real(ci) + 25, ay = imag(ci);
		}
		if (time2 > 60) {
			flag = false;
		}
	}

	DrawGraph(0, 0, GetHandle("�w�i2"), TRUE);
	DrawGraph(ax, ay, arm, TRUE);
	DrawGraph(0, 0, body, TRUE);
	// ���S�񐔂̕\��
	//DrawFormatString(500, 40, blue, "%d", player.deathcount2);
	
	// 覐΂̃G�t�F�N�g�̕`��
	DrawAsteroidEffect(Asteroids, MaxAsteroidNum);
	
	// �v���C���[�̕`��
	if ((player.InvulnerableTime / 3) % 2 == 0) {
		if (player.FaceDirection == Player::Direction::Direction_Left) {
			DrawTurnGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
		}
		else {
			DrawGraph(player.x, player.y, PlayerImageHandles[((player.AnimationFlame / 5) + 1) % 3], TRUE);
		}
	}

	// �g�Q�̕`��
	for (int i = 0; i < drillcount; ++i) {
		const int GraphSize = 32;
		// �i�s�����Ƌt���������߂邽�߂̔z��
		const int ReversedDirectionList[4] = { 2, 3, 0, 1 };

		if (drill[i].x < 0) {
			// �`��ʒu
			const int DrawX = -drill[i].x / 2;
	
			DrawRotaGraph(DrawX + GraphSize / 2, drill[i].y + GraphSize / 2, 0.5, 0.0, toge[ReversedDirectionList[drill[i].dir]], TRUE);
		} else if (drill[i].x > 640) {
			// �`��ʒu
			int DrawX = 640 - (drill[i].x - 640) / 2;
		
			DrawRotaGraph(DrawX + GraphSize / 2, drill[i].y + GraphSize / 2, 0.5, 0.0, toge[ReversedDirectionList[drill[i].dir]], TRUE);
		} else if (drill[i].y < 0) {
			// �`��ʒu
			const int DrawY = -drill[i].y / 2;
			
			DrawRotaGraph(drill[i].x + GraphSize / 2, DrawY + GraphSize / 2, 0.5, 0.0, toge[ReversedDirectionList[drill[i].dir]], TRUE);
		} else if (drill[i].y > 480) {
			// �`��ʒus
			const int DrawY = 640 - (drill[i].y - 640) / 2;
		
			DrawRotaGraph(drill[i].x + GraphSize / 2, DrawY + GraphSize / 2, 0.5, 0.0, toge[ReversedDirectionList[drill[i].dir]], TRUE);
		} else {
			DrawGraph(drill[i].x, drill[i].y, toge[drill[i].dir], TRUE);
		}
	}
	// �����Ԃ��̃G�t�F�N�g�̕`��
	particle.DrawParticles();
	// ���t�g�̕`��
	for (int index = 0; index < TILE_MAX; index++) {
		if (Lifts[index].IsEnabled) {
			// Lifts[index].ApplyParticles(particle);
			Lifts[index].Draw();
		}
	}
	// ������ьŒ�̃g�Q�̕`��
	for (int i = 0; i < W; ++i) {
		for (int j = 0; j < H; ++j) {
			if (tile[i][j].pattern <= 1) {
				tile[i][j].Draw();
			}
		}
	}

	DrawGraph(360, 0, DeathCountImage, TRUE);
	DrawNumber(470, 0, player.deathcount1);
	DrawGraph(27, 394, GetHandle("�_�̃e���b�v"), TRUE);

	DrawGraph(570, 40, SecImage, TRUE);
	DrawNumber(410, 40, GetLeftTime() / 60);
}

// �Q�[���I�[�o�[���ǂ���
bool Boss::IsOver() {
	// �Q�[���I�[�o�[�̏����������ɒǉ�
	//player.deathcount3 = player.deathcount2 + 10;
	return player.deathcount2 >= player.deathcount3+(180*60-timer)/60/30;

	return gameover;
	// ���̏������͎��̏������Ɠ���
	// if (gameover) {
	//		return true;
	// } else {
	//		return false;
	// }
}

// �Q�[���N���A�������ǂ���
bool Boss::IsEnd() {
	// �Q�[���N���A�̏����������ɒǉ�
	return time >= 60 * 30 || hp <= 0;
	// ���̏������͎��̏������Ɠ���
	// if (time >= 60 * 30 || hp <= 0) {
	//		return true;
	// } else {
	//		return false;
	// }
}

int Boss::GetLeftTime() const {
	return hp * 300 - time;
}

Boss enemy;

STATE boss() {
	if (!bossflag) {
		// �{�X�ʂ̏�����
		enemy.Init();
		bossflag = true;
		SetLoopPosSoundMem(9600, enemy.bgm);
		PlaySoundMem(enemy.bgm, DX_PLAYTYPE_LOOP);
		AddGraphicHandle("�_�̃e���b�v", "Graphic/�_�̃e���b�v.png");

		if (ThrowSound == -1) {
			ThrowSound = LoadSoundMem("���y/���hQGJ_SE_�n���}�[�U�艺�낵.ogg");
		}
	}
	else {
		enemy.Update();
		enemy.Draw();
		// �Q�[���I�[�o�[�ɂȂ��Ă�����
		if (enemy.IsOver()) {
			if (CheckSoundMem(enemy.bgm) == TRUE) {
				StopSoundMem(enemy.bgm);
			}
			return GAMEOVER;
		}
		// �Q�[���N���A������
		if (enemy.IsEnd()) {
			if (CheckSoundMem(enemy.bgm) == TRUE) {
				StopSoundMem(enemy.bgm);
			}
			return RESULT;
		}
		if (getResetRequestStatus()) {
			if (CheckSoundMem(enemy.bgm) == TRUE) {
				StopSoundMem(enemy.bgm);
			}
			return TITLE;
		}
	}
	return BOSS;
}


bool resultflag = false;
int resultHandle;
int FontHandle2;
// ���S�񐔂�\�����邽�߂̐���
int DeathCountNumberImages[10] = { -1 };
// �N���A���Ԃ�\�����邽�߂̐���
int ClearTimeNumberImages[10] = { -1 };

STATE result() {
	if (!resultflag) {
		resultHandle = LoadGraph("Graphic/���U���g���.png");
		AddGraphicHandle("���U���g��ʃX�R�A", "Graphic/���U���g��ʃX�R�A.png");
		FontHandle = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
		FontHandle2 = CreateFontToHandle(NULL, 30, 3, DX_FONTTYPE_ANTIALIASING);

		if (DeathCountNumberImages[0] == -1) {
			LoadDivGraph("Graphic/���S�񐔃��U���g����.png", 10, 10, 1, 32, 36, DeathCountNumberImages);
			LoadDivGraph("Graphic/�b�����U���g����.png", 10, 10, 1, 32, 36, ClearTimeNumberImages);
		}

		resultflag = true;
	}
	else {
		DrawGraph(0, 0, resultHandle, false);
		DrawGraph(15, 258, GetHandle("���U���g��ʃX�R�A"), TRUE);
		//DrawFormatStringToHandle(50, 300, GetColor(0, 255, 0), FontHandle, "���S�� %3d��\n", player.deathcount2);
		//DrawFormatStringToHandle(50, 350, GetColor(0, 255, 0), FontHandle, "�N���A���� %3.1f�b\n", (double)timer / 60);
		DrawNumber(75, 308, player.deathcount2, DeathCountNumberImages);
		DrawNumber(75, 368, timer / 60, ClearTimeNumberImages);

		// DrawStringToHandle(100, 400, "PRESS SPACE", GetColor(0, 0, 255), FontHandle2);
		// �� �悭�킩��Ȃ������Ȃ̂ŃR�����g�A�E�g�����Ă��炢�܂���
		// player.deathcount1 = 0;
		// player.deathcount2 = 0;
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
		player.deathcount1 = 0;
		player.deathcount2 = 0;
			titleflag = false;
			bossflag = false;
			gameflag = false;
			resultflag = false;
			return TITLE;
		}
	}
	return RESULT;
}


bool gameoverflag = false;
int gameoverHandle;

STATE gameover() {
	if (!gameoverflag) {
		gameoverHandle = LoadGraph("Graphic/gameover.png");
		gameoverflag = true;
		PlaySoundMem(Sound3, DX_PLAYTYPE_LOOP);
	}
	else {
		DrawGraph(0, 0, gameoverHandle, true);
		player.deathcount1 = 0;
		player.deathcount2 = 0;
		if (getKeyPress(KEY_INPUT_SPACE, PRESS_ONCE)) {
			titleflag = false;
			gameflag = false;
			bossflag = false;
			gameoverflag = false;
			StopSoundMem(Sound3);
			return TITLE;
		}
	}
	return GAMEOVER;
}