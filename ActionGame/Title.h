#pragma once
#include <Scene.h>

// �^�C�g���Ƃ��̃X�v���C�g(�V�[�����ʂ���Ȃ��X�v���C�g)�́AGameObject2D�ō�邱��

class Title :
    public MelLib::Scene
{
private:
	void LoadResources();

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��
};

