#include "WeakEnemy.h"
#include"Pause.h"
#include"Stage.h"

#include"CapsuleEnemyAttack.h"

#include"PlayerSlush.h"
#include"JumpAttack.h"
#include"SlushHitEffect.h"

#include<GameObjectManager.h>

#include<LibMath.h>

// 35Fで攻撃発生

void WeakEnemy::SetStartAttackAnimation()
{
	modelObjects["main"].SetAnimation("Attack");
	modelObjects["main"].SetAnimationFrameStart();
	state = ThisState::ATTACK;
}

void WeakEnemy::Attack()
{
	/*timer++;

	if (timer >= 60 * 1.5f)
	{
		isAttack = false;
		timer = 0;
	}*/

	// 攻撃判定追加
	if (modelObjects["main"].GetAnimationFrame() == 35)
	{
		AddAttackCollisionDetection();
	}

	if (modelObjects["main"].GetAnimationEndFlag()) 
	{
		isAttack = false;
		state = ThisState::MOVE;
		modelObjects["main"].SetAnimation("E_Dash");
	}
}

void WeakEnemy::AddAttackCollisionDetection()
{
	std::shared_ptr<CapsuleEnemyAttack>attack;

	// 判定のセット方法
	// 1.Tポーズにする
	// 2.四角いモデルを用意し、当たり判定の位置にセットし、その座標をメモする
	// 3.2つ目の引数(今回の0の部分)をメモした座標にする
	// 4.2を行ったときのモデルの座標などを引数の5～7にセットする
	// 
	// 0を判定の初期座標に書き換える
	// 5の部分に、当たり判定の座標ををセットした時の座標、角度、大きさを入力

	// 判定の座標
	MelLib::Value2<MelLib::Vector3>p = 
		MelLib::Value2<MelLib::Vector3>(MelLib::Vector3(-4.9, 9.7, 6.5), MelLib::Vector3(-4.9, 9.7, 8));

	attack = std::make_shared<CapsuleEnemyAttack>
		(
			10,
			p,
			2.0f,
			modelObjects["main"],
			0,
			0,
			1,
			"Bone_R.003",
			"Body.001"
		);

	MelLib::GameObjectManager::GetInstance()->AddObject(attack);

}

void WeakEnemy::Rotate()
{
	// 攻撃中は回転しない

	//direction = (pPlayer->GetPosition() - GetPosition()).Normalize();

	//// 回転
	//float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	//modelObjects["main"].SetAngle(MelLib::Vector3(28, angle, 0));

	// 進行方向に向くように
	MelLib::Vector3 direction = (pPlayer->GetPosition() - GetPosition()).Normalize();

	// 回転
	float directionAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	float preAngle = modelObjects["main"].GetAngle().y;

	// プレイヤーの向きの角度と今の角度が5度以内だったらreturn
	if (MelLib::LibMath::AngleDifference(directionAngle, preAngle, 5))return;

	SetAngle(MelLib::Vector3(0, directionAngle, 0));
	//modelObjects["main"].SetAngle(MelLib::Vector3(0, directionAngle, 0));
}

void WeakEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/WeakEnemy/WeakEnemy.fbx", false, "WeakEnemy");
}

WeakEnemy::WeakEnemy(const MelLib::Vector3& pos):NewEnemy("WeakEnemy")
{
	SetPosition(pos);

	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"WeakEnemy");
	modelObjects["main"].Create(MelLib::ModelData::Get("WeakEnemy"),"WeakEnemy");
	modelObjects["main"].SetPosition(pos);

	// 当たり判定の作成
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, 0, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());

	// プレイヤーみたいな十字の線判定作って、壁でジャンプするようにする


	//modelObjects["main"].SetScale(20);

	/*modelObjects["Hit01"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "H01");
	modelObjects["Hit01"].SetPosition(MelLib::Vector3(-4.9, 9.7, 6.5));

	modelObjects["Hit02"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "H02");
	modelObjects["Hit02"].SetPosition(MelLib::Vector3(-4.9, 9.7, 8));*/

	tags.push_back("Enemy");

	collisionCheckDistance = 50.0f;
}

void WeakEnemy::Initialize()
{
	FallStart(0.0f);
	modelObjects["main"].SetAnimationPlayFlag(true);
	modelObjects["main"].SetAnimation("E_Dash");
	state = ThisState::MOVE;
}

