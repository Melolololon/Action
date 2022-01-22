#pragma once
#include <GameObject.h>
#include<RouteSearch.h>
#include<FrameTimer.h>
#include"Player.h"

class Player;

//敵クラス
class Enemy :
	public MelLib::GameObject
{
private:


	static Player* pPlayer;
	static std::vector<std::vector<std::vector<MelLib::AStarNode>>> nodes;

	// 現在選ばれている進行方向
	int currentMoveVector = 0;
	MelLib::FrameTimer routeSearchTimer;
	std::vector<MelLib::Vector3>routeVectors;
	// デバッグ用
	MelLib::ModelObject routeObj[100];

	MelLib::Vector3 direction;



	bool isMuteki = false;
	MelLib::FrameTimer mutekiTimer;

	bool slowMove = false;
private:
	/// <summary>
	/// 経路探索の結果の加算関数
	/// </summary>
	void AddRouteVector();

protected:

	unsigned int hp = 0;
	float moveSpeed = 0.0f;

	bool isAttack = false;

	// 攻撃食らって硬直してるかどうか
	bool isStun = false;

	// やられたかどうか
	bool isDead = false;

	// 攻撃を出すためのタイマー
	MelLib::FrameTimer attackTimer;

	// 消すまでの時間
	MelLib::FrameTimer eraseTimer;

protected:
	/// <summary>
	/// プレイヤーとの距離が指定した数値未満か確認
	/// </summary>
	/// <param name="distance"></param>
	/// <returns></returns>
	bool CheckPlayerDistance(float distance);

	/// <summary>
	/// プレイヤーへ進行できるか確認し、移動させる
	/// </summary>
	/// <returns>進行したかどうか</returns>
	bool CalcPlayerRoute();

	
	void CheckMutekiEnd();

	void AttackStart();
	void CheckAttackEnd();

	void RotModel();

	void Stun();

	void Dead();
public:

	Enemy(const MelLib::Vector3& pos, unsigned int hp, float moveSpeed, const std::string& modelName = "");
	virtual ~Enemy() {}
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

