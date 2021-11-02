#include "Enemy.h"

#include<LibMath.h>
#include<Collision.h>

//debug
#include<Input.h>

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

	routeSearchTimer.SetMaxTime(1);
	routeSearchTimer.SetStopFlag(false);

	for (int i = 0; i < _countof(routeObj); i++)
	{
		routeObj[i].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
		routeObj[i].SetScale(MelLib::Vector3(nodes[0][0][0].size.x,1, nodes[0][0][0].size.z));
	}
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

void Enemy::AddRouteVector()
{
	if (MelLib::LibMath::CalcDistance3D(routeVectors[currentMoveVector], GetPosition()) <= 0.3f
		&& currentMoveVector + 1 != routeVectors.size())
	{
		currentMoveVector++;
	}

	if (currentMoveVector != routeVectors.size()) 
	{
		AddPosition(routeVectors[currentMoveVector] * moveSpeed);
	}
}

void Enemy::CalcPlayerRoute()
{
	// 毎フレーム計算するとがくがくする(フレームごとに向きが変わるから)一定フレームでいいかも
	// ノードに辿りつくごとがベスト?
	// プレイヤーが移動したらにする?

	// デバッグ用
	std::vector<MelLib::Vector3>routeNodePos;

	MelLib::Vector3 playerPos = pPlayer->GetPosition();

	MelLib::Vector3 myToPlayer;
	bool straightMove = MelLib::RouteSearch::CheckStraightMove(GetPosition(), playerPos, nodes, 4.0f,myToPlayer);
	
	if (!straightMove && routeSearchTimer.GetMaxOverFlag() || routeVectors.size() == 0)
	{
		bool result = MelLib::RouteSearch::CalcRoute(GetPosition(), playerPos, nodes, &routeVectors,&routeNodePos);
		routeSearchTimer.ResetTimeZero();
	}
	
	if(straightMove)
	{

		routeVectors.clear();
		routeVectors.resize(1, myToPlayer);
		routeVectors[0] = myToPlayer;
	}

	AddRouteVector();

	// デバッグ用
	if(MelLib::Input::KeyState(DIK_SPACE))
	{
  		MelLib::RouteSearch::CalcRoute(GetPosition(), playerPos, nodes, &routeVectors, &routeNodePos);
	}
	if(MelLib::Input::KeyState(DIK_Z))AddRouteVector();
	
	for(int i = 0;i < routeNodePos.size();i++)
	{
		routeObj[i].SetPosition(routeNodePos[i]);
		routeObj[i].Draw();
	}

	// 斜めにジグザグになる原因は、斜めと真っすぐのコストが同じときに、斜めが選ばれるときがあるから
	// ゴールに近いノードを優先的に通るようにする?(コストだけで判断しないようにする?)
}

