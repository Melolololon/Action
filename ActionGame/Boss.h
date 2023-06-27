#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"Player.h"
#include"EnemyHPGauge.h"


// メモスペース
// 現在のボスの攻撃パターン
// 回転　地面叩くやつ

// 追加案
// ジャンプからの落下振り下ろし(カメラがネック？)
// 構え、超接近、ハンマー振り

// 攻撃の選別基準をどうするか
// 乱数?

class Boss :public MelLib::GameObject
{
private:

	static Player* pPlayer;
	static Boss* pBoss;

	// 移動や攻撃を制御するタイマー。例　遠くにいる時、このタイマーの時間に応じて移動するか遠距離攻撃するかを決める
	MelLib::FrameTimer actionTimer;
	
	enum class ThisState 
	{
		BATTLE,
		OTHER
	};
	ThisState thisState = ThisState::OTHER;

	MelLib::Vector3 jumpAttackDir; 
	int jumpAttackCount = 0;
	static const int JUMP_ATTACK_MAX_COUNT = 12;
	MelLib::FrameTimer jumpAttackTimer;

	enum class CurrentState : unsigned int
	{
		NONE,
		NORMAL_1,
		ROLL_ATTACK,

		// ここから遠距離
		JUMP_ATTACK = 10,
		DASH_ATTACK,

	};
	// 乱数でジャンプ攻撃を指定するときに加算する値
	const int JUMP_ATTACK_ADD_NUM = static_cast<std::underlying_type_t<Boss::CurrentState>>(Boss::CurrentState::JUMP_ATTACK);
	CurrentState currentState = CurrentState::NONE;

	// 攻撃制御用タイマー
	MelLib::FrameTimer attackControlTimer;

	MelLib::Vector3 dashAttackStartPlayerPos;

	MelLib::GuiInt hp;
	MelLib::FrameTimer mutekiTimer;
	bool isMuteki = false;
	std::unique_ptr<EnemyHPGauge> hpGauge;
private:

	void Move();

	void SelectAction();
	void AttackUpdate();

	void AttackEnd();


	// 攻撃一覧
	void NormalAttackUpdate();
	void RollAttackUpdate();
	void JumpAttackUpdate();
	void DashAttackUpdate();

	void Rotate();

	void MoveToPlayer(const float speed);
public:


	static void LoadResources();
	static void SetPPlayer(Player* p) { pPlayer = p; }

	Boss();

	std::shared_ptr<MelLib::GameObject> GetNewPtr()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static bool CheckStateBattle() { return pBoss->thisState == ThisState::BATTLE; }



};

