#pragma once
#include <GameObject2D.h>
class GameOverSprite :
    public MelLib::GameObject2D
{
private:
	float alphaMul = 0.0f;

public:
	static void LoadResources();

	//コンストラクタ
	GameOverSprite();
	//デストラクタ

	//更新
	void Update()override;
	//描画
	void Draw()override;
};

