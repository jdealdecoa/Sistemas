#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"
#include <vector>
#include "3Nodes/Node.h"
enum class PortalLimits
{
    TOP,
    BOTOM,
    LEFT,
    RIGHT,
    NONE
};

class Portal : public INodeContent {
public:
    Portal(DisplayType type, Vector2 direction) : INodeContent(type), direction(direction){};
    ~Portal() = default;

    Vector2 direction;

    std::vector<PortalLimits> limits{ PortalLimits::NONE ,PortalLimits::NONE };

    void CalculateLimits(Vector2 mapPos, Vector2 worldSize)
    {
        if (mapPos.Y == 0)
        {
            limits[0] = PortalLimits::TOP;
        }
        if (mapPos.Y == worldSize.Y -1)
        {
            limits[0] = PortalLimits::BOTOM;
        }
        if (mapPos.X == 0)
        {
            limits[1] = PortalLimits::LEFT;
        }
        if (mapPos.X == worldSize.X -1)
        {
            limits[1] = PortalLimits::RIGHT;
        }
    }
    void SetPortals(Node* node, Vector2 position, Vector2 size)
    {

        if (position.X == size.X / 2 && position.Y == 0)
        {
            if (limits[0] == PortalLimits::NONE)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(0, -1)));
            }
            if (limits[0] != PortalLimits::TOP)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(0, -1)));
            }
        } 

        if (position.X == size.X / 2 && position.Y == size.Y - 1) 
        {
            if (limits[0] == PortalLimits::NONE)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(0, 1)));
            }
            if (limits[0] != PortalLimits::BOTOM)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(0, 1)));
            }
        }

        if (position.X == 0 && position.Y == size.Y / 2) 
        {
            if (limits[1] == PortalLimits::NONE)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(-1, 0)));
            }
            if (limits[1] != PortalLimits::LEFT)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(-1, 0)));
            }
        } 

        if (position.X == size.X - 1 && position.Y == size.Y / 2) 
        {
            if (limits[1] == PortalLimits::NONE)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(1, 0)));
            }
            if (limits[1] != PortalLimits::RIGHT)
            {
                node->SetContent(new Portal(DisplayType::PORTAL, Vector2(1, 0)));
            }
        }
    }

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::CYAN, CC::BLACK); 
        std::cout << GetDisplayType(DisplayType::PORTAL);
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