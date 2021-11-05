#include "ActionPart.h"

#include"DirectionalLight.h"

#include"GameObjectManager.h"
#include"Input.h"

#include"Player.h"
#include"NoemalEnemy.h"

#include"Ground.h"

#include"Option.h"
#include"EditMode.h"

bool ActionPart::isPause;
bool ActionPart::isEdit;

void ActionPart::LoadResources()
{
	Ground::LoadResource();

	pauseBackSpr.Create(MelLib::Color(0, 0, 0, 255));
	pauseBackSpr.SetScale(MelLib::Vector2(1280, 720));


}

// ポーズの暗くなるやつとか文字出すのクラス化する?
void ActionPart::PauseUpdate()
{

	static const float PAUSE_SUB_ALPHA_SPEED = 10.0f;
	//スプライトのアルファ値の減算値増減
	if (pauseEnd)
	{

		pauseSubAlpha.SetAddPar(PAUSE_SUB_ALPHA_SPEED);
		pauseBackSubAlpha.SetAddPar(PAUSE_SUB_ALPHA_SPEED);
		if(pauseSubAlpha.GetPar() < 100.0f)
		{
			pauseSubAlpha.Lerp();
			pauseBackSubAlpha.Lerp();
		}
		else
		{
			pauseSubAlpha.SetPar(100.0f);
			pauseBackSubAlpha.SetPar(100.0f);
			isPause = false;
			pauseEnd = false;
		}
	}
	else
	{

		pauseSubAlpha.SetAddPar(-PAUSE_SUB_ALPHA_SPEED);
		pauseBackSubAlpha.SetAddPar(-PAUSE_SUB_ALPHA_SPEED);
		if(pauseSubAlpha.GetPar() > 0.0f)
		{
			pauseSubAlpha.Lerp();
			pauseBackSubAlpha.Lerp();
		}
		else
		{
			pauseSubAlpha.SetPar(0.0f);
			pauseBackSubAlpha.SetPar(0.0f);
		}

	}

	//こいつだけ50.0fで止める
	//pauseBackSpr.SetSubColor(MelLib::Color(0,0,0, pauseBackSubAlpha2.GetValue()));
	pauseBackSpr.SetSubColor(MelLib::Color(0,0,0, MelLib::Color::ParToUChar(pauseBackSubAlpha.GetValue())));

	// 項目変更
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::UP)
		|| MelLib::Input::LeftStickUpTrigger(30.0f, 30.0f))
	{
		currentPauseSelect--;
		if (currentPauseSelect < 0)currentPauseSelect = PauseMenu::NUM_MAX;
	}
	else if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::DOWN)
		|| MelLib::Input::LeftStickDownTrigger(30.0f, 30.0f))
	{
		currentPauseSelect--;
		if (currentPauseSelect > PauseMenu::NUM_MAX)currentPauseSelect = 0;
	}


	// 選択
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))pushPauseSelect = currentPauseSelect;

	// 選択したものに応じて処理を行う
	switch (pushPauseSelect)
	{
	case PauseMenu::PAUSE_END:
		pauseEnd = true;
		break;

	case PauseMenu::RESTART:

		break;

	case PauseMenu::OPTION:
		break;

	case PauseMenu::RETURN_TITLE:
		break;
	}

	
}

void ActionPart::PauseDraw()
{
	pauseBackSpr.Draw();

	switch (pushPauseSelect)
	{
	case PauseMenu::PAUSE_END:
		break;

	case PauseMenu::RESTART:

		break;

	case PauseMenu::OPTION:
		break;

	case PauseMenu::RETURN_TITLE:
		break;
	}
}



void ActionPart::Initialize()
{
	LoadResources();

	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(20, 0, 0));

	// プレイヤーのポインタ
	std::shared_ptr<Player>p;

	// ファイルがなかったら必要最低限のものだけ用意
	// プレイヤーを必ず最初に追加するために、elseで追加処理を分ける
	if (!EditMode::GetInstance()->Load(p))
	{
		p = std::make_shared<Player>(MelLib::Vector3(0, 5, 0));
		MelLib::GameObjectManager::GetInstance()->AddObject(p);

		MelLib::GameObjectManager::GetInstance()->AddObject
		(std::make_shared<Ground>(0, MelLib::Vector3(90, 0, 0), 10));
	}
	else
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(p);
	}



	//経路探索用
	Enemy::SetPPlayer(p.get());
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

	pauseSubAlpha.SetStart(0.0f);
	pauseSubAlpha.SetEnd(100.0f);
	pauseSubAlpha.SetPar(100.0f);
	pauseBackSubAlpha.SetStart(70.0f);
	pauseBackSubAlpha.SetEnd(100.0f);
	pauseBackSubAlpha.SetPar(100.0f);

	
}

void ActionPart::Update()
{
	if (MelLib::Input::KeyTrigger(DIK_F5))isEdit = isEdit == false ? true : false;
	if(editOn && isEdit) EditMode::GetInstance()->Update();
	

	// ポーズ(仮)
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::START)&& !isPause)
	{
		isPause = true;
		pushPauseSelect = -1;
	}
	// ポーズ終了
	else if(MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START))
	{
		pauseEnd = true;
	}



	if (isPause)
	{
		PauseUpdate();
	}



	MelLib::GameObjectManager::GetInstance()->Update();

	// デバッグ用
	if (MelLib::Input::KeyTrigger(DIK_ESCAPE))
	{
		MelLib::GameObjectManager::GetInstance()->AllEraseObject();
		isEnd = true;
	}
}

void ActionPart::Draw()
{
	if (editOn && isEdit)  EditMode::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();
	if (isPause)
	{
		PauseDraw();
	}

	obj.Draw();
}

void ActionPart::Finalize()
{
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
