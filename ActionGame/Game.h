#pragma once
class Game
{
private:
	Game(){}
	~Game(){}
public:
	Game(Game& ins) = delete;
	Game& operator=(Game& ins) = delete;
	static Game* GetInstance();

	void Run();
	void Initialize();
	void Finitialize();
	void Update();
	void Draw();
};

