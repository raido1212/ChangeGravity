#include <string>
#include <math.h>

#include "DxLib.h"
#include "Player.h"
#include "enemy.h"
#include "StageSelectMenu.h"
#include "Item.h"

#define PI 3.141592f
#define _window_w 640
#define _window_h 480
#define _Stage_w 40
#define _Stage_h 4
#define _enemy_number 20
#define _enemy_number_normal 10
#define _enemy_attack_flag_time 1500
#define _item_max 1

//�X�e�[�W
char stage[_Stage_h][_Stage_w];

//�^�C�}�[�@�b��
int Time = 600*2;

//�G�̎c�� ���̂Ƃ���g��Ȃ�
//int Stage_kill_Total = 0;

//�L�[���͏��
int Key[256];

//�{�^���������ꂽ�u�Ԃ̏���
int KeyInput() {
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {
			Key[i]++;
		}
		else {
			Key[i] =  0;
		}
	}return 0;

}

//�v���C���[����
void cPlayer::Init()
{
	getItem = 0;
	Life = 1;

	//�v���C���[���W����
	Player_UpperleftPosx = _window_w / 2 - Player_Size/3;
	Player_UpperleftPosy = _window_h / 2 - Player_Size/3;
	Player_BottomrightPosx = _window_w / 2 + Player_Size/3;
	Player_BottomrightPosy = _window_h / 2 + Player_Size/3;
	//�v���C���[���S���W
	Player_posx = Player_UpperleftPosx + (Player_Size / 2);
	Player_posy = Player_UpperleftPosy + (Player_Size / 2);
}

//�v���C���[�̏�Ԃ��X�V
void cPlayer::Updata()
{
	//�ړ�����
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		Player_UpperleftPosx+=3;
		Player_BottomrightPosx+=3;
		Player_posx+=3;
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		Player_UpperleftPosx-=3;
		Player_BottomrightPosx-=3;
		Player_posx-=3;
	}

	if (CheckHitKey(KEY_INPUT_UP)) {
		Player_UpperleftPosy-=3;
		Player_BottomrightPosy-=3;
		Player_posy -= 3;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		Player_UpperleftPosy+=3;
		Player_BottomrightPosy+=3;
		Player_posy += 3;
	}

	//���W�m�F�ƍX�V
	if (Check_ContactArea() == -1) {
		Player_UpperleftPosy--;
		Player_BottomrightPosy--;
		Player_posy--;
	}
	if (Check_ContactArea() == 0) {
		Player_UpperleftPosy++;
		Player_BottomrightPosy++;
		Player_posy++;
	}
	//�v���C���[���S���W�X�V
	Player_posx = Player_UpperleftPosx + (Player_Size / 2);

}

//�v���C���[�ʒu�𔻒�
//0  ���ɉ�����
//-1 ��ɏオ��
int cPlayer::Check_ContactArea()
{
	if (stage[_Stage_h][Player_posx /16 -40] ==0) {
		return -1;
	}
	else { return 0; }
}

//�v���C���[���㉺�ɍs���Ǝ��S������֐�
int cPlayer::Check_Life(int y)
{
	//�c����
	if (y >= _window_h || y <= 0) {
		return 1;
	}
	else return 0;
}

//�v���C���[�ɒe���������������肷��֐�
int cPlayer::Check_Bullet(int x, int y)
{
	if ((Player_UpperleftPosx <= x) && (Player_BottomrightPosx >= x) && (Player_UpperleftPosy <= y) && (Player_BottomrightPosy >= y)) {
		return 1;
	}
	else { return 0; }
}

//�v���C���[�`�揈��
void cPlayer::Draw()
{
	DrawBox(Player_UpperleftPosx, 
		Player_UpperleftPosy, 
		Player_BottomrightPosx, 
		Player_BottomrightPosy, GetColor(255, 0, 0), TRUE);
}

