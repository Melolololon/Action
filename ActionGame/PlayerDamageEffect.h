#pragma once
#include<Sprite2D.h>
#include<Easing.h>

class PlayerDamageEffect
{
private:
	MelLib::Sprite2D spr;
	MelLib::Easing<float> alpha;
	bool startEffect = false;
	bool returnAlpha = false;
public:
	PlayerDamageEffect();
	void Update();
	void Draw();

	void StartEffect() { startEffect = true; }
};

