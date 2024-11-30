

#include "Mapa.h"
#include "3Nodes/NodeMap.h"
#include "Utils/ConsoleControl.h"
#include "3Nodes/Vector2.h"
#include "Wall.h"
#include "Portal.h"
#include "Empty.h"

Mapa::Mapa() : _nodeMap(nullptr) {}

Mapa::~Mapa() {

}

void Mapa::Initialize(Vector2 size, Vector2 offset, Vector2 worldPos, Vector2 wSize) {
    this->worldPos = worldPos;
    this->worldSize = wSize;
    this->mapOffset = offset;

    _nodeMap = new NodeMap(size, offset);

    //Inicializar nodos con contenido
    Portal portalSetter(DisplayType::PORTAL);
    portalSetter.CalculateLimits(worldPos, worldSize);
    for (int x = 0; x < size.X; ++x) {
        for (int y = 0; y < size.Y; ++y) {
            Vector2 position(x, y);
            _nodeMap->SafePickNode(position, [&](Node* node) {
                if (x == 0 || x == size.X - 1 || y == 0 || y == size.Y - 1) {
                    node->SetContent(new Wall(DisplayType::WALL));
                }
                else
                {
                    node->SetContent(new Empty(DisplayType::ENEMY));
                }
                portalSetter.SetPortals(node, position, size);
                });
        }
    }

}

void Mapa::Draw(NodeMap* nodeMap) {
    Vector2 mapSize = nodeMap->GetSize();

    for (int x = 0; x < mapSize.X; x++) {
        for (int y = 0; y < mapSize.Y; y++) {
            Vector2 position(x,y);

            nodeMap->SafePickNode(position, [&](Node* node) {
                CC::Lock();
                CC::SetPosition(position.X + mapOffset.X, position.Y + mapOffset.Y);

               if (node->GetContent()->nodeDisplay != DisplayType::PLAYER && node->GetContent()->nodeDisplay != DisplayType::ENEMY) {
                    node->GetContent()->Draw(Vector2(0, 0));
                }

                CC::Unlock();
                });
        }
    }
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

Vector2 Mapa::GetMapOffset()
{
    return mapOffset;
}
