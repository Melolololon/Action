#include "ActionPart.h"

#include"DirectionalLight.h"
#include<TextWrite.h>

#include"GameObjectManager.h"
#include"Input.h"

#include"Player.h"
#include"HPGauge.h"

#include"Enemy.h"
#include"EnemyAttack.h"
#include"NormalEnemyAttack.h"
#include"StageObject.h"

#include"NoemalEnemy.h"
#include"JumpEnemy.h"

#include"Ground.h"

#include"Rock.h"

#include"Fade.h"
#include"Option.h"
#include"Pause.h"
#include"EditMode.h"

#include"Title.h"

// �e�X�g�p
#include"SlushEffect.h"

std::vector<std::shared_ptr<Enemy>>ActionPart::pEnemys;

void ActionPart::LoadResources()
{
	Ground::LoadResource();

	NoemalEnemy::LoadResource();
	JumpEnemy::LoadResources();

	Rock::LoadResources();
	Stage::LoadResources(1);

	MelLib::ModelData::Load("Resources/Model/Stage/Stage.obj",true,"stage");

}

void ActionPart::Fade()
{
	// �t�F�[�h���ĂȂ�������I���m�F
	if (!Fade::GetInstance()->GetIsFade())
	{
		// �|�[�Y����I�����������
		if (Pause::GetInstance()->GetIsEnd() || Pause::GetInstance()->GetIsReStart())
		{
			Fade::GetInstance()->Start();
		}
	}

	Fade::GetInstance()->Update();

	// �t�F�[�h���I��������I��
	if (Fade::GetInstance()->GetChangeSceneFlag())
	{
		isEnd = true;
	}
}

void ActionPart::Event()
{
	// �C�x���g�t���O�ɐG�ꂽ��i�ރC�x���g�̊Ǘ�
	if(pPlayer->GetHitEventFlag())
	{
		switch (currentEvent)
		{
		case EventType::TUTORIAL:
			
			if(tutorial.GetTutorialEndFlag())
			{
				// �`���[�g���A���I��
				endEvents.push_back(currentEvent);
				currentEvent = EventType::NONE;
			}
			else
			{
				// ���ɐi�߂�
				tutorial.NextTutorial();
			}
			
			break;


		case EventType::NONE:
			
			// �`���[�g���A�����I����Ă��ăC�x���g�ɐG�ꂽ�玟�̃C�x���g��
			if(CheckEndEvent(EventType::TUTORIAL))
			{
				currentEvent = EventType::BOSS_MOVIE;
			}

			break;
		}
	}
}

bool ActionPart::CheckEndEvent(const std::vector<EventType>& checkEvents)
{
	int count = 0;
	for(const auto end : endEvents)
	{
		for (const auto check: checkEvents)
		{
			if (end == check)count++;
			if (count == checkEvents.size())return true;
		}
	}
	return false;
}

bool ActionPart::CheckEndEvent(const EventType checkEvent)
{
	for (const auto end : endEvents)
	{
		if (end == checkEvent)return true;
	}
	return false;
}


