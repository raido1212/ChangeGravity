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

//ステージ
char stage[_Stage_h][_Stage_w];

//タイマー　秒数
int Time = 600*2;

//敵の残数 今のところ使わない
//int Stage_kill_Total = 0;

//キー入力状態
int Key[256];

//ボタンが押された瞬間の処理
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

//プレイヤー処理
void cPlayer::Init()
{
	getItem = 0;
	Life = 1;

	//プレイヤー座標決定
	Player_UpperleftPosx = _window_w / 2 - Player_Size/3;
	Player_UpperleftPosy = _window_h / 2 - Player_Size/3;
	Player_BottomrightPosx = _window_w / 2 + Player_Size/3;
	Player_BottomrightPosy = _window_h / 2 + Player_Size/3;
	//プレイヤー中心座標
	Player_posx = Player_UpperleftPosx + (Player_Size / 2);
	Player_posy = Player_UpperleftPosy + (Player_Size / 2);
}

//プレイヤーの状態を更新
void cPlayer::Updata()
{
	//移動処理
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

	//座標確認と更新
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
	//プレイヤー中心座標更新
	Player_posx = Player_UpperleftPosx + (Player_Size / 2);

}

//プレイヤー位置を判定
//0  下に下がる
//-1 上に上がる
int cPlayer::Check_ContactArea()
{
	if (stage[_Stage_h][Player_posx /16 -40] ==0) {
		return -1;
	}
	else { return 0; }
}

//プレイヤーが上下に行くと死亡させる関数
int cPlayer::Check_Life(int y)
{
	//縦処理
	if (y >= _window_h || y <= 0) {
		return 1;
	}
	else return 0;
}

//プレイヤーに弾が当たったか判定する関数
int cPlayer::Check_Bullet(int x, int y)
{
	if ((Player_UpperleftPosx <= x) && (Player_BottomrightPosx >= x) && (Player_UpperleftPosy <= y) && (Player_BottomrightPosy >= y)) {
		return 1;
	}
	else { return 0; }
}

//プレイヤー描画処理
void cPlayer::Draw()
{
	DrawBox(Player_UpperleftPosx, 
		Player_UpperleftPosy, 
		Player_BottomrightPosx, 
		Player_BottomrightPosy, GetColor(255, 0, 0), TRUE);
}

//敵　処理
void cEnemy::Init()
{
	

	radius = 90;
	enemy_move = radius * PI / 180;

	enemy_attack_time = 0;					//攻撃までの時間 
	enemy_attack_flag = 0;					//攻撃フラグ 
	enemy_life = 1;							//敵のライフ 
	enemy_attack_bullet_posx = 0;			//敵の弾の座標 
	enemy_attack_bullet_posy = 0;			//
	enemy_attack_add_time = GetRand(1000);	//乱数取得

	
}

