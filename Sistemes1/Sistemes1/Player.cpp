#include "Player.h"

Player::Player(Vector2 p) : lives(3), coins(0), healt(100), potionsAmount(3)
{
    this->position = p;
}

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

