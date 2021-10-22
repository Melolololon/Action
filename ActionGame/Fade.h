#pragma once
#include<Sprite2D.h>

class Fade
{
private:
	bool isFade = false;
	bool fadeIn = true;

	MelLib::Sprite2D fadeSpr;
	float fadeAlpha = 0.0f;
private:
	Fade(){}
	~Fade(){}

public:
	Fade(Fade& f) = delete;
	Fade& operator= (Fade& f) = delete;
	static Fade* GetInstance();

	void Initializ();
	void Update();

	void Start() { isFade = true; }
	bool GetIsFade() { return isFade; }
	bool GetChangeSceneFlag();
};

