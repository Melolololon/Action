#include "Boss.h"

#include"Stage.h"


void Boss::AttackUpdate()
{
	switch (currentAttack)
	{
	case Boss::CurrentAttack::NONE:
		break;
	case Boss::CurrentAttack::JUMP:
		JumpAttack();
		break;
	default:
		break;
	}
}

void Boss::JumpAttack()
{
	if (modelObjects["main"].GetAnimationFrame() == 17) FallStart(3.0f);
}

void Boss::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx",true,"boss");
}

void Boss::SetPlayer()
{
}

Boss::Boss()
	:GameObject("boss")
{
	modelObjects["main"].Create(MelLib::ModelData::Get("boss"), "boss", nullptr);
	modelObjects["main"].SetAnimation("No_Cont");


	//modelObjects["main"].SetPosition();
	//sphereDatas["main"].resize(3);




	actionTimer.SetMaxTime(60 * 6);

	// âºê›íË
	modelObjects["main"].SetAnimation("Attack_Jump.001");
	currentAttack = CurrentAttack::JUMP;
}

std::shared_ptr<MelLib::GameObject> Boss::GetNewPtr()
{
	return std::make_shared<Boss>();
}

void Boss::Initialize()
{
	FallStart(0.0f);

	// ìñÇΩÇËîªíËÇÃçÏê¨
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(GetPosition() + MelLib::Vector3(0, 25.0f, 0), GetPosition() + MelLib::Vector3(0,-3.0f, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());
}

void Boss::Update()
{
	if (thisState == ThisState::BATTLE)actionTimer.SetStopFlag(false);


	modelObjects["main"].Update();
	modelObjects["main"].SetAnimationPlayFlag(true);
	AttackUpdate();
	CalcMovePhysics();

	// âº
	if (MelLib::Input::KeyState(DIK_Z)) 
	{
		modelObjects["main"].SetAnimation("No_Cont");
		currentAttack = CurrentAttack::NONE;
	}
	else 
	{
		modelObjects["main"].SetAnimation("Attack_Jump.001");
		currentAttack = CurrentAttack::JUMP;
	}
}

void Boss::Draw()
{
	AllDraw();
}

void Boss::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//ìäÇ∞è„Ç∞èàóùèIóπ
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);

	/*	if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
		{
			currentThisAttackEffect = AttackEffect::NONE;

			state = ThisState::GET_UP;
			modelObjects["main"].SetAnimation("BeBlownAway_2");
			modelObjects["main"].SetAnimationFrameStart();
		}*/
	}
}


void Boss::Attack()
{
}

void Boss::Move()
{
}
