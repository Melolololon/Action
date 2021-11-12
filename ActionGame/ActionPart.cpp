#include "ActionPart.h"

#include"DirectionalLight.h"

#include"GameObjectManager.h"
#include"Input.h"

#include"Player.h"

#include"Enemy.h"
#include"StageObject.h"


#include"Ground.h"

#include"Option.h"
#include"Pause.h"
#include"EditMode.h"


void ActionPart::LoadResources()
{
	Ground::LoadResource();


}

void ActionPart::Initialize()
{
	LoadResources();

	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(20, 0, 0));

	// �v���C���[�̃|�C���^
	std::shared_ptr<Player>p;

	// �t�@�C�����Ȃ�������K�v�Œ���̂��̂����p��
	// �v���C���[��K���ŏ��ɒǉ����邽�߂ɁAelse�Œǉ������𕪂���
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

	Enemy::SetPPlayer(p.get());
	StageObject::SetPPlayer(p.get());

	//�o�H�T���p
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
}

void ActionPart::Update()
{
	/*if (MelLib::Input::KeyTrigger(DIK_F5))isEdit = isEdit == false ? true : false;
	if(editOn && isEdit) EditMode::GetInstance()->Update();*/
	EditMode::GetInstance()->Update();
	Pause::GetInstance()->Update();



	MelLib::GameObjectManager::GetInstance()->Update();

	// �f�o�b�O�p
	if (MelLib::Input::KeyTrigger(DIK_ESCAPE))
	{
		MelLib::GameObjectManager::GetInstance()->AllEraseObject();
		isEnd = true;
	}
}

void ActionPart::Draw()
{
	//if (editOn && isEdit)  EditMode::GetInstance()->Draw();
	EditMode::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();
	Pause::GetInstance()->Draw();

	obj.Draw();
}

void ActionPart::Finalize()
{
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
