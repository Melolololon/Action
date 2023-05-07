#pragma once
#include<GameObject2D.h>
class ClearSprite
	: public MelLib::GameObject2D
{
private:
	float alphaMul = 0.0f;
	float zanzoAlphaMul = 100.0f;

public:
	static void LoadResources();

	//コンストラクタ
	ClearSprite();
	//デストラクタ

	//更新
	void Update()override;
	//描画
	void Draw()override;
};