//�G�@����
void cEnemy::Init()
{
	

	radius = 90;
	enemy_move = radius * PI / 180;

	enemy_attack_time = 0;					//�U���܂ł̎��� 
	enemy_attack_flag = 0;					//�U���t���O 
	enemy_life = 1;							//�G�̃��C�t 
	enemy_attack_bullet_posx = 0;			//�G�̒e�̍��W 
	enemy_attack_bullet_posy = 0;			//
	enemy_attack_add_time = GetRand(1000);	//�����擾

	
}

void cEnemy::Updata()
{
	
	if (enemy_life == 1) {
		//��������
		enemy_posy = enemy_posy + (0.8 * sin(enemy_move));
		if (radius == 360) {
			radius = 0;
		}
		radius++;
		enemy_move = radius* PI / 180;	

		//�U���t���O�������Ă��Ȃ��Ƃ�
		if (enemy_attack_flag == 0) {

			if (enemy_attack_time + enemy_attack_add_time < _enemy_attack_flag_time) {
				enemy_attack_time+=10;
			}

			if (enemy_attack_time + enemy_attack_add_time >= _enemy_attack_flag_time) {
				enemy_attack_flag = 1;
				enemy_attack_bullet_posx = enemy_posx;
				enemy_attack_bullet_posy = enemy_posy;
				PlaySoundMem(enemy_attack_sound, DX_PLAYTYPE_BACK);
			}
		}
		
		//�U���t���O����������
		if (enemy_attack_flag == 1) {
			if (enemy_posx > 320) {
				enemy_attack_bullet_posx-= 5;
				
			}
			else {
				
				enemy_attack_bullet_posx+= 5;
			}
			if (enemy_attack_bullet_posx <= 0 || enemy_attack_bullet_posx >= _window_w) {
				enemy_attack_add_time = GetRand(300);
				enemy_attack_flag = 0;
				enemy_attack_time = 0;
			}
		}

	}	
}

//�G��`��
void cEnemy::Draw()
{
	if (enemy_life == 1) {
		if (enemy_attack_flag == 1) {
			DrawRotaGraph((int)enemy_posx, (int)enemy_posy, 1.0, 0, Load_enemy_graph[1], TRUE, TRUE);
		}
		else {
			DrawRotaGraph((int)enemy_posx, (int)enemy_posy, 1.0, 0, Load_enemy_graph[0], TRUE, TRUE);
		}
	}
	else{}
}

//�G�𔽓]���ĕ`�悷�鏈��
void cEnemy::Draw_Inverted()
{
	if (enemy_life == 1) {
		if (enemy_attack_flag == 1) {
			DrawRotaGraph((int)enemy_posx, (int)enemy_posy, 1.0, 0, Load_enemy_graph[1], TRUE, FALSE);
		}
		else {
			DrawRotaGraph((int)enemy_posx, (int)enemy_posy, 1.0, 0, Load_enemy_graph[0], TRUE, FALSE);
		}
	}
	else {}
}

//�G�̒e�̏���
void cEnemy::Draw_Bullet()
{
	if (enemy_attack_flag == 1) {
		DrawCircle(enemy_attack_bullet_posx, enemy_attack_bullet_posy, 6, GetColor(125, 125, 125), TRUE);
	}
	else{}
}


//�X�e�[�W�Z���N�g��ʁ@����
void cStageSelectMenu::Init()
{
	Play_Level = 1;			//��Փx�ݒ�

	box_upperleftx = 80;	//�\���L�[�ňړ��ł���l�p���g�̍��W
	box_upperlefty = 160;	//
	box_bottomrightx = 160;	//
	box_bottomrighty = 320;	//
	
	box_gap = 40;			//��ʍ��̋󔒕���
}

//�X�e�[�W�Z���N�g�@�ړ��ƌ���̏���
void cStageSelectMenu::Updata()
{
	if (Key[KEY_INPUT_RIGHT] == 1) {
		if (Play_Level == 3) {		//�ړ��ł��Ȃ��悤�ɂ���

		}
		else {
			box_bottomrightx += (_window_w / 4) + box_gap;
			box_upperleftx += (_window_w / 4) + box_gap;
			Play_Level++;
		}
	}
	if (Key[KEY_INPUT_LEFT] == 1) {
		if (Play_Level == 1) {

		}
		else {
			box_bottomrightx += -(_window_w / 4) - box_gap;
			box_upperleftx += -(_window_w / 4) - box_gap;
			Play_Level--;
		}
	}

}


