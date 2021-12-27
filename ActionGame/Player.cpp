#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Title.h"

#include"ActionPart.h"

#include"PlayerSlush.h"

#include"Ground.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"

std::unordered_map<Player::ActionType, MelLib::PadButton> Player::keyConfigData =
{
	{ActionType::JUMP, MelLib::PadButton::A},
	{ActionType::ATTACK, MelLib::PadButton::X},
	{ActionType::DASH, MelLib::PadButton::B},

};

const float Player::JUMP_POWER = 2.0f;

std::unordered_map<PlayerSlush::AttackType, const Player::AttackData> Player::attackData =
{
	{PlayerSlush::AttackType::NONE,AttackData(0,0,0)},
	{PlayerSlush::AttackType::NORMAL_1,AttackData(1,20,10)},
	{PlayerSlush::AttackType::NORMAL_2,AttackData(1,30,20)},
	{PlayerSlush::AttackType::NORMAL_3,AttackData(1,20,10)},
	{PlayerSlush::AttackType::DASH_1,AttackData(1,20,10)},
};



void Player::LoadResources()
{
	// タイトルでも使うから自動削除削除
	MelLib::ModelData::Load("Resources/Model/Player/Player_Test.fbx", false, "Player");

}

Player::Player(const MelLib::Vector3& pos)
{

	//物理演算のためにseterとgeter作る?


	collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].SetRadius(2.5f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 3, 0), GetPosition() + MelLib::Vector3(0, -18, 0)));

	segment3DData.resize(1);
	segment3DData[0] = capsuleData[0].GetSegment3DData();


	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	modelObjects["main"].Create(MelLib::ModelData::Get("Player"), nullptr);

	modelObjects["main"].SetAnimationPlayFlag(true);
#pragma region ダッシュ
	dashEasing.SetAddPar(5.0f);


#pragma endregion

	mutekiTimer.SetMaxTime(60 * 1.5);



	// タイトル用処理
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(Title))
	{
		modelObjects["main"].SetScale(MelLib::Vector3(3));
		modelObjects["main"].SetAngle(0);
		modelObjects["main"].SetAnimation("No_Cont");
	}
	else
	{
		modelObjects["main"].SetScale(MelLib::Vector3(3));
		modelObjects["main"].SetAngle(0);
		modelObjects["main"].SetPosition(MelLib::Vector3(0, -16, -0));


		//浮き防止
		FallStart(0.0f);
	}

	SetPosition(pos);
}

void Player::Update()
{

	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())
	{

		MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
		if (typeid(*currentScene) != typeid(Title))
		{
			SetCameraPosition();
		}
		return;
	}

	modelObjects["main"].Update();
	if (!setStartParam)
	{
		if (typeid(*currentScene) != typeid(Title))
		{
			JumpAnimation();
			ChangeAnimationData();

			SetCameraPosition();
			CalcMovePhysics();
		}
		return;
	}


	if (hitGroundNotMove)
	{
		if (hitGroundNotMoveTimer.GetMaxOverFlag())
		{
			hitGroundNotMoveTimer.ResetTimeZero();
			hitGroundNotMoveTimer.SetStopFlag(true);
			hitGroundNotMove = false;
		}
		else
		{
			Camera();
			return;
		}
	}

	prePos = GetPosition();
	Move();
	Jump();
	Attack();


	CalcMovePhysics();

	Camera();
	LockOn();

	float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	// 回転処理(仮)
	modelObjects["main"].SetAngle(MelLib::Vector3(0, angle, 0));



	preHitGround = hitGround;
	hitGround = false;

	// アニメーションに合わせて再生速度やループを設定する
	ChangeAnimationData();




}

void Player::TitleUpdate()
{
	// 移動
	AddPosition(MelLib::Vector3(0, 0, 0.5f));


	// ジャンプ
	if (!GetIsFall())
	{
		FallStart(JUMP_POWER);
	}
	CalcMovePhysics();

	JumpAnimation();
	ChangeAnimationData();
}

