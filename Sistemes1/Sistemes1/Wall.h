#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"

class Wall : public INodeContent {
public:
    Wall(DisplayType type) : INodeContent(type) {};

    ~Wall() = default;

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::RED, CC::BLACK);
        std::cout << GetDisplayType(DisplayType::WALL);
        CC::SetColor(CC::WHITE, CC::BLACK);
    }


	Json::Value Code() const override{
		Json::Value json;
		json["type"] = static_cast<int>(nodeDisplay);
		return json;
    }
	void Decode(Json::Value json) override{
		nodeDisplay = static_cast<DisplayType>(json["type"].asInt());
    }
};