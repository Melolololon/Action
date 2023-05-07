#pragma once
#include<GameObject2D.h>
class ClearSprite
	: public MelLib::GameObject2D
{
private:


public:
	//コンストラクタ
	ClearSprite();
	//デストラクタ

	//更新
	void Update()override;
	//描画
	void Draw()override;
};

