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

#include"Game.h"

// テスト用
#include"SlushEffect.h"

std::vector<std::shared_ptr<Enemy>>ActionPart::pEnemys;

bool ActionPart::loadTexture = false;

void ActionPart::LoadResources()
{
	Ground::LoadResource();

	NoemalEnemy::LoadResource();
	JumpEnemy::LoadResources();

	Rock::LoadResources();
	Stage::LoadResources(1);

	//MelLib::ModelData::Load("Resources/Model/Stage/Stage.obj",true,"stage");

	if(!loadTexture)
	{
		const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
		MelLib::Texture::Load(TEXTURE_PATH + "ActionPart/GameOver.png","gameover");
		MelLib::Texture::Load(TEXTURE_PATH + "ActionPart/retry.png", "retry");
		MelLib::Texture::Load(TEXTURE_PATH + "ActionPart/returnTitle.png", "retTitle");
		MelLib::Texture::Load(TEXTURE_PATH + "ActionPart/Clear.png", "clear");
		loadTexture = true;
	}
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
			
			if(tutorial.GetTutorialEndFlag())
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

void ActionPart::GameClear()
{
	float scale = gameClearTextScale.EaseIn();
	if (scale < 1)scale = 1;
	gameClearTextSpr.SetScale(scale);
}

void ActionPart::GameOver()
{
	float alpha = gameoverBackAlpha.Lerp();
	if (alpha > 100)alpha = 100;
	gameoverBackSpr.SetAddColor(MelLib::Color(0,0,0,MelLib::Color::ParToUChar(alpha)));

	gameoverTextAlpha--;
	if (gameoverTextAlpha < 0)gameoverTextAlpha = 0;
	gameoverTextSpr.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(gameoverTextAlpha)));


	if (alpha >= 100)
	{
		if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::UP)
			|| MelLib::Input::LeftStickUp(50.0f))returnTitleFlag = false;

		if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::DOWN)
			|| MelLib::Input::LeftStickDown(50.0f))returnTitleFlag = true;

		if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))Fade::GetInstance()->Start();

		const MelLib::Vector2 SELECT_SCALE(1.7f);
		if (!returnTitleFlag)
		{
			gameoverRetrySpr.SetScale(SELECT_SCALE);
			gameoverRetrySpr.SetAddColor(MelLib::Color(80, 80, 80, 0));
			gameoverReturnTitleSpr.SetScale(1);
			gameoverReturnTitleSpr.SetAddColor(MelLib::Color(0,0));
		}
		else
		{
			gameoverRetrySpr.SetScale(1);
			gameoverRetrySpr.SetAddColor(MelLib::Color(0, 0));
			gameoverReturnTitleSpr.SetScale(SELECT_SCALE);
			gameoverReturnTitleSpr.SetAddColor(MelLib::Color(80, 80, 80, 0));
		}
	}

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

	
	// エディットデータ読み込み
	EditMode::GetInstance()->Load(pPlayer, &pEnemys, &pWalls);

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

	// チュートリアルが開始するまでの時間
	tutorialStartTimer.SetMaxTime(60 * 2);
	tutorialStartTimer.SetStopFlag(false);

	gameClearTimer.SetMaxTime(60 * 3);

	// 各スプライト準備
	gameoverBackSpr.Create(MelLib::Color(0, 0));
	gameoverBackSpr.SetScale(MelLib::Vector2(MelLib::Library::GetWindowWidth(), MelLib::Library::GetWindowHeight()));
	gameoverTextSpr.Create(MelLib::Texture::Get("gameover"));
	gameoverTextSpr.SetPosition(MelLib::Vector2(310,100));
	gameoverTextSpr.SetSubColor(MelLib::Color(0, 0, 0, 255));

	gameoverRetrySpr.Create(MelLib::Texture::Get("retry"));
	gameoverRetrySpr.SetPosition(MelLib::Vector2(450,420));
	gameoverRetrySpr.SetScalingPoint(MelLib::Texture::Get("retry")->GetTextureSize() /2);

	gameoverReturnTitleSpr.Create(MelLib::Texture::Get("retTitle"));
	gameoverReturnTitleSpr.SetPosition(MelLib::Vector2(400, 550));
	gameoverReturnTitleSpr.SetScalingPoint(MelLib::Texture::Get("retTitle")->GetTextureSize() / 2);

	gameClearTextSpr.Create(MelLib::Texture::Get("clear"));
	gameClearTextSpr.SetScalingPoint(MelLib::Texture::Get("clear")->GetTextureSize()/ 2);
	gameClearTextSpr.SetScale(5);
	gameClearTextSpr.SetPosition(MelLib::Vector2(350,200));
}

