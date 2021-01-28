#pragma once
//ÉvÉåÉCÉÑÅ[ê›íË
const int Player_Size = 32;
int Life = 0;

class cPlayer {
private:
	int Player_UpperleftPosx;
	int Player_UpperleftPosy;

	int Player_BottomrightPosx;
	int Player_BottomrightPosy;

public:
	int getItem;
	int Life;

	int Player_posx;
	int Player_posy;

	void Init();
	void Updata();
	void Draw();

	int Check_ContactArea();
	int Check_Bullet(int x, int y);
	int Check_Life(int y);
	int GetPosX() { return Player_UpperleftPosx + Player_Size / 2; }
	int GetPosY() { return Player_UpperleftPosy + Player_Size / 2; }
	
};
	