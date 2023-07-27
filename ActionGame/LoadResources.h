#pragma once


// リソース読み込みクラス
class LoadResources final
{
private:
	LoadResources(LoadResources& l) = delete;
	LoadResources& operator=(LoadResources& l) = delete;

	LoadResources(){}
	~LoadResources(){}

	void LoadTexture();
	void LoadModel();
	void LoadBGM();
	void LoadSE();

public:
	static LoadResources* GetInstance();

	// 後々シーン指定して切り替えられるようにしたらいいかも
	void Load();
};

