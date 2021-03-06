#pragma once

class cItem {
	float posx, posy;
	int image[20];
	short imageCount;
	short width;
	
public:
	bool judge;
	cItem(int x,int y) {
		// 画像読み込みをここでする
		LoadDivGraph("Item.png", 20, 5, 4, 192, 192, image);
		// 画像の幅も決める
		width = 192;
		// 座標読み込み
		posx = x, posy = y;
		judge = false;
		imageCount = 0;
	}
	bool GetItem(int playerx,int playery);
	void DrawItem();
	void Update();
	void Init();
	bool GetJudge() { return judge; }
};