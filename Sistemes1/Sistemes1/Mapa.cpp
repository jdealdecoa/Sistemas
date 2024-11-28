

#include "Mapa.h"
#include "3Nodes/NodeMap.h"
#include "Utils/ConsoleControl.h"
#include "3Nodes/Vector2.h"
#include "Wall.h"
#include "Portal.h"

Mapa::Mapa() : _nodeMap(nullptr) {}

Mapa::~Mapa() {

}

void Mapa::Initialize(Vector2 size, Player* player, Vector2 offset, Vector2 worldPos, Vector2 wSize) {
    this->worldPos = worldPos;
    this->worldSize = wSize;
    this->mapOffset = offset;
    if (_nodeMap != nullptr) {
        delete _nodeMap;
    }
    _nodeMap = new NodeMap(size, offset);

    //Inicializar nodos con contenido
    Portal portalSetter;
    CC::Lock();
    CC::SetPosition(30, 0);
    std::cout << worldPos.X << " : " << worldPos.Y;
    CC::Unlock();
    portalSetter.CalculateLimits(worldPos, worldSize);
    for (int x = 0; x < size.X; ++x) {
        for (int y = 0; y < size.Y; ++y) {
            Vector2 position(x, y);
            _nodeMap->SafePickNode(position, [&](Node* node) {
                if (x == 0 || x == size.X - 1 || y == 0 || y == size.Y - 1) {
                    
                    node->SetContent(new Wall());
                }
                if (x == player->position.X && y == player->position.Y) 
                {
                    node->SetContent(player);
                }
                portalSetter.SetPortals(node, position, size);
                });
        }
    }

}

void Mapa::Draw(NodeMap* nodeMap, const std::list<Vector2>& enemyPositions) {
    Vector2 mapSize = nodeMap->GetSize();

    for (int x = 0; x < mapSize.X; x++) {
        for (int y = 0; y < mapSize.Y; y++) {
            Vector2 position(x,y);

            nodeMap->SafePickNode(position, [&](Node* node) {
                CC::Lock();
                CC::SetPosition(position.X + mapOffset.X, position.Y + mapOffset.Y);

                if (std::find(enemyPositions.begin(), enemyPositions.end(), position) != enemyPositions.end()) {
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
    CC::Lock();
    CC::SetPosition(30, 0);
    std::cout << worldPos.X << " : " << worldPos.Y;
    CC::Unlock();
}

bool Mapa::IsValidMove(const Vector2& position)
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

NodeMap* Mapa::GetNodeMap()
{
    return _nodeMap;
}
