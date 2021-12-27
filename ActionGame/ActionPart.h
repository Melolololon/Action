#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//�e�X�g�p
#include<ModelObject.h>

#include"Enemy.h"

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
private:

private:
	//�e�X�g�p
	MelLib::ModelObject obj;

	// ���b�N�I���p
	static std::vector<std::shared_ptr<Enemy>>pEnemys;
private:
	void LoadResources();

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��

	static const std::vector<std::shared_ptr<Enemy>>& GetEnemys() { return pEnemys; }
};