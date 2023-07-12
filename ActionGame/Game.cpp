#include "Game.h"
#include"Library.h"

#include"ModelData.h"
#include"ModelObject.h"
#include"ImguiManager.h"
#include<TextWrite.h>

// �I�u�W�F�N�g
#include"GameObject.h"
#include"GameObjectManager.h"
#include"Player.h"
#include"SlushEffect.h"
#include"HPGauge.h"
#include"Wall.h"
#include"Boss.h"
#include"BossAttack.h"
#include"Water.h"
#include"EnemyDamageParticle.h"
#include"Stage.h"

#include"ItemEffect.h"
#include"RecoveryItem.h"
#include"RecoveryEffect.h"

#include"Wall.h"

#include"EnemySpaunPoint.h"
#include"WeakEnemy.h"

#include"ClearSprite.h"
#include"GameOverSprite.h"

#include"Pause.h"

// �V�[��
#include"SceneManager.h"
#include"SceneEditer.h"
#include"Fade.h"
#include"ActionPart.h"
#include"Title.h"
#include"TestScene.h"
#include"GamePlay.h"
#include"SandEffect.h"

#include"Option.h"
#include"Tutorial.h"

#include"EnemyDarknessEffect.h"

#include"DirectionalLight.h"

#include"LoadResources.h"

Game* Game::GetInstance()
{
	static Game game;
	return &game;
}

void Game::Run()
{
	Initialize();

	while (true)
	{
		MelLib::Library::LoopStartProcess();	

		Update(); 
		Draw();

		if (MelLib::Library::GetIsEnd())break;
	
		MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

void Game::Initialize()
{
	MelLib::Library::Initialize(1920, 1080, MelLib::Color(0, 170, 255, 255), L"�a�����o");
	MelLib::Library::SetFramesPerSecond60(true);

	//�d�͉����x��ݒ�
	MelLib::GameObject::SetGravutationalAcceleration(0.17f);


	// �ǂݍ��� �G�f�B�^�[�̒ǉ�������Ȋ����ŃN���X�ł������C���N���[�h���点�Ă������肷�邩��
	LoadResources::GetInstance()->Load();

	Pause::GetInstance()->Initialize();
	

	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(EDITER_FLAG);
	MelLib::SceneEditer::GetInstance()->SetEditerFlag(EDITER_FLAG);
	MelLib::SceneEditer::GetInstance()->SetReleaseEditFlag(EDITER_FLAG);

#pragma region �G�f�B�^�[�o�^
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Player>(), "Player");
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Wall>(), "Wall");


	// �X�e�[�W�o�^
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Stage>(0), "Stage");
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Water>(), "Stage");

	// �G�ǉ�
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<EnemySpaunPoint>(typeid(WeakEnemy).name()), "Enemy");
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Boss>(), "Boss");
#pragma endregion

	if (EDITER_FLAG) 
	{
		MelLib::SceneManager::GetInstance()->SetStartScene(new TestScene());
	}
	else 
	{
		MelLib::SceneManager::GetInstance()->SetStartScene(new Title());
	}
	
	Fade::GetInstance()->Initializ();
	Option::GetInstance()->Initialize();
	

	MelLib::TextWrite::CreateFontData(/*L"HGP�޼��E"*/L"Arial",64.0f, "Arial");

	MelLib::Camera::Get()->SetFar(FLT_MAX);

	MelLib::SceneEditer::GetInstance()->Initialize();

	// �A�C�e���ǉ�
	//MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<RecoveryItem>(), "H_Item");

	// ���ꌴ����DX12�̃G���[���o��
	// �����[�X����
	// �G���[������
	// ���ꂭ�邭��񂹂Ή_������
	pSky = std::make_unique<Sky>();


	// ���C�g�쐬
	MelLib::DirectionalLight::Create("Ligth1");
	MelLib::DirectionalLight::Get("Ligth1").SetDirection(MelLib::Vector3(1, 0, 0));

	MelLib::DirectionalLight::Create("Ligth2");
	MelLib::DirectionalLight::Get("Ligth2").SetDirection(MelLib::Vector3(-1, 0, 0));

	MelLib::DirectionalLight::Create("Ligth3");
	MelLib::DirectionalLight::Get("Ligth3").SetDirection(MelLib::Vector3(0, 0, -1));

	MelLib::DirectionalLight::Create("Ligth4");
	MelLib::DirectionalLight::Get("Ligth4").SetDirection(MelLib::Vector3(0, -0.75f, 0));
}

void Game::Finalize()
{
	MelLib::Library::Finalize();
	delete pSky.release();
}

void Game::Update()
{
	MelLib::SceneManager::GetInstance()->Update();
	MelLib::SceneEditer::GetInstance()->Update();
}

void Game::Draw()
{
	pSky->Draw();
	MelLib::SceneManager::GetInstance()->Draw();
	MelLib::SceneEditer::GetInstance()->Draw();

}

std::string Game::GetPath(const ResourcePath type)const
{
	switch (type)
	{
	case ResourcePath::TEXTURE:
		return texturePath;
		break;
	}
}
