#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"

#include"ActionPart.h"

#include"PlayerSlush.h"

#include"Ground.h"

#include"Pause.h"
#include"EditMode.h"

std::unordered_map<PlayerSlush::AttackType, const Player::AttackData> Player::attackData =
{
	{PlayerSlush::AttackType::NONE,AttackData(0,0,0)},
	{PlayerSlush::AttackType::NORMAL_1,AttackData(1,20,10)},
	{PlayerSlush::AttackType::NORMAL_2,AttackData(1,30,20)},
	{PlayerSlush::AttackType::NORMAL_3,AttackData(1,20,10)},
};


 
void Player::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Player/Player_Test.fbx",true,"Player");
}

Player::Player(const MelLib::Vector3& pos)
{

	//物理演算のためにseterとgeter作る?


	collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].SetRadius(2.5f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 3, 0), GetPosition() + MelLib::Vector3(0, -28, 0)));

	segment3DData.resize(1);
	segment3DData[0] = capsuleData[0].GetSegment3DData();

	SetPosition(pos);

	modelObjects["main"];
	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	modelObjects["main"].Create(MelLib::ModelData::Get("Player"), nullptr);
	modelObjects["main"].SetScale(MelLib::Vector3(3));
	modelObjects["main"].SetPosition(MelLib::Vector3(0, 4, -0));
	modelObjects["main"].SetAnimationPlayFlag(true);
#pragma region ダッシュ
	dashEasing.SetAddPar(5.0f);



#pragma endregion

	//浮き防止
	FallStart(0.0f);
}

void Player::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	prePos = GetPosition();
	Move();
	Jump();
	Attack();


	CalcMovePhysics();

	// 落下アニメーション(仮)
	//if (GetIsFall() && !hitGround)
	//{
	//	MelLib::Vector3 velocity = GetVelocity();
	//	if (velocity.y != 0) {
	//		modelObjects["main"].SetAnimationEndStopFlag(true);
	//		modelObjects["main"].SetAnimation("Jump");
	//	}
	//}


	//とりあえずSetPositionを使って判定セットしたり攻撃判定動かしてる
	//SetPosition(position);

	Camera();

	float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	// 回転処理(仮)
	modelObjects["main"].SetAngle(MelLib::Vector3(0, angle, 0));


	hitGround = false;


	ChangeAnimationData();
}

void Player::ChangeAnimationData()
{
	std::string animationName = modelObjects["main"].GetCurrentAnimationName();

	modelObjects["main"].SetAnimationSpeedMagnification(1);
	modelObjects["main"].SetAnimationEndStopFlag(false);

	if(animationName == "Dash"){
		modelObjects["main"].SetAnimationSpeedMagnification(2);
	}
	else if(animationName.find("Attack") != std::string::npos){

		modelObjects["main"].SetAnimationSpeedMagnification(2);
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Dash_02") != std::string::npos) {

		modelObjects["main"].SetAnimationEndStopFlag(true);
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
		AddPosition(direction * 0.1f);
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

	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
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


		modelObjects["main"].SetAnimation("Dash_02");

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

	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))
	{
		FallStart(2.0f);
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
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::X)
		&& (attackTimer.GetNowTime() == 0 && currentAttack == PlayerSlush::AttackType::NONE
			|| attackTimer.GetNowTime() >= attackData[currentAttack].nextTime && currentAttack != PlayerSlush::AttackType::NONE))
	{
		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(false);

		//CurrentAttack更新
		SetAttackType();
		//攻撃時間セット
		attackTimer.SetMaxTime(attackData[currentAttack].time);

		//コンボの最後の時にボタン押したらNONEをセットするため、コンボを終わらせるためのif
		if (currentAttack != PlayerSlush::AttackType::NONE)
		{
			if (pPSlush)
			{
				//解放
				pPSlush.reset();
			}

			pPSlush = std::make_shared<PlayerSlush>(GetPosition(), direction, currentAttack, attackData[currentAttack].time);
			MelLib::GameObjectManager::GetInstance()->AddObject(pPSlush);

		}
	}

	//攻撃判定も動かす
	if (pPSlush)pPSlush->AddPosition(GetPosition() - prePos);
}

void Player::SetAttackType()
{
	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		
		//通常攻撃
		if (hitGround)
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
	default:
		break;
	}
}

void Player::Camera()
{


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


	pCamera->SetRotateCriteriaPosition(GetPosition() + MelLib::Vector3(0,10,0));

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
	}
}
