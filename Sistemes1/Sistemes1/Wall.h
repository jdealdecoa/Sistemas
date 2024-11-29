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

};