void Player::ChangeAnimationData()
{
	std::string animationName = modelObjects["main"].GetCurrentAnimationName();

	modelObjects["main"].SetAnimationSpeedMagnification(1);
	modelObjects["main"].SetAnimationEndStopFlag(false);

	if (animationName == "Dash") {
		modelObjects["main"].SetAnimationSpeedMagnification(2);
	}
	else if (animationName.find("Attack") != std::string::npos) {

		modelObjects["main"].SetAnimationSpeedMagnification(2);
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Dash_02") != std::string::npos) {

		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Jump_Up") != std::string::npos)
	{
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}

	// デバッグ用
	if (MelLib::Input::KeyTrigger(DIK_1))
	{
		isTPause = !isTPause;
	}

	// 強制Tポーズ
	if (isTPause)
	{
		modelObjects["main"].SetAnimation("_T");
		modelObjects["main"].SetAnimationSpeedMagnification(1);
		modelObjects["main"].SetAnimationEndStopFlag(false);
	}

}

void Player::Move()
{
	// 仮
	modelObjects["main"].SetAnimationSpeedMagnification(1);

	Dash();

	if (isDash)
	{
		return;
	}
	else if (attackTimer.GetNowTime() > 0)
	{
		AttackMove();
		return;
	}


	static const float FAST_WARK_STICK_PAR = 60.0f;
	static const float WALK_STICK_PAR = 20.0f;
	static const float MAX_FAST_WARK_SPEED = 2.5f;
	static const float MAX_WALK_SPEED = 0.5f;
	static const float FRAME_UP_FAST_WARK_SPEED = MAX_FAST_WARK_SPEED / 10;
	static const float FRAME_UP_WARK_SPEED = MAX_WALK_SPEED / 10;

	if (MelLib::Input::LeftStickUp(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickDown(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickRight(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickLeft(WALK_STICK_PAR))
	{
		direction = MelLib::Input::LeftStickVector3(MelLib::Camera::Get(), false, true);

		MelLib::Vector3 addPos = direction;

		//ダッシュの傾き量を超えていたらダッシュ
		if (MelLib::Input::LeftStickUp(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickDown(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickRight(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickLeft(FAST_WARK_STICK_PAR))
		{
			if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
			else moveSpeed = MAX_FAST_WARK_SPEED;
			addPos *= moveSpeed;

			modelObjects["main"].SetAnimation("Dash");
		}
		else
		{
			if (moveSpeed < MAX_WALK_SPEED)moveSpeed += FRAME_UP_WARK_SPEED;
			else moveSpeed = MAX_WALK_SPEED;
			addPos *= moveSpeed;
			addPos *= MAX_WALK_SPEED;

			modelObjects["main"].SetAnimation("Walk");
		}

		AddPosition(addPos);


	}
	else
	{
		moveSpeed = 0.0;

		modelObjects["main"].SetAnimation("No_Cont");
	}


	//if (position.y <= -100)position.y = 100;

	//落下するために
	if (!GetIsFall())FallStart(0.0f);



}

void Player::AttackMove()
{

	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		//AddPosition(direction * 0.1f);
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::DASH_1:
		break;
	default:
		break;
	}



}

void Player::Dash()
{
	static const float MAX_DASH_SPEED = 1.5f;
	static const float START_DASH_SPEED = 1.0f;

	static const float DASH_DISTANCE = 200.0f;

	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::DASH])
		&& !isDash
		&& currentAttack == PlayerSlush::AttackType::NONE)
	{
		isDash = true;

		dashEasing.SetStart(GetPosition());
		dashEasing.SetEnd(MelLib::LibMath::FloatDistanceMoveVector3(GetPosition(), direction, DASH_DISTANCE));
	}

	if (isDash)
	{
		MelLib::Vector3 prePos = GetPosition();
		MelLib::Vector3 pos = dashEasing.EaseInOut();
		MelLib::Vector3 addPos = pos - prePos;

		// Yを0にしないとダッシュ中にYが変わったらおかしくなっちゃう
		AddPosition(MelLib::Vector3(addPos.x, 0, addPos.z));


		// ダッシュ攻撃中じゃなかったらダッシュアニメーション
		if (currentAttack != PlayerSlush::AttackType::DASH_1)
		{
			modelObjects["main"].SetAnimation("Dash_02");
		}

	}

	if (dashEasing.GetPar() >= 100.0f)
	{
		dashEasing.SetPar(0.0f);
		isDash = false;
	}
}

void Player::Jump()
{
	// ジャンプの着地アニメーションは、ジャンプの逆再生でOK

	if (attackTimer.GetNowTime() != 0)return;

	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::JUMP]))
	{
		FallStart(JUMP_POWER);
	}


	JumpAnimation();
}

