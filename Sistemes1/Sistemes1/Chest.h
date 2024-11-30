#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"
#include <vector>
#include "3Nodes/Node.h"
#include "Loot.h"

class Chest : public INodeContent
{


public:
    Chest(DisplayType type) : INodeContent(type) 
    {
        SetRandomContent();
    }
    ~Chest() = default;

    int content;

    void SetRandomContent() 
    {
        content = rand() % 2;
    }

    void ReciveDamage(Node* node) 
    {
        node->SetContent(new Loot(DisplayType::LOOT, content));
    }

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::YELLOW, CC::BLACK);
        std::cout << GetDisplayType(DisplayType::CHEST);
        CC::SetColor(CC::WHITE, CC::BLACK);
    }

    Json::Value Code() const override {
        Json::Value json;
        json["type"] = static_cast<int>(nodeDisplay);
        json["content"] = static_cast<int>(content);

        return json;
    }
    void Decode(Json::Value json) override {
        nodeDisplay = static_cast<DisplayType>(json["type"].asInt());
        content = static_cast<int>(json["content"].asInt());
    }
};

