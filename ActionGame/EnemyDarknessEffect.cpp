#include "EnemyDarknessEffect.h"

#include<Random.h>

void EnemyDarknessEffect::LoadResources()
{
	MelLib::Texture::Load("Resources/Texture/Effect/EnemyDarkEffect.png", "DarknessEffect");
}

EnemyDarknessEffect::EnemyDarknessEffect(const MelLib::Vector3& pos)
	:GameObject("EnemyDarknessEffect")
{

	sprite.Create(MelLib::Texture::Get("DarknessEffect"));
	sprite.SetPosition(pos + MelLib::Random::GetRandomVector3NumberRangeSelect(-5, 5, 2));
	sprite.SetScale(4);



	sprite.SetBillboardFlag(true, true, true);
	sprite.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));
}

void EnemyDarknessEffect::Initialize()
{
	deadTimer.SetMaxTime(60 * 1);
	deadTimer.SetStopFlag(false);
}

void EnemyDarknessEffect::Update()
{
	sprite.SetPosition(sprite.GetPosition() + MelLib::Vector3(0, 0.1f, 0));
	sprite.SetPosition(sprite.GetPosition() + MelLib::Vector3(0, 0.1f, 0));

	static const float UP_ALPHA_NUM = 5;
	if (deadTimer.GetNowTime() >= deadTimer.GetMaxOverFlag() - 60 * 0.2)
	{
		subAlpha -= UP_ALPHA_NUM;
	}
	else
	{
		subAlpha += UP_ALPHA_NUM;
	}

	if (subAlpha >= 100.0f)subAlpha = 100.0f;
	else if (subAlpha <= 0.0f)subAlpha = 0.0f;

	if (deadTimer.GetMaxOverFlag()) eraseManager = true;
}


void EnemyDarknessEffect::Draw()
{
	sprite.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));
	sprite.Draw();
}
