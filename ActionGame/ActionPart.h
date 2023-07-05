//#pragma once
//#include"Scene.h"
//
//#include<Easing.h>
//#include<Sprite2D.h>
//#include<FrameTimer.h>
//
//#include"Tutorial.h"
//
////�e�X�g�p
//#include<ModelObject.h>
//
//
//#include"Enemy.h"
//
//#include"Stage.h"
//
////�A�N�V�����p�[�g�̃V�[��
//class ActionPart :public MelLib::Scene
//{
//private:
//
//private:
//	enum class GameState 
//	{
//		NOT_PLAYED,
//		PLAY,
//		CLEAR,
//		GAMEOVER,
//	};
//	GameState currentState = GameState::NOT_PLAYED;
//	
//	enum class EventType
//	{
//		NONE,
//		TUTORIAL,
//		BOSS_MOVIE,
//	};
//	EventType currentEvent = EventType::TUTORIAL;
//	std::vector<EventType>endEvents;
//
//	// �v���C���[�̃|�C���^
//	std::shared_ptr<Player>pPlayer;
//
//	//�e�X�g�p
//	MelLib::ModelObject obj;
//
//	Tutorial tutorial;
//
//	// �G�̔z��(���b�N�I���Ȃǂɗ��p)
//	static std::vector<std::shared_ptr<Enemy>>pEnemys;
//	std::vector<std::shared_ptr<Enemy>>pDeadEnemys;
//
//	// �G��|���Ȃ��Ə����Ȃ���
//	std::vector<std::shared_ptr<MelLib::GameObject>>pWalls;
//
//	// �`���[�g���A���J�n����܂ł̎��Ԃ��v������^�C�}�[
//	MelLib::FrameTimer tutorialStartTimer;
//
//	// �Q�[���N���A����ɂȂ��Ă���ǂ̂��炢�ŏI�����邩�̎��Ԃ��v������^�C�}�[
//	MelLib::FrameTimer gameClearTimer;
//	// �N���A�̃A�j���[�V�����̊J�n����
//	const int CLEAR_ANIMATION_START_TIME = 60 * 1;
//
//	MelLib::Sprite2D gameoverBackSpr;
//	MelLib::Sprite2D gameoverTextSpr;
//	MelLib::Easing<float> gameoverBackAlpha = MelLib::Easing<float>(0, 100, 1, 0);
//	float gameoverTextAlpha = 100;
//
//	MelLib::Sprite2D gameoverRetrySpr;
//	MelLib::Sprite2D gameoverReturnTitleSpr;
//
//
//	MelLib::Sprite2D gameClearTextSpr;
//	MelLib::Easing<float>gameClearTextScale = MelLib::Easing<float>(20, 1, 3, 20);
//
//	bool returnTitleFlag = false;
//
//	static bool loadTexture;
//private:
//	void LoadResources();
//	
//	/// <summary>
//	/// �V�[���J�ڎ��̃t�F�[�h�����Ăяo��
//	/// </summary>
//	void Fade();
//
//	/// <summary>
//	/// �C�x���g�֌W�̏���
//	/// </summary>
//	void Event();
//
//	/// <summary>
//	/// �C�x���g���I���ς݂��ǂ����̊m�F�BendEvents�����ׂďI�����Ă�����true
//	/// </summary>
//	/// <param name="checkEvents">�I���������ǂ����m�F����C�x���g</param>
//	/// <returns></returns>
//	bool CheckEndEvent(const std::vector<EventType>& checkEvents);
//
//	bool CheckEndEvent(const EventType checkEvent);
//
//	void GameClear();
//	void GameOver();
//public:
//	void Initialize()override;//������
//	void Update()override;
//	void Draw()override;
//	void Finalize()override;//�I������
//	Scene* GetNextScene()override;//���̃V�[��
//
//	static const std::vector<std::shared_ptr<Enemy>>& GetEnemys() { return pEnemys; }
//};