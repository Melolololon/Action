#pragma once
#include<Particle3D.h>

// 砂煙のエフェクト(ボスのダッシュ攻撃などで表示)(プレイヤーのダッシュで出してもいいかも)
class SandEffect : public MelLib::Particle3D
{
private:
	const float FRAME_MOVE_DIS = 0.2f;

	// 1フレでの減算アルファ値(パーセント)
	const float FRAME_SUB_ALPHA = 5.0f;
	float subAlpha = 0.0f;

public:
	static void LoadResources();

	void Initialize()override;
	void Update()override;
	std::shared_ptr<Particle3D>GetNewPtr()const override;


};

