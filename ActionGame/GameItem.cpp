#include "GameItem.h"
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
{
	sphereDatas.resize(1);
	sphereDatas[0].SetPosition(pos);
	sphereDatas[0].SetRadius(5);

	modelObjects["main"].SetPosition(pos);

	SetPosition(pos);
}

void GameItem::Update()
{

}

void GameItem::Draw()
{
}

void GameItem::Hit(const MelLib::GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	
}
