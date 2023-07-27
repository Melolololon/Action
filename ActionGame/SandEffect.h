#pragma once
#include<Particle3D.h>

// �����̃G�t�F�N�g(�{�X�̃_�b�V���U���Ȃǂŕ\��)(�v���C���[�̃_�b�V���ŏo���Ă���������)
class SandEffect : public MelLib::Particle3D
{
private:
	const float FRAME_MOVE_DIS = 0.2f;

	// 1�t���ł̌��Z�A���t�@�l(�p�[�Z���g)
	const float FRAME_SUB_ALPHA = 5.0f;
	float subAlpha = 0.0f;

public:
	static void LoadResources();

	void Initialize()override;
	void Update()override;
	std::shared_ptr<Particle3D>GetNewPtr()const override;


};

