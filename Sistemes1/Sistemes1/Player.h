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
	int maxHealt = 100;
	int potionsAmount;
	int healAmount = 20;

public:
	Vector2 position;
	Player(Vector2 p, DisplayType type);


	int GetLives();
	int GetCoins();
	int GetHealth();
	int GetPotionAmount();

	void addOnePotion();

	void UsePotion();

	void Draw(Vector2 offset) override {
		CC::SetColor(CC::WHITE, CC::BLACK);
		std::cout << GetDisplayType(DisplayType::PLAYER);
		CC::SetColor(CC::WHITE, CC::BLACK);
	}

	Json::Value Code() const override;
	void Decode(Json::Value json) override;
};