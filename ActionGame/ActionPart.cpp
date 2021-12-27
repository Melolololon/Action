#include "ActionPart.h"

#include"DirectionalLight.h"
#include<TextWrite.h>

#include"GameObjectManager.h"
#include"Input.h"

#include"Player.h"

#include"Enemy.h"
#include"EnemyAttack.h"
#include"StageObject.h"

#include"NoemalEnemy.h"

#include"Ground.h"

#include"Fade.h"
#include"Option.h"
#include"Pause.h"
#include"EditMode.h"

std::vector<std::shared_ptr<Enemy>>ActionPart::pEnemys;

void ActionPart::LoadResources()
{
	Ground::LoadResource();
	NoemalEnemy::LoadResource();
}

void ActionPart::Initialize()
{
	LoadResources();

	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(20, 0, 0));

	// プレイヤーのポインタ
	std::shared_ptr<Player>pPlayer;

	// ファイルがなかったら必要最低限のものだけ用意
	// プレイヤーを必ず最初に追加するために、elseで追加処理を分ける
	if (!EditMode::GetInstance()->Load(pPlayer,&pEnemys))
	{
		pPlayer = std::make_shared<Player>(MelLib::Vector3(0, 5, 0));
		MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);

		MelLib::GameObjectManager::GetInstance()->AddObject
		(std::make_shared<Ground>(0, MelLib::Vector3(90, 0, 0), 10));
	}
	else
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);
	}
	EditMode::GetInstance()->SetPPlayer(pPlayer.get());

	Enemy::SetPPlayer(pPlayer.get());
	EnemyAttack::SetPPlayer(pPlayer.get());
	StageObject::SetPPlayer(pPlayer.get());

	//経路探索用
	std::vector<MelLib::BoxData>bData;
	bData.resize(1);
	bData[0].SetPosition(MelLib::Vector3(0, 0, 10));
	bData[0].SetSize(MelLib::Vector3(50, 20, 2));
	obj.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	obj.SetPosition(bData[0].GetPosition());
	obj.SetScale(bData[0].GetSize());

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
}

void ActionPart::Update()
{
	// このシーンにて開始時に急激なメモリ確保あり
	// 17フレーム目で確保されてるから、16で確保されてる可能性あり
	// DrectWriteのせいでした。仕様。
	// 2021 12/17


	/*if (MelLib::Input::KeyTrigger(DIK_F5))isEdit = isEdit == false ? true : false;
	if(editOn && isEdit) EditMode::GetInstance()->Update();*/
	EditMode::GetInstance()->Update();
	Pause::GetInstance()->Update();



	MelLib::GameObjectManager::GetInstance()->Update();

	// デバッグ用
	if (MelLib::Input::KeyTrigger(DIK_ESCAPE) && !Fade::GetInstance()->GetIsFade())
	{
		Fade::GetInstance()->Start();
	}

	// && の左にシーン切り替え条件を記述
	//if ( && !Fade::GetInstance()->GetIsFade())
	//{
	//	Fade::GetInstance()->Start();
	//}

	Fade::GetInstance()->Update();

	// フェードが終了したら切り替え
	if (Fade::GetInstance()->GetChangeSceneFlag())
	{
		isEnd = true;

		
		MelLib::GameObjectManager::GetInstance()->AllEraseObject();
	}
}

void ActionPart::Draw()
{
	//if (editOn && isEdit)  EditMode::GetInstance()->Draw();
	EditMode::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();
	Pause::GetInstance()->Draw();
	Fade::GetInstance()->Draw();

	//obj.Draw();


	// テスト
	MelLib::TextWrite::Draw
	(MelLib::Vector2(0,670),MelLib::Color(255,255,255,255),L"メニューボタン　メニューを開く","test");
}

void ActionPart::Finalize()
{
	// 全消しして問題ない場合は、コメントアウト解除
	//MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
