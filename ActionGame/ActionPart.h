#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//�e�X�g�p
#include<ModelObject.h>

#include"Enemy.h"

#include"Stage.h"

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
private:

private:
	// �v���C���[�̃|�C���^
	std::shared_ptr<Player>pPlayer;

	//�e�X�g�p
	MelLib::ModelObject obj;

	//MelLib::ModelObject stage;
	std::shared_ptr<Stage>stage;

	// ���b�N�I���p
	static std::vector<std::shared_ptr<MelLib::GameObject>>pEnemys;
private:
	void LoadResources();
	void Fade();
public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��

	static const std::vector<std::shared_ptr<MelLib::GameObject>>& GetEnemys() { return pEnemys; }
};