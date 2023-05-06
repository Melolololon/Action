#pragma once
#include<GameObject.h>
#include"Player.h"
#include"AttackEffect.h"
#include"EnemyHPGauge.h"
#include"FrameTimer.h"

class NewEnemy:public MelLib::GameObject
{
private:
	MelLib::FrameTimer addDarknessEffectTimer;

private:
	void DropItem();

public:
	enum class ThisState
	{
		MOVE,
		ATTACK,
		BE_BLOWN_AWAY,
		GET_UP,
		STUN,
		DEAD,
	};

protected:
	std::unique_ptr<EnemyHPGauge> hpGauge;

	/// <summary>
	/// ������ԓ���
	/// </summary>
	void BeBlownAwayMove();

	void Dead();

	void AddParticle();
protected:
	
	MelLib::FrameTimer deadEndTimer;

	bool isAttack = false;
	static Player* pPlayer;

	ThisState state = ThisState::MOVE;
	AttackEffect currentThisAttackEffect = AttackEffect::NONE;

	// �v���C���[�ւ̈ړ�
	//void ToPlayerMove();
	//bool CheckMove();


	static const float MOVE_SPEED;

	// �Œ዗��
	static const float MIN_DISTANCE;

	int hp = 10;

public:



	static void SetPPlayer(Player* p) { pPlayer = p; }

	NewEnemy(const std::string& name);
	void Update()override;
	void Draw()override;

	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;


	void StartAttack() { isAttack = true; }
	virtual void SetStartAttackAnimation();

	bool GetIsAttack()const { return isAttack; }

	AttackEffect GetCurrentThisAttackEffect()const { return currentThisAttackEffect; }

	ThisState GetThisState()const { return state; }

	int GetHP()const { return hp; }

	void AddDarknessEffect();
};

