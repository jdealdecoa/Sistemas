

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
        delete _nodeMap; // Limpiar cualquier mapa previo
    }
    _nodeMap = new NodeMap(size, offset);

    // Inicializar nodos con contenido (opcional: paredes, portales, etc.)
    for (int x = 0; x < size.X; ++x) {
        for (int y = 0; y < size.Y; ++y) {
            Vector2 position(x, y);
            _nodeMap->SafePickNode(position, [&](Node* node) {
                if (x == 0 || x == size.X - 1 || y == 0 || y == size.Y - 1) {
                    // Paredes en los bordes
                    node->SetContent(new Wall()); // `Wall` sería una clase que implementa `INodeContent`
                }
                if ((x == size.X / 2 && y == 0) || (x == size.X / 2 && y == size.Y - 1)) {
                    node->SetContent(new Portal()); // `Portal` implementa `INodeContent`
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
                    std::cout << "P"; // Representación del jugador
                }
                else if (std::find(enemyPositions.begin(), enemyPositions.end(), position) != enemyPositions.end()) {
                    std::cout << "E"; // Representación de enemigos
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

NodeMap* Map::GetNodeMap()
{
    return _nodeMap;
}
