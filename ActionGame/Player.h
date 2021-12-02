#pragma once
#include"GameObject.h"

#include<FrameTimer.h>
#include<Easing.h>

#include"PlayerSlush.h"


//攻撃時間終わったかどうか判定から受け取ってプレイヤー制御する?
//攻撃判定の生存時間と攻撃時間は違うからとりあえず今のままでいい?
//とりあえず攻撃時間は技ごとに定義したほうがよさそう

class Player :public MelLib::GameObject
{
private:
	
	//前フレームの座標を格納する変数
	MelLib::Vector3 prePos;
	//向いてる向き
	MelLib::Vector3 direction = MelLib::Vector3(0, 0, 1);

	static const unsigned int HP_MAX = 200;
	unsigned int hp = HP_MAX;
	bool hitGround = false;

#pragma region 移動
	float moveSpeed = 0.5f;

#pragma region ダッシュ
	//シュン!って感じで動くやつ
	//ヤーナムステップ、閃乱カグラのあれ
	bool isDash = false;

	MelLib::Easing<MelLib::Vector3> dashEasing;
	//ダッシュの増加量
	MelLib::Vector3 dashAddValue = 0.0f;
#pragma endregion

#pragma endregion

#pragma region 攻撃

	struct AttackData
	{
		int power = 0;
		int time = 0;


		//キャンセル可能になるまでの時間
		//現在は攻撃判定のある時間にもなっているが、変える可能性あり
		//その場合、判定のある時間を判定自身に持たせ、引数をなくす
		// 次の攻撃に移れるようになるまでの時間
		int nextTime = 0;

		AttackData(){}
		AttackData(const int power, const int time, const int nextTime):
			power(power),
			time(time),
			nextTime(nextTime){}

	};

	std::shared_ptr<PlayerSlush>pPSlush = nullptr;
	//現在の攻撃
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;
	
	//攻撃時間タイマー
	MelLib::FrameTimer attackTimer;
	

	// static constだと要素にアクセスできないので、const AttackDataにしてる
	// 攻撃の情報をまとめたもの
	static std::unordered_map<PlayerSlush::AttackType, const AttackData>attackData;



#pragma endregion


#pragma region カメラ
	float cameraSpeed = 0.0f;
#pragma endregion


private:

#pragma region Update関係

	void ChangeAnimationData();

	//操作不可能時にifで囲むの大変だから関数化


	void Move();
	/// <summary>
	/// 攻撃時の移動
	/// </summary>
	void AttackMove();

	void Dash();
	void Jump();

	//攻撃処理、攻撃判定の操作
	void Attack();
	void SetAttackType();

	void Camera();
	void SetCameraPosition();
#pragma endregion

public:
	static void LoadResources();

	Player(const MelLib::Vector3& pos);
	~Player(){}

	PlayerSlush* GetPPlayerSlush() { return pPSlush.get(); }

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

#pragma region Get
	unsigned int GetCurrentAttackPower()const { return attackData[currentAttack].power; }

	MelLib::Vector3 GetDirection() { return direction; }
#pragma endregion

};

