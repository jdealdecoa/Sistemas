#include "Player.h"

Player::Player(Vector2 p, DisplayType type) : INodeContent(type), lives(3), coins(0), healt(90), potionsAmount(3), position(p) {};

int Player::GetLives()
{
    return lives;
}

int Player::GetCoins()
{
    return coins;
}

int Player::GetHealth()
{
    return healt;
}

int Player::GetPotionAmount()
{
    return potionsAmount;
}

void Player::addOnePotion()
{
    potionsAmount++;
}

void Player::UsePotion()
{
    if (potionsAmount > 0) 
    {
        if (healt < maxHealt && healt < maxHealt - healAmount)
        {
            healt += healAmount;
            potionsAmount--;
        }
        else if (healt > maxHealt - healAmount && healt != maxHealt)
        {
            healt += maxHealt - healt;
            potionsAmount--;
        }
    }
}

Json::Value Player::Code() const {
	Json::Value json;

	// Guardar tipo de contenido
	json["type"] = static_cast<int>(nodeDisplay);

	// Guardar atributos del jugador
	json["position"]["x"] = position.X;
	json["position"]["y"] = position.Y;
	json["lives"] = lives;
	json["coins"] = coins;
	json["healt"] = healt;
	json["maxHealt"] = maxHealt;
	json["potionsAmount"] = potionsAmount;
	json["healAmount"] = healAmount;

	return json;
}
void Player::Decode(Json::Value json) {
	// Restaurar tipo de contenido
	nodeDisplay = static_cast<DisplayType>(json["type"].asInt());

	// Restaurar atributos del jugador
	position = Vector2(json["position"]["x"].asInt(), json["position"]["y"].asInt());
	lives = json["lives"].asInt();
	coins = json["coins"].asInt();
	healt = json["healt"].asInt();
	maxHealt = json["maxHealt"].asInt();
	potionsAmount = json["potionsAmount"].asInt();
	healAmount = json["healAmount"].asInt();
}
