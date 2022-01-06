#pragma once
#include<GameObject.h>
#include<Sprite3D.h>

// スプライトを並べれば斬撃エフェクトっぽくなる?
// 剣の角度などに応じてスプライトの角度をセットして、時間に応じて透明にしていく

// 斬撃エフェクト
class SlushEffect :public MelLib::GameObject
{
private:
	MelLib::Sprite3D sprite[2];
	float subAlpha = 0.0f;

public:
	static void LoadResources();

	SlushEffect(const MelLib::Vector3& pos,const MelLib::Vector3& slushVector,unsigned int frame,const MelLib::Vector3& direction);

	void Update()override;
	void Draw()override;
	
};

