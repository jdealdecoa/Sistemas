#pragma once
#include "Vector2.h"
#include <iostream>
enum class DisplayType 
{
	PLAYER,
	ENEMY,
	WALL,
	PORTAL,
	CHEST,
	EMPTY
};

class INodeContent
{
public:
	DisplayType nodeDisplay;

	INodeContent(DisplayType type) : nodeDisplay(type) {};

	std::string GetDisplayType(DisplayType type) 
	{
		switch (type)
		{
		case DisplayType::PLAYER:
			return "J";
			break;
		case DisplayType::ENEMY:
			return "E";
			break;
		case DisplayType::WALL:
			return "#";
			break;
		case DisplayType::PORTAL:
			return "P";
			break;
		case DisplayType::CHEST:
			return "C";
		case DisplayType::EMPTY:
			return " ";
			break;
		default:
			break;
		}

	}
	virtual void Draw(Vector2 offset) = 0;
};

