#include "ClearSprite.h"

void ClearSprite::LoadResources()
{
	bool r = MelLib::Texture::Load("Resources/Texture/ActionPart/Clear.png","Clear");
}

ClearSprite::ClearSprite()
{
	sprites["main"].Create(MelLib::Texture::Get("Clear"));
	sprites["main"].SetScale(MelLib::Vector2(3,3));
	sprites["main"].SetScalingPoint(MelLib::Vector2(0.5f, 0.5f));
	sprites["main"].SetPosition(MelLib::Vector2(700, 380));
	
	sprites["zanzo"].Create(MelLib::Texture::Get("Clear"));
	sprites["zanzo"].SetScale(MelLib::Vector2(1,1));
	sprites["zanzo"].SetScalingPoint(MelLib::Vector2(0.5f, 0.5f));
	sprites["zanzo"].SetPosition(MelLib::Vector2(700, 380));
}

void ClearSprite::Update()
{
	const float CHANGE_ALPHA_MUL = 1.5f;

	alphaMul += CHANGE_ALPHA_MUL;
	if (alphaMul >= 100.0f) 
	{
		alphaMul = 100.0f;
	}
	sprites["main"].SetMulColor(MelLib::Color(255, 255, 255, MelLib::Color::ParToUChar(alphaMul)));

	const float DOWN_SCALE = 0.1f;
	const MelLib::Vector2 SCALE = sprites["main"].GetScale();
	if(SCALE.x > 1.0f)sprites["main"].SetScale(SCALE - DOWN_SCALE);
	else
	{
		zanzoAlphaMul -= CHANGE_ALPHA_MUL * 4;
		if (zanzoAlphaMul <= 0.0f) zanzoAlphaMul = 0.0f;
		sprites["zanzo"].SetMulColor(MelLib::Color(255, 255, 255, MelLib::Color::ParToUChar(zanzoAlphaMul)));

		const MelLib::Vector2 ZANZO_SCALE = sprites["zanzo"].GetScale();
		sprites["zanzo"].SetScale(ZANZO_SCALE + DOWN_SCALE * 6);
	}
}

void ClearSprite::Draw()
{
	sprites["main"].Draw();

	const MelLib::Vector2 SCALE = sprites["main"].GetScale();
	if (SCALE.x <= 1.0f)sprites["zanzo"].Draw();
}
