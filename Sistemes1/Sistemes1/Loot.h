#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"
#include <vector>
#include "3Nodes/Node.h"
#include "Player.h"
#include "Empty.h"

class Loot : public INodeContent
{


public:
    Loot(DisplayType type, int content) : INodeContent(type), act(content)
    {
    }
    ~Loot() = default;

    int act;

    void Act(Player* p, Node* node)
    {
        switch (act)
        {
        case 0:
            AddPotiont(p);
            break;
        case 1:
            AddMoney(p);
            break;
        default:
            break;
        }
        node->SetContent(new Empty(DisplayType::EMPTY));
    }

    void AddPotiont(Player* p) 
    {
        p->addOnePotion();
    }

    void AddMoney(Player* p)
    {
        p->add100Coins();
    }

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::YELLOW, CC::BLACK);
        std::cout << GetDisplayType(DisplayType::LOOT);
        CC::SetColor(CC::WHITE, CC::BLACK);
    }

    Json::Value Code() const override {
        Json::Value json;
        json["type"] = static_cast<int>(nodeDisplay);
        json["act"] = static_cast<int>(act);

        return json;
    }
    void Decode(Json::Value json) override {
        nodeDisplay = static_cast<DisplayType>(json["type"].asInt());
        act = static_cast<int>(json["act"].asInt());
    }
};