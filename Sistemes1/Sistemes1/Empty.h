#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"

class Empty : public INodeContent {
public:

    Empty(DisplayType type) : INodeContent(type) {};

    ~Empty() = default;

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::WHITE, CC::BLACK);
        std::cout << GetDisplayType(DisplayType::EMPTY);
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