#include "SandEffect.h"

#include<Random.h>
#include<LibMath.h>

void SandEffect::LoadResources()
{
	MelLib::Texture::Load("Resources/Texture/Boss/SandEffect.png","sand");
}

void SandEffect::Initialize()
{
	particleSprite.Create(MelLib::Texture::Get("sand"));
	particleSprite.SetScale(10);
	particleSprite.SetBillboardFlag(true, true, true);

	// ランダムで色の濃さを変える
	// RGB全部一括で下げて調整する
	int addColor = MelLib::Random::GetRandomNumberRangeSelect(-40, 40);
	char cAddColor = static_cast<char>(addColor);
	particleSprite.SetAddColor(MelLib::Color(cAddColor, cAddColor, cAddColor, 0));

}

void SandEffect::Update()
{
	// 上に上がって透明度上げて消す感じに

	// 上に舞い上がる処理
	particleSprite.SetPosition
	(MelLib::LibMath::FloatDistanceMoveVector3(particleSprite.GetPosition(), { 0,1,0 }, FRAME_MOVE_DIS));

	// 少しずつ透明に
	particleSprite.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));

	if (subAlpha < 100.0f) 
	{
		subAlpha += FRAME_SUB_ALPHA;
	}
	else
	{
		subAlpha = 100.0f;
	}
}

std::shared_ptr<MelLib::Particle3D> SandEffect::GetNewPtr() const
{
	return std::make_shared<SandEffect>();
}
