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

    void Initialize(Vector2 size, Player* player, Vector2 offset, Vector2 worldPos, Vector2 wSize);
    void Draw(NodeMap* nodeMap, const std::list<Vector2>& enemyPositions);
    bool IsValidMove(const Vector2& position);

    NodeMap* GetNodeMap();

private:
    NodeMap* _nodeMap;
    Vector2 mapOffset;

};