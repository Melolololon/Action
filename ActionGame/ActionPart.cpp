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

// テスト用
#include"SlushEffect.h"

std::vector<std::shared_ptr<MelLib::GameObject>>ActionPart::pEnemys;

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
	// フェードしてなかったら終了確認
	if (!Fade::GetInstance()->GetIsFade())
	{
		// ポーズから終了したら入る
		if (Pause::GetInstance()->GetIsEnd() || Pause::GetInstance()->GetIsReStart())
		{
			Fade::GetInstance()->Start();
		}
	}

	Fade::GetInstance()->Update();

	// フェードが終了したら終了
	if (Fade::GetInstance()->GetChangeSceneFlag())
	{
		isEnd = true;
	}
}

void ActionPart::Event()
{
	// イベントフラグに触れたら進むイベントの管理
	if(pPlayer->GetHitEventFlag())
	{
		switch (currentEvent)
		{
		case EventType::TUTORIAL:
			
			if(tutorial.TutorialEnd())
			{
				// チュートリアル終了
				endEvents.push_back(currentEvent);
				currentEvent = EventType::NONE;
			}
			else
			{
				// 次に進める
				tutorial.NextTutorial();
			}
			
			break;


		case EventType::NONE:
			
			// チュートリアルが終わっていてイベントに触れたら次のイベントへ
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

	// ステージを追加
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Stage>(1));
	/*stage.Create(MelLib::ModelData::Get("stage"),nullptr);
	stage.SetPosition(MelLib::Vector3(0,10,0));
	stage.SetScale(MelLib::Vector3(8, 8, 8));
	stage.SetAngle(MelLib::Vector3(0, 180, 0));*/

	

	// ファイルがなかったら必要最低限のものだけ用意
	// プレイヤーを必ず最初に追加するために、elseで追加処理を分ける
	if (!EditMode::GetInstance()->Load(pPlayer,&pEnemys))
	{

	/*	MelLib::GameObjectManager::GetInstance()->AddObject
		(std::make_shared<Ground>(0, MelLib::Vector3(90, 0, 0), 10));*/
	}
	else
	{
	}
	EditMode::GetInstance()->SetPPlayer(pPlayer.get());

	Enemy::SetPPlayer(pPlayer.get());
	EnemyAttack::SetPPlayer(pPlayer.get());
	NormalEnemyAttack::SetPPlayer(pPlayer.get());
	StageObject::SetPPlayer(pPlayer.get());

	HPGauge::SetPPlayer(pPlayer.get());

	//経路探索用
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
		MelLib::Vector3(-50, -500, -50),// Yどっちも0にするとノードサイズが0になって判定おかしくなるから注意
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


	// UI追加
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<HPGauge>());


	

}

void ActionPart::Update()
{

	EditMode::GetInstance()->Update();
	Pause::GetInstance()->Update();

	
	if (currentEvent == EventType::TUTORIAL)tutorial.Update();
	
	// チュートリアルを進める
	if (pPlayer->GetHitTutorialEventFlag())tutorial.NextTutorial();
	
	MelLib::GameObjectManager::GetInstance()->Update();

	for(int i = 0; i < pEnemys.size();i++)
	{
		// 倒した敵は敵リストから消す
		if(pEnemys[i]->GetEraseManager())
		{
			// ロックしてる敵を倒したら、ロック解除
			if(pPlayer->GetPLockOnEnemy() == pEnemys[i].get())
			{
				pPlayer->LockOnEnd();
			}

			pEnemys.erase(pEnemys.begin() + i);
			i--;
		}
	}

	// フェードが関わる処理
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
	// 全消しして問題ない場合は、コメントアウト解除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();

	// 例外防止(これ消すと例外出る)EditModeにポインタ渡してるからだと思われる
	pEnemys.clear();
}

MelLib::Scene* ActionPart::GetNextScene()
{
	if(Pause::GetInstance()->GetIsEnd()) return new Title();
	else if (Pause::GetInstance()->GetIsReStart()) return new ActionPart();
}