void WeakEnemy::Update()
{
	if (pPlayer->GetAttackChangeFrame()) 
	{
		// これただのフラグにしてもいいかも
		hitAttackName = "";
	}

	CheckDropDead();

	CheckMutekiEnd();
	CheckAttackTagDelete();

	if (hitGround)FallStart(0.0f);

	CheckParticleTimer();
	//AddDarknessEffect();

	if(state != ThisState::DEAD)CalcMovePhysics();

	hpGauge->SetPosition(GetPosition() + MelLib::Vector3(0, 15, 0));
	hpGauge->Update();
	
	modelObjects["main"].Update();



	if (state == ThisState::DEAD) 
	{
		
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			deadEndTimer.SetStopFlag(false);
		}
		Dead();
		return;
	}

	Rotate();

	if (isAttack)Attack();

	// 吹っ飛び中だったら吹っ飛ぶ動作をする
	if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
	{
		BeBlownAwayMove();
		state = ThisState::BE_BLOWN_AWAY;
	}

	if (state == ThisState::STUN) 
	{
		MelLib::Vector3 toPlayerVec = pPlayer->GetPosition() - GetPosition();
		toPlayerVec.y = 0;
		toPlayerVec = toPlayerVec.Normalize();

		AddPosition(-toPlayerVec * 0.05f);
	}

	if ((state == ThisState::GET_UP || state == ThisState::STUN)
		&& modelObjects["main"].GetAnimationEndFlag()) 
	{
		

		state = ThisState::MOVE;
	}

	if (state == ThisState::MOVE) 
	{
		if (prePos == GetPosition())
		{
			modelObjects["main"].SetAnimation("No_Cont");
		}
		else
		{
			modelObjects["main"].SetAnimation("E_Dash");
		}
	}

	prePos = GetPosition();
	hitGround = false;

}

void WeakEnemy::Draw()
{
	
	AllDraw();
	hpGauge->Draw();
}

void WeakEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	//if (Pause::GetInstance()->GetIsPause())return;

	//if (isDead)return;
	if (state == ThisState::DEAD)return;

	std::string n = typeid(object).name();
	if (typeid(object) == typeid(PlayerSlush))
	{
		// hitAttackName関係は関数化して確認と代入まとめよう

		// 違う攻撃判定だったらダメージ
		if (hitAttackName != object.GetTags()[0])
		{
			AddParticle();

			hitAttackName = object.GetTags()[0];

		/*	MelLib::Vector3 toCameraVec = (MelLib::Camera::Get()->GetCameraPosition() - GetPosition()).Normalize();
			MelLib::Vector3 effectAddPos = MelLib::Vector3(0, 6, 0) + toCameraVec * 2.1f;
			MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<SlushHitEffect>
				(GetPosition() + effectAddPos));*/

			// プレイヤーから現在の攻撃の攻撃力を取得し、体力を減らす
			hp -= pPlayer->GetCurrentAttackPower();

			isMuteki = true;
			// 攻撃強制終了
			isAttack = false;


			// 0になったらやられ処理
			if (hp <= 0)
			{
				state = ThisState::DEAD;

				modelObjects["main"].SetAnimation("Dead");
				modelObjects["main"].SetAnimationFrameStart();
				modelObjects["main"].SetAnimationEndStopFlag(true);
				return;
			}

			// 吹っ飛ばし処理
			// 後で1フレーム置いてNONEに切り替えるようにする
			if (pPlayer->GetPlayerAttackEffect() == AttackEffect::BE_BLOWN_AWAY)
			{
				FallStart(0.6f);
				currentThisAttackEffect = AttackEffect::BE_BLOWN_AWAY;
				state = ThisState::BE_BLOWN_AWAY;
				modelObjects["main"].SetAnimation("BeBlownAway");
				modelObjects["main"].SetAnimationFrameStart();
			}
			else
			{
				state = ThisState::STUN;
				modelObjects["main"].SetAnimation("Stun");
				modelObjects["main"].SetAnimationFrameStart();

			}
		}
	}

	if (typeid(object) == typeid(JumpAttack) && !isMuteki) 
	{
		MelLib::Vector3 toCameraVec = (MelLib::Camera::Get()->GetCameraPosition() - GetPosition()).Normalize();
		MelLib::Vector3 effectAddPos = MelLib::Vector3(0, 6, 0) + toCameraVec * 2.1f;
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<SlushHitEffect>
			(GetPosition() + effectAddPos));

		// プレイヤーから現在の攻撃の攻撃力を取得し、体力を減らす
		hp -= pPlayer->GetCurrentAttackPower();

		isMuteki = true;
		mutekiTimer.SetStopFlag(false);

		// 攻撃強制終了
		isAttack = false;


		// 0になったらやられ処理
		if (hp <= 0)
		{
			state = ThisState::DEAD;

			modelObjects["main"].SetAnimation("Dead");
			modelObjects["main"].SetAnimationFrameStart();
			modelObjects["main"].SetAnimationEndStopFlag(true);
			return;
		}


		FallStart(0.6f);
		currentThisAttackEffect = AttackEffect::BE_BLOWN_AWAY;
		state = ThisState::BE_BLOWN_AWAY;
		modelObjects["main"].SetAnimation("BeBlownAway");
		modelObjects["main"].SetAnimationFrameStart();
	}


	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//投げ上げ処理終了
		FallEnd();
		hitGround = true;

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);

		if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
		{
			currentThisAttackEffect = AttackEffect::NONE;

			state = ThisState::GET_UP;
			modelObjects["main"].SetAnimation("BeBlownAway_2");
			modelObjects["main"].SetAnimationFrameStart();
		}
	}
}
