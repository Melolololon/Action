#include "WeakEnemy.h"

const float WeakEnemy::MOVE_SPEED = 0.2f;

void WeakEnemy::Attack()
{
}

WeakEnemy::WeakEnemy(const MelLib::Vector3& pos):NewEnemy("WeakEnemy")
{
	SetPosition(pos);

	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"WeakEnemy");
	modelObjects["main"].SetPosition(pos);

	// ìñÇΩÇËîªíËÇÃçÏê¨
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());


	modelObjects["main"].SetScale(3);
}

void WeakEnemy::Update()
{
}

void WeakEnemy::Draw()
{
	AllDraw();
}
