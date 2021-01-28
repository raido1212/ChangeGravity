#pragma once
class cStageSelectMenu
{
private:
	int box_upperleftx;
	int box_upperlefty;
	int box_bottomrightx;
	int box_bottomrighty;
	int box_gap;
public:
	int Play_Level;

	void Init();
	void Draw();
	void Updata();
};