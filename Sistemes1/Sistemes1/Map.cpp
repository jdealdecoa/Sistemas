

#include "Map.h"
#include "3Nodes/NodeMap.h"
#include "Utils/ConsoleControl.h"
#include "3Nodes/Vector2.h"
#include "Wall.h"
#include "Portal.h"

Map::Map() : _nodeMap(nullptr) {}

Map::~Map() {
    if (_nodeMap != nullptr) {
        delete _nodeMap;
    }
}

void Map::Initialize(Vector2 size, Vector2 offset) {
    if (_nodeMap != nullptr) {
        delete _nodeMap;
    }
    _nodeMap = new NodeMap(size, offset);

    //Inicializar nodos con contenido
    for (int x = 0; x < size.X; ++x) {
        for (int y = 0; y < size.Y; ++y) {
            Vector2 position(x, y);
            _nodeMap->SafePickNode(position, [&](Node* node) {
                if (x == 0 || x == size.X - 1 || y == 0 || y == size.Y - 1) {
                    
                    node->SetContent(new Wall());
                }
                if ((x == size.X / 2 && y == 0) || (x == size.X / 2 && y == size.Y - 1)) {
                    node->SetContent(new Portal());
                }
                });
        }
    }
}

void Map::Draw(NodeMap* nodeMap, const Vector2& playerPosition, const std::list<Vector2>& enemyPositions) {
    Vector2 mapSize = nodeMap->GetSize();

    for (int x = 0; x < mapSize.X; x++) {
        for (int y = 0; y < mapSize.Y; y++) {
            Vector2 position(x, y);

            nodeMap->SafePickNode(position, [&](Node* node) {
                CC::Lock();
                CC::SetPosition(x, y);

                if (playerPosition.X == x && playerPosition.Y == y) {
                    std::cout << "P"; 
                }
                else if (std::find(enemyPositions.begin(), enemyPositions.end(), position) != enemyPositions.end()) {
                    std::cout << "E";
                }
                else if (node->GetContent() != nullptr) {
                    node->GetContent()->Draw(Vector2(0, 0));
                }
                else {
                    std::cout << " ";
                }
                CC::Unlock();
                });
        }
    }
}

bool Map::IsValidMove(const Vector2& position)
{
    if (position.X < 0 || position.X >= _nodeMap->GetSize().X ||
        position.Y < 0 || position.Y >= _nodeMap->GetSize().Y) {
        return false; 
    }

    Node* node = nullptr;
    _nodeMap->SafePickNode(position, [&](Node* n) { node = n; });

    if (node && node->GetContent() != nullptr) {
        return dynamic_cast<Wall*>(node->GetContent()) == nullptr;
    }

    return true;
}

NodeMap* Map::GetNodeMap()
{
    return _nodeMap;
}
