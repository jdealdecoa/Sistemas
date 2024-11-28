#pragma once
#include "3Nodes/NodeMap.h"
#include <list>

class Map {
public:
    Map();
    ~Map();

    void Initialize(Vector2 size, Vector2 offset);
    void Draw(NodeMap* nodeMap, const Vector2& playerPosition, const std::list<Vector2>& enemyPositions);
    bool IsValidMove(const Vector2& position);

    NodeMap* GetNodeMap();

private:
    NodeMap* _nodeMap;
};