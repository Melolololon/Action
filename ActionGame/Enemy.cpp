#include "Enemy.h"

#include<LibMath.h>
#include<Collision.h>

Player* Enemy::pPlayer;
std::vector<std::vector<std::vector<MelLib::AStarNode>>> Enemy::nodes;

Enemy::Enemy(const MelLib::Vector3& pos, const std::string& modelName)
{
	SetPosition(pos);

	modelObjects["main"];
	if (modelName == "")
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
		modelObjects["main"].SetScale(5);
	}
	else
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(modelName), nullptr);
	}
	modelObjects["main"].SetPosition(pos);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	modelObjects["main"].Draw();
}

void Enemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}

void Enemy::SetAStarNodeDatas
(
	const MelLib::Vector3& leftDownFrontPos,
	const MelLib::Vector3& rightUpBackPos,
	const MelLib::Value3<UINT>& nodeNum, 
	const std::vector<MelLib::BoxData>& datas, 
	const std::vector<UINT>& costs
)
{
	MelLib::RouteSearch::SetNodePosition(leftDownFrontPos, rightUpBackPos, nodeNum, nodes);
	MelLib::RouteSearch::SetHitObjectFlag(datas, nodes);
	MelLib::RouteSearch::SetCost(datas,costs, nodes);
}

void Enemy::CalcPlayerRoute()
{
	// 毎フレーム計算するとがくがくする(フレームごとに向きが変わるから)一定フレームでいいかも
	// ノードに辿りつくごとがベスト?

	MelLib::Vector3 playerPos = pPlayer->GetPosition();
	MelLib::RouteSearch::CalcRoute(GetPosition(), playerPos, nodes, routeVectors, true, 4);
}
