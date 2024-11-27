#pragma once
#include "3Nodes/NodeMap.h"
#include <list>

class Map {
public:
    Map();
    ~Map();

    /// Inicializa el mapa con dimensiones y un offset
    void Initialize(Vector2 size, Vector2 offset);

    /// Dibuja el mapa en la consola
    void Draw(NodeMap* nodeMap, const Vector2& playerPosition, const std::list<Vector2>& enemyPositions);

    bool IsValidMove(const Vector2& position);

    NodeMap* GetNodeMap();

private:
    NodeMap* _nodeMap;
};