#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

// �A�j���[�V�������쐬����N���X���G���W�����ō���Ă���������

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

