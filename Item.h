#pragma once

class cItem {
	float posx, posy;
	int image[20];
	short imageCount;
	short width;
	
public:
	bool judge;
	cItem(int x,int y) {
		// �摜�ǂݍ��݂������ł���
		LoadDivGraph("Item.png", 20, 5, 4, 192, 192, image);
		// �摜�̕������߂�
		width = 192;
		// ���W�ǂݍ���
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