#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

// アニメーションを作成するクラスをエンジン側で作ってもいいかも

class SlushHitEffect :
    public MelLib::GameObject
{
private:

	MelLib::Sprite3D effectSprite;

	MelLib::FrameTimer changeTimer;

	unsigned int textureNum = 0;

	bool ret = false;
public:

	SlushHitEffect(const MelLib::Vector3& pos);
	virtual ~SlushHitEffect() {}

	virtual void Initialize();
	virtual void Update();
	void Draw();
};