void cStageSelectMenu::Draw()
{
	DrawBox((int)box_upperleftx, (int)box_upperlefty, (int)box_bottomrightx, (int)box_bottomrighty, GetColor(255, 255, 255), FALSE);
	DrawString((int)(_window_w * 0.18), (int)_window_h / 3, "��\n��\n��\n��",GetColor(255,255,255));
	DrawString((int)(_window_w * 0.49), (int)_window_h / 3, "�m\n|\n�}\n��\n��\n�[\n�h", GetColor(255, 255, 255));
	DrawString((int)(_window_w * 0.8), (int)_window_h / 3, "�n\n|\n�h\n��\n�[\n�h\n", GetColor(255, 255, 255));
}

//�^�C�}�[����
int Timer()
{
	
	DrawFormatString(_window_w / 2, 0, GetColor(0, 255, 0), "%2d:%2d", Time / 60, Time % 60);
	Time--;
	if (Time == 0) {
		return 1;
	}
	else return 0;
}


//���[�h
enum Mode {
	ModeTitle,
	ModeStageSelect,
	ModeTutorial,
	ModeGame,
	ModeGameHard,
	ModeClear,
	ModeOver,
};
//�X�e�[�W����
void DrawStage()
{
	memset(stage, 0, sizeof(stage));
	int stage_Size = 16;												//�㉺�̎l�p�̑傫��
	int stage_UpperleftPosx = 0;										//�l�p�̍���X���W
	int stage_UpperleftPosy = _window_h - stage_Size * _Stage_h;		//�l�p�̍���Y���W
	int stage_BottomrightPosx = stage_Size;								//�l�p�̉E��X���W
	int stage_BottomrightPosy = _window_h - stage_Size * (_Stage_h - 1);//�l�p�̉E��Y���W

	for (int i = 0; i < _Stage_h; i++) {
		for (int j = 0; j < _Stage_w; j++) {
			if (j % 8  < 4) {
				//�h��Ԃ��ŕ`��
				if (i < _Stage_h / 2) {
					DrawBox(j * stage_Size, i * stage_Size, (j + 1) *stage_Size, (i + 1)*stage_Size, GetColor(255, 255, 255), TRUE);
				}
				//�g�����`��
				else {
					DrawBox(stage_UpperleftPosx, stage_UpperleftPosy, stage_BottomrightPosx, stage_BottomrightPosy, GetColor(255, 255, 255), TRUE);
				}
				stage[i][j] = 1;
			}
			else {
				if (i < _Stage_h / 2) {
					DrawBox(j * stage_Size, i * stage_Size, (j + 1) *stage_Size, (i + 1)*stage_Size, GetColor(255, 255, 255), FALSE);
				}
				else {
					DrawBox(stage_UpperleftPosx, stage_UpperleftPosy, stage_BottomrightPosx, stage_BottomrightPosy, GetColor(255, 255, 255), FALSE);
				}
			}
			stage_UpperleftPosx += stage_Size;
			stage_BottomrightPosx += stage_Size;
		}
		stage_UpperleftPosy += stage_Size;
		stage_BottomrightPosy += stage_Size;

		stage_UpperleftPosx = 0;
		stage_BottomrightPosx = stage_Size;
	}

}


// �A�C�e���̏���

void cItem::Init() {
	judge = false;
}

void cItem::Update() {
	static short count = 0;
	count++;
	if (count = 60) {
		count = 0;
		imageCount++;
	}
	if (imageCount == 19)imageCount = 0;

}

bool cItem::GetItem(int playerx,int playery) {
	if (posx < playerx &&
		posx + width > playerx&&
		posy < playery &&
		posy + width > playery) {
		judge = true;
		return true;
	}
	else if (judge == false) {
		return false;
	}
}

