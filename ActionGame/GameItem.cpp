#include "GameItem.h"

#include"Stage.h"

Player* GameItem::pPlayer;

bool GameItem::CheckHitPlayer(const MelLib::GameObject& pObject)
{

	return typeid(pObject) == typeid(Player);
}

void GameItem::Erase()
{
	// �����ɃG�t�F�N�g�ǉ�����

	eraseManager = true;
}

GameItem::GameItem(const MelLib::Vector3& pos)
	:GameObject("Item")
{
	SetPosition(pos);
	sphereDatas["main"].resize(1);
	

	//// ���Ɣ̔��肪�Ȃ����߁A���ɃJ�v�Z���ɂ��Ă��� �X�e�[�W�͎O�p�`������֌W�Ȃ�
	//// �����蔻��̍쐬
	//capsuleDatas["main"].resize(1);
	//capsuleDatas["main"][0].SetRadius(4.5f);

	//segment3DDatas["main"].resize(1);
	//segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());
}

void GameItem::Initialize()
{
	FallStart(1.0f);
	sphereDatas["main"][0].SetRadius(20);
	sphereDatas["main"][0].SetPosition(GetPosition());
	/*MelLib::Vector3 pos = GetPosition();
	MelLib::Value2<MelLib::Vector3>segmentPos = 
		MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, 0, 0));
	capsuleDatas["main"][0].GetRefSegment3DData().SetPosition(segmentPos);*/
}

void GameItem::Update()
{
	// �Ȃ񂩏���ɓ����蔻��̔��a���������Ȃ��Ă邩��ǂ��ŕς���Ă邩�T��
	// �������SetScale�Ń��f���Ɣ���̑傫���������ɂȂ�悤�ɂȂ��Ă邩�珟��ɕς�����Ⴄ
	sphereDatas["main"][0].SetRadius(5);
	CalcMovePhysics();

	SetAngle(GetAngle() + MelLib::Vector3(0, 3, 0));
}

void GameItem::Draw()
{
	AllDraw();
}

void GameItem::Hit(const MelLib::GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	
}

void GameItem::CheckHitGround(const GameObject& object)
{
	if (typeid(object) == typeid(Stage))
	{
		FallEnd();

		sphereDatas["main"][0].SetPosition(GetPosition() + MelLib::Vector3(0,3,0));
	}
}
