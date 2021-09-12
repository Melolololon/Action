#pragma once
#include <GameObject.h>
#include"Player.h"
//敵クラス
class Enemy :
    public MelLib::GameObject
{
private:

	static Player* pPlayer;

	int life = 0;
	
private:
	/// <summary>
	/// プレイヤーへ向かう処理
	/// </summary>
	void MoveToPlayer();
public:

	Enemy(const MelLib::Vector3& pos);
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
		const MelLib::ShapeType3D collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D hitObjColType,
		const int hitObjArrayNum
	)override;


	static void SetPPlayer(Player* p) { pPlayer = p; }
};

