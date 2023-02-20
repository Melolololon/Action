#pragma once
#include<string>
#include"Sky.h"

class Game
{
private:
	Game(){}
	~Game(){}

	const std::string texturePath = "Resources/Texture/";

	std::unique_ptr<Sky> pSky;

public:
	Game(Game& ins) = delete;
	Game& operator=(Game& ins) = delete;
	static Game* GetInstance();

	void Run();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	enum class ResourcePath
	{
		TEXTURE,
		MODEL,
	};
	std::string GetPath(const ResourcePath type)const;
};

