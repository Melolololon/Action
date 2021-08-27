#pragma once
#include<string>

class Game
{
private:
	Game(){}
	~Game(){}

	const std::string texturePath = "Resources/Texture/";
public:
	Game(Game& ins) = delete;
	Game& operator=(Game& ins) = delete;
	static Game* GetInstance();

	void Run();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	enum class ResourcePathType
	{
		TEXTURE,
	};
	std::string GetPath(const ResourcePathType type)const;
};