void cEnemy::Updata()
{
	
	if (enemy_life == 1) {
		//動き処理
		enemy_posy = enemy_posy + (0.8 * sin(enemy_move));
		if (radius == 360) {
			radius = 0;
		}
		radius++;
		enemy_move = radius* PI / 180;	

		//攻撃フラグがたっていないとき
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
		
		//攻撃フラグがたったら
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

//敵を描画
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

//敵を反転して描画する処理
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

//敵の弾の処理
void cEnemy::Draw_Bullet()
{
	if (enemy_attack_flag == 1) {
		DrawCircle(enemy_attack_bullet_posx, enemy_attack_bullet_posy, 6, GetColor(125, 125, 125), TRUE);
	}
	else{}
}


//ステージセレクト画面　処理
void cStageSelectMenu::Init()
{
	Play_Level = 1;			//難易度設定

	box_upperleftx = 80;	//十字キーで移動できる四角い枠の座標
	box_upperlefty = 160;	//
	box_bottomrightx = 160;	//
	box_bottomrighty = 320;	//
	
	box_gap = 40;			//画面左の空白部分
}

//ステージセレクト　移動と決定の処理
void cStageSelectMenu::Updata()
{
	if (Key[KEY_INPUT_RIGHT] == 1) {
		if (Play_Level == 3) {		//移動できないようにする

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
	DrawString((int)(_window_w * 0.18), (int)_window_h / 3, "操\n作\n説\n明",GetColor(255,255,255));
	DrawString((int)(_window_w * 0.49), (int)_window_h / 3, "ノ\n|\nマ\nル\nモ\nー\nド", GetColor(255, 255, 255));
	DrawString((int)(_window_w * 0.8), (int)_window_h / 3, "ハ\n|\nド\nモ\nー\nド\n", GetColor(255, 255, 255));
}

//タイマー処理
int Timer()
{
	
	DrawFormatString(_window_w / 2, 0, GetColor(0, 255, 0), "%2d:%2d", Time / 60, Time % 60);
	Time--;
	if (Time == 0) {
		return 1;
	}
	else return 0;
}


//モード
enum Mode {
	ModeTitle,
	ModeStageSelect,
	ModeTutorial,
	ModeGame,
	ModeGameHard,
	ModeClear,
	ModeOver,
};
//ステージ処理
void DrawStage()
{
	memset(stage, 0, sizeof(stage));
	int stage_Size = 16;												//上下の四角の大きさ
	int stage_UpperleftPosx = 0;										//四角の左上X座標
	int stage_UpperleftPosy = _window_h - stage_Size * _Stage_h;		//四角の左上Y座標
	int stage_BottomrightPosx = stage_Size;								//四角の右上X座標
	int stage_BottomrightPosy = _window_h - stage_Size * (_Stage_h - 1);//四角の右上Y座標

	for (int i = 0; i < _Stage_h; i++) {
		for (int j = 0; j < _Stage_w; j++) {
			if (j % 8  < 4) {
				//塗りつぶしで描画
				if (i < _Stage_h / 2) {
					DrawBox(j * stage_Size, i * stage_Size, (j + 1) *stage_Size, (i + 1)*stage_Size, GetColor(255, 255, 255), TRUE);
				}
				//枠だけ描画
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


// アイテムの処理

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
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //ウィンドウモード変更と初期化と裏画面設定

	//変数宣言
	cPlayer cplayer[1];
	cEnemy cenemy[_enemy_number];
	cStageSelectMenu cstageselectmenu;
	//cItem citem(500,500);
	
	cItem *citem[5];

	for (int i = 0; i < 5; i++) {
		citem[i] =  new cItem(100,i * 50);
	}

	int ItemTextLoad = FileRead_open("ItemPos.txt");


	//BGM　格納
	static int MainMusic = LoadSoundMem("シューティングゲーム　タイトル　ループ.wav");
	static int BattleMusic = LoadSoundMem("シューティング　戦闘.wav");

	//音量調整
	ChangeVolumeSoundMem(200,MainMusic);
	ChangeVolumeSoundMem(200,BattleMusic);

	int MusicFlag = 0;
	int Init_Flag = 0;
	int Tutorial_number = 0;
	
	Mode mode;	
	mode = ModeTitle;
	
	
	// while( 裏画面を表画面に反映, メッセージ処理, 画面クリア )
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyInput() == 0) {
		DrawStage();
		switch (mode) {
			//タイトル画面
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
			//ステージセレクト画面
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
			//操作方法を選択した時
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
				LoadGraphScreen(0, 0, "チュートリアル1.png", TRUE);
			}
			if (Tutorial_number == 1) {
				LoadGraphScreen(0, 0, "チュートリアル2.png", TRUE);
			}
			if (Tutorial_number == 2) {
				LoadGraphScreen(0, 0, "チュートリアル3.png", TRUE);
			}

			if (Key[KEY_INPUT_ESCAPE]) {
				Tutorial_number = 0;
				mode = ModeStageSelect;
			}
			break;
			//ノーマルモードを選択した時
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
			//ハードモードを選択した時
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
			//ゲームクリア処理
		case ModeClear:
			LoadGraphScreen(_window_w / 4, _window_h / 4, "GameClear.bmp", TRUE);
			if (MusicFlag == 0) {
				PlaySoundFile("シューティング　戦闘　勝利.wav", DX_PLAYTYPE_BACK);
				MusicFlag++;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE)) {
				StopSoundFile();
				MusicFlag = 0;
				mode = ModeTitle;
			}
			break;
			//ゲームオーバー処理
		case ModeOver:
			LoadGraphScreen(_window_w / 4, _window_h / 4, "GameOver.bmp", TRUE);
			if (MusicFlag == 0) {
				PlaySoundFile("シューティング　戦闘　敗北.wav",DX_PLAYTYPE_BACK);
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

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}