void ActionPart::Initialize()
{
	LoadResources();

	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(20, 0, 0));

	// �X�e�[�W��ǉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Stage>(1));
	/*stage.Create(MelLib::ModelData::Get("stage"),nullptr);
	stage.SetPosition(MelLib::Vector3(0,10,0));
	stage.SetScale(MelLib::Vector3(8, 8, 8));
	stage.SetAngle(MelLib::Vector3(0, 180, 0));*/

	
	// �G�f�B�b�g�f�[�^�ǂݍ���
	EditMode::GetInstance()->Load(pPlayer, &pEnemys, &pWalls);

	EditMode::GetInstance()->SetPPlayer(pPlayer.get());
	Enemy::SetPPlayer(pPlayer.get());
	EnemyAttack::SetPPlayer(pPlayer.get());
	NormalEnemyAttack::SetPPlayer(pPlayer.get());
	StageObject::SetPPlayer(pPlayer.get());

	HPGauge::SetPPlayer(pPlayer.get());

	//�o�H�T���p
	std::vector<MelLib::BoxData>bData;
	/*bData.resize(1);
	bData[0].SetPosition(MelLib::Vector3(0, 0, 10));
	bData[0].SetSize(MelLib::Vector3(50, 20, 2));
	obj.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	obj.SetPosition(bData[0].GetPosition());
	obj.SetScale(bData[0].GetSize());*/

	std::vector<UINT>costs;

	Enemy::SetAStarNodeDatas
	(
		MelLib::Vector3(-50, -500, -50),// Y�ǂ�����0�ɂ���ƃm�[�h�T�C�Y��0�ɂȂ��Ĕ��肨�������Ȃ邩�璍��
		MelLib::Vector3(50, 500, 50),
		MelLib::Value3<UINT>(50, 1, 50),
		bData,
		costs
	);


	/*MelLib::GameObjectManager::GetInstance()->AddObject
	(std::make_shared<Ground>(0, MelLib::Vector3(90, 0, 0), 100));
	MelLib::GameObjectManager::GetInstance()->AddObject
	(std::make_shared<Ground>(MelLib::Vector3(0, 0, 50), MelLib::Vector3(45, 0, 0), 100));*/

	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<NoemalEnemy>(MelLib::Vector3(10,0,30)));
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<NoemalEnemy>(MelLib::Vector3(0,0,30)));
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<NoemalEnemy>(MelLib::Vector3(-10,0,30)));


	Pause::GetInstance()->Initialize();


	// UI�ǉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());


	tutorialStartTimer.SetMaxTime(60 * 2);
	tutorialStartTimer.SetStopFlag(false);
}

void ActionPart::Update()
{
	if (currentEvent == EventType::TUTORIAL)tutorial.Update();
	if (tutorial.GetDrawWindow())return;

	EditMode::GetInstance()->Update();
	Pause::GetInstance()->Update();
	
	// �v���C���[���t���O�ɐG��邩���Ԃ𒴂�����`���[�g���A���J�n
	bool nextTutorial = tutorialStartTimer.GetMaxOverFlag()
		|| pPlayer->GetHitTutorialEventFlag();
	if (nextTutorial)tutorial.NextTutorial();

	// �^�C�}�[���~�߂�
	if (tutorialStartTimer.GetMaxOverFlag())
	{
		tutorialStartTimer.SetStopFlag(true);
		tutorialStartTimer.ResetTimeZero();
	}

	MelLib::GameObjectManager::GetInstance()->Update();


	// <�ǔԍ�,�J�E���g>
	std::unordered_map<int, int>enemyWallNum;

	// �G�̂���m�F&�ǂ��������߂̔ԍ��擾
	for(int i = 0; i < pEnemys.size();i++)
	{
		// �|�����G�͓G���X�g�������
		if(pEnemys[i]->GetEraseManager())
		{
			// ���b�N���Ă�G��|������A���b�N����
			if(pPlayer->GetPLockOnEnemy() == pEnemys[i].get())
			{
				pPlayer->LockOnEnd();
			}

			pDeadEnemys.push_back(pEnemys[i]);
			pEnemys.erase(pEnemys.begin() + i);
			i--;

		}
		else
		{
			enemyWallNum[pEnemys[i]->GetWallNum()]++;
		}
	}

	// �ǂ̍폜�m�F
	for(const auto& enemyWallNum : enemyWallNum)
	{
		// �ǂɑΉ����Ă�G�����Ȃ�������ǂ�����
		if(enemyWallNum.second == 0)
		{
			// �����I�[�o�[���Ă����΂�
			if (enemyWallNum.first >= pWalls.size())continue;

			// �Ǘ��N���X�������
			pWalls[enemyWallNum.first]->TrueEraseManager();
		}
	}

	// �t�F�[�h���ւ�鏈��
	Fade();
}

void ActionPart::Draw()
{
	EditMode::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();

	if (currentEvent == EventType::TUTORIAL
		&& !Pause::GetInstance()->GetIsPause())tutorial.Draw();

	Pause::GetInstance()->Draw();
	Fade::GetInstance()->Draw();

}

void ActionPart::Finalize()
{
	// �S�������Ė��Ȃ��ꍇ�́A�R�����g�A�E�g����
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();

	// ��O�h�~(��������Ɨ�O�o��)EditMode�Ƀ|�C���^�n���Ă邩�炾�Ǝv����
	pEnemys.clear();
}

MelLib::Scene* ActionPart::GetNextScene()
{
	if(Pause::GetInstance()->GetIsEnd()) return new Title();
	else if (Pause::GetInstance()->GetIsReStart()) return new ActionPart();
}
