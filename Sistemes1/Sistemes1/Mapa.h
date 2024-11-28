#pragma once
#include "3Nodes/NodeMap.h"
#include <list>
#include "Player.h"

class Mapa {
public:

    Vector2 worldPos;
    Vector2 worldSize;

    Mapa();

    ~Mapa();

    void Initialize(Vector2 size, Vector2 offset, Vector2 worldPos, Vector2 wSize);
    void Draw(NodeMap* nodeMap);
    bool IsValidMove(const Vector2& position);

    NodeMap* GetNodeMap();
    Vector2 GetMapOffset();

private:
    NodeMap* _nodeMap;
    Vector2 mapOffset;

};