#include "INodeContent.h"
#include "../Wall.h"
#include "../Empty.h"
#include "../Portal.h"
#include "../Chest.h"
#include "../Loot.h"


INodeContent* INodeContent::CreateFromJson(Json::Value json) {
	if (!json.isMember("type")) {
		return nullptr; 
	}

	DisplayType type = static_cast<DisplayType>(json["type"].asInt());

	switch (type) {
	case DisplayType::WALL: {
		Wall* wall = new Wall(DisplayType::WALL);
		wall->Decode(json);
		return wall;
	}
	case DisplayType::EMPTY: {
		Empty* empty = new Empty(DisplayType::EMPTY);
		empty->Decode(json);
		return empty;
	}
	case DisplayType::PORTAL: {
		Portal* portal = new Portal(DisplayType::PORTAL, Vector2(0,0));
		portal->Decode(json);
		return portal;
	}
	case DisplayType::CHEST: {
		Chest* chest = new Chest(DisplayType::CHEST);
		chest->Decode(json);
		return chest;
	}
	case DisplayType::LOOT: {
		Loot* loot = new Loot(DisplayType::LOOT, 0);
		loot->Decode(json);
		return loot;
	}
	// case DisplayType::CHEST: {
	// 	Chest* chest = new Chest(DisplayType::CHEST);
	// 	chest->Decode(json);
	// 	return chest;
	// }
	default:
		return nullptr;
	}
}