void cItem::DrawItem() {
	// DrawGraph(posx, posy, image[imageCount], TRUE);
	DrawExtendGraph(posx, posy, posx + 32, posy + 32, image[imageCount], TRUE);
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�

	//�ϐ��錾
	cPlayer cplayer[1];
	cEnemy cenemy[_enemy_number];
	cStageSelectMenu cstageselectmenu;
	//cItem citem(500,500);
	
	cItem *citem[5];

	for (int i = 0; i < 5; i++) {
		citem[i] =  new cItem(100,i * 50);
	}

	int ItemTextLoad = FileRead_open("ItemPos.txt");


	//BGM�@�i�[
	static int MainMusic = LoadSoundMem("�V���[�e�B���O�Q�[���@�^�C�g���@���[�v.wav");
	static int BattleMusic = LoadSoundMem("�V���[�e�B���O�@�퓬.wav");

	//���ʒ���
	ChangeVolumeSoundMem(200,MainMusic);
	ChangeVolumeSoundMem(200,BattleMusic);

	int MusicFlag = 0;
	int Init_Flag = 0;
	int Tutorial_number = 0;
	
	Mode mode;	
	mode = ModeTitle;
	
	
	// while( ����ʂ�\��ʂɔ��f, ���b�Z�[�W����, ��ʃN���A )
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyInput() == 0) {
		DrawStage();
		switch (mode) {
			//�^�C�g�����
		case ModeTitle:
			LoadGraphScreen(_window_w / 4, _window_h / 4, "Gravity Change.bmp", TRUE);
			if (Init_Flag == 0) {
				for (int i = 0; i <= _enemy_number - 1; i++) {
					cenemy[i].Init();
					if (i <= 9) {
						cenemy[i].enemy_posx = 16;
						cenemy[i].enemy_posy = (float)(48 * i) + 16;
					}
					else if (i >= 10) {
						cenemy[i].enemy_posx = _window_w - 16;
						cenemy[i].enemy_posy = (float)(48 * (i - 10)) + 16;
					}
				}
				Init_Flag++;
			}
			if (CheckSoundMem(MainMusic) == 0) {
				PlaySoundMem(MainMusic, DX_PLAYTYPE_LOOP);
			}
			
			cplayer[0].Init();
			cstageselectmenu.Init();
			if (Key[KEY_INPUT_Z] == 1) {
				Time = 600 * 1;
				mode = ModeStageSelect;
				Init_Flag = 0;
				for (int i = 0; i < 5; i++) {
					citem[i]->Init();
				}
			}
			break;
			//�X�e�[�W�Z���N�g���
		case ModeStageSelect:
			if (CheckSoundMem(MainMusic) == 0) {
				PlaySoundMem(MainMusic, DX_PLAYTYPE_LOOP);
			}

			cstageselectmenu.Updata();
			cstageselectmenu.Draw();
			if (cstageselectmenu.Play_Level == 1 && Key[KEY_INPUT_Z] == 1) {
				mode = ModeTutorial;
			}
			else if (cstageselectmenu.Play_Level == 2 && Key[KEY_INPUT_Z] == 1) {
				mode = ModeGame;
				StopSoundMem(MainMusic);
			}
			/*else if (cstageselectmenu.Play_Level == 3 && Key[KEY_INPUT_Z] == 1) {
				mode = ModeGameHard;
				StopSoundMem(MainMusic);
			}*/
			break;
			//������@��I��������
		case ModeTutorial:
			if (Key[KEY_INPUT_RIGHT] == 1) {
				if (Tutorial_number != 2) {
					Tutorial_number++;
				}
			}
			if (Key[KEY_INPUT_LEFT] == 1) {
				if (Tutorial_number != 0) {
					Tutorial_number--;
				}
			}
			if (Tutorial_number == 0) {
				LoadGraphScreen(0, 0, "�`���[�g���A��1.png", TRUE);
			}
			if (Tutorial_number == 1) {
				LoadGraphScreen(0, 0, "�`���[�g���A��2.png", TRUE);
			}
			if (Tutorial_number == 2) {
				LoadGraphScreen(0, 0, "�`���[�g���A��3.png", TRUE);
			}

			if (Key[KEY_INPUT_ESCAPE]) {
				Tutorial_number = 0;
				mode = ModeStageSelect;
			}
			break;
			//�m�[�}�����[�h��I��������
		case ModeGame:
			if (CheckSoundMem(BattleMusic) == 0) {
				PlaySoundMem(BattleMusic, DX_PLAYTYPE_BACK);
			}

			//Timer();
			for (int i = 0; i < _enemy_number; i++) {
				cenemy[i].Updata();
				cenemy[i].Draw_Bullet();
				if (i <= 9) {
					cenemy[i].Draw();
				}
				if (i >= 10) {
					cenemy[i].Draw_Inverted();
				}
				if (cplayer[0].Check_Bullet(cenemy[i].enemy_attack_bullet_posx, cenemy[i].enemy_attack_bullet_posy) == 1) {
					StopSoundMem(BattleMusic);
					mode = ModeOver;
				}
			}
			cplayer[0].Draw();
			cplayer[0].Updata();

			for (int i = 0; i < 5; i++) {
				if (citem[i] != nullptr) {
					if (citem[i]->GetJudge() == false) {
						citem[i]->DrawItem();
						citem[i]->Update();
						citem[i]->GetItem(cplayer[0].GetPosX(), cplayer[0].GetPosY());
					}
					else {
						citem[i] = nullptr;
						cplayer[0].getItem++;
					}
				}
			}

			if (cplayer[0].getItem == 5) {
				mode = ModeClear;
			}

			if (cplayer[0].Check_Life(cplayer[0].Player_posy) == 1) {
				StopSoundMem(BattleMusic);
				mode = ModeOver;
			}
			if (Timer() == 1) {
				StopSoundMem(BattleMusic);
				mode = ModeClear;
			}

			DrawFormatString(200, 200, 0xfffff, "%d", cplayer[0].getItem);

			break;
			//�n�[�h���[�h��I��������
		case ModeGameHard:
			if (CheckSoundMem(BattleMusic) == 0) {
				PlaySoundMem(BattleMusic, DX_PLAYTYPE_BACK);
			}

			Timer();
			for (int i = 0; i < _enemy_number; i++) {
				cenemy[i].Updata();
				cenemy[i].Updata();
				cenemy[i].Updata();
				cenemy[i].Draw_Bullet();
				if (i <= 9) {
					cenemy[i].Draw();
				}
				if (i >= 10) {
					cenemy[i].Draw_Inverted();
				}
				if (cplayer[0].Check_Bullet(cenemy[i].enemy_attack_bullet_posx, cenemy[i].enemy_attack_bullet_posy) == 1) {
					mode = ModeOver;
					StopSoundMem(BattleMusic);
				}
			}
			cplayer[0].Draw();
			cplayer[0].Updata();
			cplayer[0].Updata();
			cplayer[0].Updata();
			if (cplayer[0].Check_Life(cplayer[0].Player_posy) == 1) {
				mode = ModeOver;
				StopSoundMem(BattleMusic);
			}
			if (Timer() == 1) {
				mode = ModeClear;
				StopSoundMem(BattleMusic);
			}
			break;
			//�Q�[���N���A����
		case ModeClear:
			LoadGraphScreen(_window_w / 4, _window_h / 4, "GameClear.bmp", TRUE);
			if (MusicFlag == 0) {
				PlaySoundFile("�V���[�e�B���O�@�퓬�@����.wav", DX_PLAYTYPE_BACK);
				MusicFlag++;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE)) {
				StopSoundFile();
				MusicFlag = 0;
				mode = ModeTitle;
			}
			break;
			//�Q�[���I�[�o�[����
		case ModeOver:
			LoadGraphScreen(_window_w / 4, _window_h / 4, "GameOver.bmp", TRUE);
			if (MusicFlag == 0) {
				PlaySoundFile("�V���[�e�B���O�@�퓬�@�s�k.wav",DX_PLAYTYPE_BACK);
				MusicFlag++;
			}
			if (CheckHitKey(KEY_INPUT_X)) {
				mode = ModeTitle;
				MusicFlag = 0;
				StopSoundFile();
			}
			DrawFormatString(320, 280, 0xfffff, "push X key");
			break;
		}
	}

	DxLib_End(); // DX���C�u�����I������
	return 0;
}