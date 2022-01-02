#include "GameItem.h"
Player* Game::GameItem::pPlayer;

bool Game::GameItem::CheckHitPlayer(const MelLib::GameObject& pObject)
{

	return typeid(pObject) == typeid(Player);
}

void Game::GameItem::Erase()
{
	// �����ɃG�t�F�N�g�ǉ�����

	eraseManager = true;
}

Game::GameItem::GameItem(const MelLib::Vector3& pos)
{
	sphereData.resize(1);
	sphereData[0].SetPosition(pos);
	sphereData[0].SetRadius(5);

	modelObjects["main"].SetPosition(pos);

	SetPosition(pos);
}

void Game::GameItem::Update()
{

}

void Game::GameItem::Draw()
{
}

void Game::GameItem::Hit(const MelLib::GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	
}
