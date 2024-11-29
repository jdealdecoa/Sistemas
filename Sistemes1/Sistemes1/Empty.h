#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"

class Empty : public INodeContent {
public:
    Empty() = default;
    ~Empty() = default;

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::WHITE, CC::BLACK);
        std::cout << GetDisplayType(DisplayType::EMPTY);
        CC::SetColor(CC::WHITE, CC::BLACK);
    }

};