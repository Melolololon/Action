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

	//�R���X�g���N�^
	ClearSprite();
	//�f�X�g���N�^

	//�X�V
	void Update()override;
	//�`��
	void Draw()override;
};

