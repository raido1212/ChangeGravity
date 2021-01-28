#pragma once

class cItem {
	float posx, posy;
	int image[20];
	short imageCount;
	short width;
	
public:
	bool judge;
	cItem(int x,int y) {
		// ‰æ‘œ“Ç‚İ‚İ‚ğ‚±‚±‚Å‚·‚é
		LoadDivGraph("Item.png", 20, 5, 4, 192, 192, image);
		// ‰æ‘œ‚Ì•‚àŒˆ‚ß‚é
		width = 192;
		// À•W“Ç‚İ‚İ
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