#pragma once
#include"Scene.h"

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[���̎w
};