void Player::JumpAnimation()
{
	// 条件満たしたらジャンプアニメーション
	if (!hitGround && !preHitGround && !isDash && currentAttack == PlayerSlush::AttackType::NONE)
	{
		if (!jumpResetAnimation)
		{
			jumpResetAnimation = true;
			modelObjects["main"].ResetAnimation();
		}
		modelObjects["main"].SetAnimation("Jump_Up");
	}
}



void Player::Attack()
{
	if (attackTimer.GetMaxOverFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		attackTimer.ResetTimeZero();

		attackTimer.SetStopFlag(true);
	}

	//ifの2行目のタイマー確認は、コンボ終了後にNONEにするため、その攻撃中に入らないようにするために書いてる
	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::ATTACK])
		&& (attackTimer.GetNowTime() == 0 && currentAttack == PlayerSlush::AttackType::NONE
			|| attackTimer.GetNowTime() >= attackData[currentAttack].nextTime && currentAttack != PlayerSlush::AttackType::NONE))
	{


		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(false);


		// 向き切り替え
		Move();

		//CurrentAttack更新
		SetAttackType();
		//攻撃時間セット
		attackTimer.SetMaxTime(attackData[currentAttack].time);

		//コンボの最後の時にボタン押したらNONEをセットするため、コンボを終わらせるためのif
		if (currentAttack != PlayerSlush::AttackType::NONE)
		{
			if (pPSlush)pPSlush.reset();
			if (pRigthSlush)pRigthSlush.reset();


			CreateAttackSlush();

			//アニメーションをリセット
			modelObjects["main"].ResetAnimation();
		}
	}

	//攻撃判定も動かす
	//if (pPSlush)pPSlush->AddPosition(GetPosition() - prePos);
}

void Player::SetAttackType()
{


	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:

		// ダッシュ攻撃
		if (isDash)
		{
			currentAttack = PlayerSlush::AttackType::DASH_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");
		}
		else if (hitGround)// 通常攻撃
		{
			currentAttack = PlayerSlush::AttackType::NORMAL_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");

		}
		else// ジャンプ攻撃
		{
		}

		break;
	case PlayerSlush::AttackType::NORMAL_1:
		currentAttack = PlayerSlush::AttackType::NORMAL_2;

		modelObjects["main"].SetAnimation("Attack_Normal_2");
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		currentAttack = PlayerSlush::AttackType::NORMAL_3;

		modelObjects["main"].SetAnimation("Attack_Normal_3");
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;

	case PlayerSlush::AttackType::DASH_1:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;

	default:
		break;
	}



}

void Player::CreateAttackSlush()
{
	// 消える時間を次の攻撃へ移行できるようになる時間と同じにすること


	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		pRigthSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);

		break;
	case PlayerSlush::AttackType::NORMAL_2:


		pPSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, true);

		break;
	case PlayerSlush::AttackType::NORMAL_3:
		pPSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, true);

		pRigthSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);


		break;
	case PlayerSlush::AttackType::DASH_1:
		break;
	default:
		break;
	}

	if (pPSlush)MelLib::GameObjectManager::GetInstance()->AddObject(pPSlush);
	if (pRigthSlush)MelLib::GameObjectManager::GetInstance()->AddObject(pRigthSlush);
}

