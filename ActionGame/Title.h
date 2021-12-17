#pragma once
#include <Scene.h>
#include"Player.h"

// �^�C�g���Ƃ��̃X�v���C�g(�V�[�����ʂ���Ȃ��X�v���C�g)�́AGameObject2D�ō�邱��

class Title :
    public MelLib::Scene
{
private:
	std::shared_ptr<Player>pPlayer;

	bool pushGameStart = false;

private:
	void LoadResources();

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��
};

