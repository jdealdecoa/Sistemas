#pragma once
#include "3Nodes/NodeMap.h"
#include <list>
#include "Player.h"
#include "4Json/ICodable.h"


class Mapa : public ICodable {
public:

    Vector2 worldPos;
    Vector2 worldSize;

    Mapa();

    ~Mapa();

    void Initialize(Vector2 size, Vector2 offset, Vector2 worldPos, Vector2 wSize);
    void Draw(NodeMap* nodeMap);

    NodeMap* GetNodeMap();
    Vector2 GetMapOffset();

	Json::Value Code() override;
	void Decode(Json::Value json) override;

private:
    NodeMap* _nodeMap;
    Vector2 mapOffset;

};