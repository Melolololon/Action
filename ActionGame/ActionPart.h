#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//�e�X�g�p
#include<ModelObject.h>

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
private:

private:
	//�e�X�g�p
	MelLib::ModelObject obj;


private:
	void LoadResources();


public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��

};

