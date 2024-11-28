#pragma once
#include "3Nodes/Vector2.h"
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"

enum Weapons {
	SWORD,
	LANCE

};

class Player : public INodeContent
{
private:
	int lives;
	int coins;
	int healt;
	int potionsAmount;

public:
	Vector2 position;
	Player(Vector2 p);


	int GetLives();
	int GetCoins();
	int GetHealth();
	int GetPotionAmount();


	void Draw(Vector2 offset) override {
		CC::SetColor(CC::WHITE, CC::BLACK);
		std::cout << GetDisplayType(DisplayType::PLAYER);
		CC::SetColor(CC::WHITE, CC::BLACK);
	}
};