void ActionPart::Update()
{
	// フェードが関わる処理
	Fade();

	// ゲームオーバー処理
	if(currentState == GameState::GAMEOVER)
	{
		GameOver();
		return;
	}
	
#ifdef _DEBUG
	if (MelLib::Input::KeyTrigger(DIK_Z))currentState = GameState::CLEAR;
#endif // _DEBUG


	// クリア処理
	if(currentState == GameState::CLEAR)
	{
		if (gameClearTimer.GetNowTime() >= CLEAR_ANIMATION_START_TIME)
		{
			GameClear();
		}

		if (gameClearTimer.GetMaxOverFlag())
		{
			returnTitleFlag = true;
			Fade::GetInstance()->Start();
		}
		return;
	}

	if (currentEvent == EventType::TUTORIAL)tutorial.Update();
	if (tutorial.GetDrawWindow())return;

	if (currentState != GameState::NOT_PLAYED) 
	{
		EditMode::GetInstance()->Update();
		Pause::GetInstance()->Update();
	}
	
	// プレイヤーがフラグに触れるか時間を超えたらチュートリアル開始
	bool nextTutorial = tutorialStartTimer.GetMaxOverFlag()
		|| pPlayer->GetHitTutorialEventFlag();
	if (nextTutorial)tutorial.NextTutorial();

	// タイマーを止める
	if (tutorialStartTimer.GetMaxOverFlag())
	{
		tutorialStartTimer.SetStopFlag(true);
		tutorialStartTimer.ResetTimeZero();
		currentState = GameState::PLAY;
	}

	MelLib::GameObjectManager::GetInstance()->Update();


	// <壁番号,カウント>
	std::unordered_map<int, int>enemyWallNum;
	for (int i = 0; i < pWalls.size(); i++)enemyWallNum[i] = 0;

	// 敵のやられ確認&壁を消すための番号取得
	for(int i = 0; i < pEnemys.size();i++)
	{
		// 倒した敵は敵リストから消す
		if(pEnemys[i]->GetEraseManager())
		{
			bool continueLoop = false;
			// すでに死んでる敵の処理は飛ばす
			for (const auto& deadEnemy : pDeadEnemys)
			{
				if (deadEnemy == pEnemys[i])
				{
					continueLoop = true;
					continue;
				}
			}
			if (continueLoop)continue;


			// ロックしてる敵を倒したら、ロック解除
			if(pPlayer->GetPLockOnEnemy() == pEnemys[i].get())
			{
				pPlayer->LockOnEnd();
			}

			pDeadEnemys.push_back(pEnemys[i]);
			//pEnemys.erase(pEnemys.begin() + i);
			//i--;

		}
		else
		{
			enemyWallNum[pEnemys[i]->GetWallNum()]++;
		}
	}

	// 壁の確認
	for(const auto& enemyWallNum : enemyWallNum)
	{
		// 壁に対応してる敵がいなかったら壁を消す
		if(enemyWallNum.second == 0)
		{
			// もしオーバーしてたら飛ばす
			if (enemyWallNum.first >= pWalls.size())continue;

			// 管理クラスから消す
			pWalls[enemyWallNum.first]->TrueEraseManager();
		}
	}


	// ゲームオーバー判定
	if(pPlayer->DeadAnimationEnd())
	{
		currentState = GameState::GAMEOVER;
	}

	// クリア判定
	if(pDeadEnemys.size() == pEnemys.size())
	{
		currentState = GameState::CLEAR;
		gameClearTimer.SetStopFlag(false);
	}

}

void ActionPart::Draw()
{
	EditMode::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();

	if (currentEvent == EventType::TUTORIAL
		&& !Pause::GetInstance()->GetIsPause()
		&& currentState == GameState::PLAY)tutorial.Draw();


	if (currentState == GameState::GAMEOVER) 
	{
		gameoverBackSpr.Draw();
		gameoverTextSpr.Draw();
		if(gameoverBackAlpha.GetValue() >= 100)
		{
			gameoverRetrySpr.Draw();
			gameoverReturnTitleSpr.Draw();
		}
	}

	if(gameClearTimer.GetNowTime() >= CLEAR_ANIMATION_START_TIME)
	{
		gameClearTextSpr.Draw();
	}



	Pause::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
}

void ActionPart::Finalize()
{
	// 全消しして問題ない場合は、コメントアウト解除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();

	// 例外防止(これ消すと例外出る)EditModeにポインタ渡してるからだと思われる
	pEnemys.clear();
	pWalls.clear();
}

MelLib::Scene* ActionPart::GetNextScene()
{
	if(Pause::GetInstance()->GetIsEnd()
		|| returnTitleFlag) return new Title();

	return new ActionPart();

}
