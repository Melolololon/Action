#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>
#include<FrameTimer.h>

#include"Tutorial.h"

//�e�X�g�p
#include<ModelObject.h>


#include"Enemy.h"

#include"Stage.h"

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
private:

private:
	enum class EventType
	{
		NONE,
		TUTORIAL,
		BOSS_MOVIE,
	};
	EventType currentEvent = EventType::TUTORIAL;
	std::vector<EventType>endEvents;

	// �v���C���[�̃|�C���^
	std::shared_ptr<Player>pPlayer;

	//�e�X�g�p
	MelLib::ModelObject obj;

	Tutorial tutorial;

	// �G�̔z��(���b�N�I���Ȃǂɗ��p)
	static std::vector<std::shared_ptr<Enemy>>pEnemys;
	std::vector<std::shared_ptr<Enemy>>pDeadEnemys;

	// �G��|���Ȃ��Ə����Ȃ���
	std::vector<std::shared_ptr<MelLib::GameObject>>pWalls;

	// �`���[�g���A���J�n����܂ł̎��Ԃ��v������^�C�}�[
	MelLib::FrameTimer tutorialStartTimer;
private:
	void LoadResources();
	
	/// <summary>
	/// �V�[���J�ڎ��̃t�F�[�h�����Ăяo��
	/// </summary>
	void Fade();

	/// <summary>
	/// �C�x���g�֌W�̏���
	/// </summary>
	void Event();

	/// <summary>
	/// �C�x���g���I���ς݂��ǂ����̊m�F�BendEvents�����ׂďI�����Ă�����true
	/// </summary>
	/// <param name="checkEvents">�I���������ǂ����m�F����C�x���g</param>
	/// <returns></returns>
	bool CheckEndEvent(const std::vector<EventType>& checkEvents);

	bool CheckEndEvent(const EventType checkEvent);
public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��

	static const std::vector<std::shared_ptr<Enemy>>& GetEnemys() { return pEnemys; }
};