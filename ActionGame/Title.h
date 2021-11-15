#pragma once
#include <Scene.h>

// タイトルとかのスプライト(シーン共通じゃないスプライト)は、GameObject2Dで作ること

class Title :
    public MelLib::Scene
{
private:
	void LoadResources();

public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン
};

