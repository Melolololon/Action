#include "SlushEffect.h"
#include"Game.h"

#include<LibMath.h>

void SlushEffect::LoadResources()
{
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
	MelLib::Texture::Load(TEXTURE_PATH + "Player/SlushEffect.png", "slushEffect");
}

SlushEffect::SlushEffect(const MelLib::Vector3& pos, const MelLib::Vector3& slushVector, const MelLib::Vector3& moveDirection,const unsigned int frame, const MelLib::Vector3& direction)
{
	SetPosition(pos);

	// frameに応じて範囲を調整
	static const unsigned int X_AREA = 5;
	unsigned int areaStart = 0;
	if (frame >= 10)areaStart = X_AREA * 10;
	else areaStart = X_AREA * frame;

	
	// 刀のベクトルを元に角度を設定
	// プレイヤーの向きも反映させる

	static const MelLib::Vector3 START_ANGLE[2] = { MelLib::Vector3(7,0,0),MelLib::Vector3(-7,0,0) };
	
	// 移動量 プレイヤーの向きに応じて回転させる
	MelLib::Vector3 movePosition[2] = { MelLib::Vector3(0,0,-2),MelLib::Vector3(0,0,2) };


	// まずはプレイヤーの向きに応じて回転
	MelLib::Vector3 angle;
	//angle.y = -MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), true);

	// 剣のベクトルをXYZごとに適応
	// X方向に向くほどZ軸を、
	// Z方向に向くほどY軸の回転角度を加算
	angle.z += MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(slushVector.x, slushVector.y), true) - 90;
	angle.y += MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(slushVector.z, slushVector.y), true) - 90;

	// X方向に振る場合、回転
	angle.y += MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(moveDirection.x, moveDirection.y), true);

	// Z方向に振る場合、回転

	for (int i = 0; i < _countof(sprite); i++)
	{
		// 移動量を回転させる
		movePosition[i] = 
			MelLib::LibMath::RotateVector3(movePosition[i], MelLib::Vector3(0, 1, 0), MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), true));

		sprite[i].Create(MelLib::Texture::Get("slushEffect"));
		sprite[i].SetPosition(pos + movePosition[i]);
	
		sprite[i].SetDrawLeftUpPosition(MelLib::Vector2(areaStart, 0));
		sprite[i].SetDrawRigthDownPosition(MelLib::Vector2(areaStart + X_AREA, sprite[i].GetTexture()->GetTextureSize().y));

		sprite[i].SetAngle(angle + START_ANGLE[i]);
		
		sprite[i].SetScale(MelLib::Vector2(10,30));

	}

	// スプライトだと、Z方向を向いたときに見えない
	// あと剣の進行方向も考慮して回転させないといけない
	// 同じ方向向いてても、上下と左右で違ってくる
	// トーラス薄くしたモデル(円柱を切り取ってつぶしたものも可)を作って、時間送れるようにして、表示範囲テクスチャで透明度指定すればいい感じになる?
	// 仮で作るんだったら、切り取ったトーラス用意してテクスチャとマテリアル複数個用意してフレームに応じて切り替えてもよさそう
	// 剣の角度によってはスプライトでも問題ない可能性があるから、スプライトダメだったら試す感じでもいいかもしれない
	// ゲームの動画を見て参考にしたほうがいいかも

	// スプライト2つを剣にかぶせるように八の字に並べたらトーラスかぶせた時と同じようになるのでは?

	// 剣の進行方向と剣の向きどっちも必要

	// 剣の面の向き取得したほうがいい?

	// いったん保留
}

void SlushEffect::Update()
{
	subAlpha += 10.0f;
	if (subAlpha >= 100.0f)eraseManager = true;

	for (int i = 0; i < _countof(sprite); i++) 
	{
		sprite[i].SetSubColor(MelLib::Color(0, 0, 0, subAlpha));
	}
}

void SlushEffect::Draw()
{
	for (int i = 0; i < _countof(sprite); i++)
	{
		sprite[i].Draw();
	}
}
