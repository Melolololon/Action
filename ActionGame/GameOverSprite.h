#pragma once
#include <GameObject2D.h>
class GameOverSprite :
    public MelLib::GameObject2D
{
private:
	float alphaMul = 0.0f;

public:
	static void LoadResources();

	//�R���X�g���N�^
	GameOverSprite();
	//�f�X�g���N�^

	//�X�V
	void Update()override;
	//�`��
	void Draw()override;
};