void Player::Muteki()
{
	if (mutekiTimer.GetMaxOverFlag())
	{
		isMuteki = false;
		mutekiTimer.ResetTimeZero();
		mutekiTimer.SetStopFlag(true);
	}
}

void Player::Camera()
{

	if (lockOn)return;

	//スティックの倒し具合でスピード変える
	//緩急あったほうがいい?だんだん加速してく。早めにスピードマックスになる

	//メインカメラのポインタ取得
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0;
	static const float MAX_CAMERA_ANGLE_X = 30.0f;
	static const float MIX_CAMERA_ANGLE_X = -5.0f;

	//最大値は一旦const(あとで設定で最大値を変えられるようにする)
	static const float MAX_CAMERA_SPEED = 3.0f;
	static const float FRAME_UP_CAMERA_SPEED = MAX_CAMERA_SPEED / 10;

	if (!MelLib::Input::RightStickLeft(30.0f)
		&& !MelLib::Input::RightStickRight(30.0f)
		&& !MelLib::Input::RightStickUp(30.0f)
		&& !MelLib::Input::RightStickDown(30.0f))cameraSpeed = 0.0f;
	else cameraSpeed += FRAME_UP_CAMERA_SPEED;

	if (MelLib::Input::RightStickLeft(30.0f))addCameraAngle.y = -cameraSpeed;
	else if (MelLib::Input::RightStickRight(30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(30.0f))addCameraAngle.x = -cameraSpeed;
	else if (MelLib::Input::RightStickDown(30.0f))addCameraAngle.x = cameraSpeed;

	if (cameraSpeed >= MAX_CAMERA_SPEED)cameraSpeed = MAX_CAMERA_SPEED;

	cameraAngle += addCameraAngle;
	if (cameraAngle.x >= MAX_CAMERA_ANGLE_X)cameraAngle.x = MAX_CAMERA_ANGLE_X;
	if (cameraAngle.x <= MIX_CAMERA_ANGLE_X)cameraAngle.x = MIX_CAMERA_ANGLE_X;

	pCamera->SetAngle(cameraAngle);
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(80.0f);

	//pCamera->SetRotateCriteriaPosition(position);
	SetCameraPosition();
}

void Player::SetCameraPosition()
{
	MelLib::Camera* pCamera = MelLib::Camera::Get();
	/*pCamera->SetRotateCriteriaPosition
	(MelLib::LibMath::FloatDistanceMoveVector3
	(GetPosition(), MelLib::LibMath::OtherVector3(pCamera->GetCameraPosition(), pCamera->GetTargetPosition()), 30.0f));*/

	if (!lockOn)
	{
		pCamera->SetRotateCriteriaPosition(GetPosition() + MelLib::Vector3(0, 10, 0));
	}
}

void Player::LockOn()
{
	// 全敵を取得
	// 距離以内かつ最短のやつにカメラ向ける
	// ロックオン中はカメラは動かせない
	// プレイヤーが映るかつ、注視点を敵にセットしていい感じに映るようにする。
	// 既存のゲームを参考にしたほうがいいかも
	// プレイヤーと敵の方向ベクトルに応じてカメラ回転させれば、いい感じになるかも?

	// スティックでロックオン対象を切り替えれるようにすること

	// ロックオンの最高距離
	static const float LOCK_ON_DISTANCE = 200.0f;

	// ロックオン有効、無効
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::R_STICK))
	{
		lockOn = !lockOn;
		if(lockOn)
		{
			lockOnEnemyDistance = FLT_MAX;
			// 近い敵を取得
			for (const auto& enemy : ActionPart::GetEnemys())
			{
				float distance = MelLib::LibMath::CalcDistance3D(GetPosition(), enemy->GetPosition());
				if (distance <= lockOnEnemyDistance)
				{
					lockOnEnemyDistance = distance;
					lockOnEnemy = enemy.get();
				}
			}

			// 敵いなかったら終了
			if (lockOnEnemyDistance == FLT_MAX)return;

			// 近くにいなかったら終了
			if (lockOnEnemyDistance > LOCK_ON_DISTANCE)return;

		}
	}

	if (!lockOn)return;

	// カメラ操作
	// カメラの動きを補完すること

	// まずは視点切り替えを優先して実装すること
	
	lockOnEnemyDistance = MelLib::LibMath::CalcDistance3D(GetPosition(), lockOnEnemy->GetPosition());

	// 離れたら解除
	if(lockOnEnemyDistance >= LOCK_ON_DISTANCE)
	{
		lockOn = false;
		return;
	}

	// 注視点を敵の座標に
	MelLib::Camera::Get()->SetRotateCriteriaPosition(lockOnEnemy->GetPosition());

	// 注視点との距離を変える
	MelLib::Camera::Get()->SetCameraToTargetDistance(lockOnEnemyDistance * 2.0f);


	MelLib::Vector3 playerToEnemy = lockOnEnemy->GetPosition() - GetPosition();
	float xzAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerToEnemy.x, playerToEnemy.z), true);

	static const float CONST_ANGLE_X = 35;
	/*float disAngle = lockOnEnemyDistance * 0.3f;
	if (disAngle - CONST_ANGLE_X > 90 - CONST_ANGLE_X)disAngle = 90 - CONST_ANGLE_X;*/
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(CONST_ANGLE_X , -xzAngle + 90,0));
}


