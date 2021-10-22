#pragma once

// シーン切り替えずに表示するため、Sceneを継承してない
class Option 
{
private:
	// オプション項目
	struct OptionMenu
	{
		static const int CAMERA_SPEED = 0;
		static const int NUM_MAX = 3;

	};
	float cameraRotSpeed = 2.0f;
private:
	int currentSelect = 0;


private:
	void LoadResources();
	Option() {}
	~Option() {}
public:
	Option(Option& o) = delete;
	Option& operator= (Option& o) = delete;
	static Option* GetInstance();

	void Initialize();
	void Update();
	void Draw();
};

