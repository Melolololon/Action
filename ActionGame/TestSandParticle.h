#pragma once
#include <GameObject.h>
#include<Sprite3D.h>
class TestSandParticle :
    public MelLib::GameObject
{const float FRAME_MOVE_DIS = 2.0f;

	// 1フレでの減算アルファ値(パーセント)
	const float FRAME_SUB_ALPHA = 5.0f;
	float subAlpha = 0.0f;
	MelLib::Sprite3D particleSprite;
public:
	static void LoadResources();
	TestSandParticle(const MelLib::Vector3 pos);
	void Initialize()override;
	void Update()override;
	void Draw()override;
	//std::shared_ptr<Particle3D>GetNewPtr()const override;
};