void Player::Draw()
{
	modelObjects["main"].Draw();
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{

	if (typeid(*object) == typeid(Ground)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		// 着地処理
		if (!hitGroundNotMove)
		{
			float velocityY = GetVelocity().y;

			if (velocityY <= -3.0f)// 超落下
			{
				hitGroundNotMoveTimer.SetMaxTime(60 * 0.75);
				hitGroundNotMoveTimer.SetStopFlag(false);
				hitGroundNotMove = true;

				modelObjects["main"].SetAnimation("Jump_End_1");
			}
			jumpResetAnimation = false;
		}




		MelLib::Vector3 addPos;

		//velocity分戻すと戻しすぎちゃう
		//カプセルの下の座標-半径(先端座標)から衝突点へのベクトルの大きさ分戻せばOK?

		//重力加速度分上げる
		//addPos.y += MelLib::GameObject::GetGravutationalAcceleration();

		//投げ上げ処理終了
		FallEnd();

		//カプセルの下の先端から衝突点のベクトルを求め、その分押し出す。
		//-0.15fは、押し出しすぎるとHitが呼ばれなくなって、非ジャンプ時の落下の処理で下がってがくがくするのを防止するためにある
		//線分の先端がヒットしたらカプセルの先端が上に来るまで押し返すから、-0.15引かないとがくがくする
		/*addPos.y -= (capsuleData[0].GetSegment3DData().GetPosition().v2.y - capsuleData[0].GetRadius()) -
			capsuleData[0].GetSegment3DData().GetCalcResult().boardHitPos.y;*/


			//カプセルじゃなくて線分で判定取ってるときの処理
		addPos.y += segment3DData[0].GetCalcResult().boardHitPos.y - segment3DData[0].GetPosition().v2.y;
		//segment3DData[0] = capsuleData[0].GetSegment3DData();

		AddPosition(addPos);
		SetCameraPosition();

		hitGround = true;



		if (!setStartParam)
		{
			setStartParam = true;

			// 攻撃判定移動用
			// 攻撃判定をセットしたときのプレイヤーのデータを保存
			startPos = modelObjects["main"].GetPosition();
			startAngle = modelObjects["main"].GetAngle();
			startScale = modelObjects["main"].GetScale();
		}
	}

	// 攻撃を受けた時(体力減算はEnemyAttack側で行ってる)
	if (!isMuteki)
	{

		if (typeid(*object) == typeid(EnemyAttack))
		{
			isMuteki = true;
			mutekiTimer.SetStopFlag(false);
		}
	}

}
