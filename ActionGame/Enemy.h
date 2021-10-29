#pragma once
#include <GameObject.h>
#include<RouteSearch.h>
#include"Player.h"
//敵クラス
class Enemy :
    public MelLib::GameObject
{
private:

	static Player* pPlayer;
	static std::vector<std::vector<std::vector<MelLib::AStarNode>>> nodes;

	int hp = 0;
	
protected:
	std::vector<MelLib::Vector3>routeVectors;

protected:
	/// <summary>
	/// プレイヤーへ向かう処理
	/// </summary>
	void CalcPlayerRoute();
public:

	Enemy(const MelLib::Vector3& pos,const std::string& modelName);
	virtual ~Enemy(){}
	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
	/// <param name="hitObjColType">相手のどの判定に当たったか</param>
	/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>
	void Hit
	(
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;


	static void SetPPlayer(Player* p) { pPlayer = p; }

	/// <summary>
	/// 経路探索に必要な情報を渡してノードに情報をセットする関数
	/// </summary>
	/// <param name="leftDownFrontPos">経路探索の範囲の左下手前座標</param>
	/// <param name="rightUpBackPos">経路探索の範囲の右上奥座標</param>
	/// <param name="nodeNum">xyzのノード数</param>
	/// <param name="datas">敵が通れないオブジェクトの判定データ</param>
	/// <param name="costs">オブジェクトのコスト</param>
	static void SetAStarNodeDatas
	(
		const MelLib::Vector3& leftDownFrontPos,
		const MelLib::Vector3& rightUpBackPos,
		const MelLib::Value3<UINT>& nodeNum, 
		const std::vector<MelLib::BoxData>& datas, 
		const std::vector<UINT>& costs
	);
};

