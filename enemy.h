#pragma once

class cEnemy
{
private:
	int Load_enemy_graph[2];
	
	int radius;
	int enemy_attack_add_time;
	float enemy_move;

	int enemy_attack_sound;
public:
	int enemy_attack_flag;
	int enemy_attack_time;

	int enemy_attack_bullet_posx;
	int enemy_attack_bullet_posy;
	
	int enemy_life;

	int enemy_posx;
	float enemy_posy;

	cEnemy() {
		enemy_attack_sound = LoadSoundMem("íeÅ@î≠éÀ.wav");
		LoadDivGraph("enemy.bmp", 2, 1, 2, 32, 32, Load_enemy_graph);
	}

	void Init();
	void Draw();
	void Draw_Inverted();
	void Draw_Bullet();
	void Updata();

};