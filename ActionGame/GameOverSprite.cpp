#include "GameOverSprite.h"

void GameOverSprite::LoadResources()
{
	bool r = MelLib::Texture::Load("Resources/Texture/ActionPart/GameOver.png", "GameOver");
}

GameOverSprite::GameOverSprite()
{
	sprites["main"].Create(MelLib::Texture::Get("GameOver"));
	sprites["main"].SetPosition(MelLib::Vector2(650,500));
}

void GameOverSprite::Update()
{
	const float CHANGE_ALPHA_MUL = 1.5f;

	alphaMul += CHANGE_ALPHA_MUL;
	if (alphaMul >= 100.0f)
	{
		alphaMul = 100.0f;
	}
	sprites["main"].SetMulColor(MelLib::Color(255, 255, 255, MelLib::Color::ParToUChar(alphaMul)));

	const MelLib::Vector2 POS = sprites["main"].GetPosition();
	if (POS.y >= 380.0f) 
	{
		sprites["main"].SetPosition(POS + MelLib::Vector2(0, -4.0f));
	}

}

void GameOverSprite::Draw()
{
	sprites["main"].Draw();
}
