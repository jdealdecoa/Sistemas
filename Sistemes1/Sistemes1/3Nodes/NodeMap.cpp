#include "NodeMap.h"

Node* NodeMap::UnSafeGetNode(Vector2 position)
{
	if (position.X > _size.X || position.Y > _size.Y) return nullptr;
	
	NodeColumn* column = _grid[position.X];
	Node* node = (*column)[position.Y];
	return node;

}

NodeMap::NodeMap(Vector2 size, Vector2 offset) {
	_size = size;
	_offset = offset;
	for (int x = 0; x < size.X; x++) {
		NodeColumn* column = new NodeColumn();

		for (int y = 0; y < _size.Y; y++) {
			column->push_back(new Node(Vector2(x, y)));
		}
		_grid.push_back(column);
	}
}

Vector2 NodeMap::GetSize()
{
	_sizeMutex.lock();
	Vector2 size = _size;
	_sizeMutex.unlock();
	return size;
}


void NodeMap::UnSafeDraw() {
	for (NodeColumn* column : _grid) {
		for(Node* node : *column)
		node->DrawContent(_offset);
	}
}

void NodeMap::SafePickNode(Vector2 position, SafePick safePickAction)
{
	_sizeMutex.lock();
	_gridMutex.lock();
	Node* node = UnSafeGetNode(position);
	_gridMutex.unlock();
	_sizeMutex.unlock();

	node->Lock();
	safePickAction(node);
	node->Unlock();
}

void NodeMap::SafeMultiPickNode(std::list<Vector2> positions, SafeMultiPick safeMultiPickAction)
{
	std::list<Node*> nodes = std::list<Node*>();
	_sizeMutex.lock();
	_gridMutex.lock();

	for (Vector2 pos : positions) {
		nodes.push_back(UnSafeGetNode(pos));
	}

	_gridMutex.unlock();
	_sizeMutex.unlock();

	_safeMultiNodeLockMutex.lock();
	for (Node* node : nodes) {
		if (node != nullptr) {
			node->Lock();
		}
	}
	_safeMultiNodeLockMutex.unlock();

	safeMultiPickAction(nodes);
	for (Node* node : nodes) {
		if (node != nullptr) {
			node->Unlock();
		}
	}

}

Json::Value NodeMap::Code() {
	Json::Value json;


	json["size"]["x"] = _size.X;
	json["size"]["y"] = _size.Y;


	json["offset"]["x"] = _offset.X;
	json["offset"]["y"] = _offset.Y;


	Json::Value nodesJson;
	for (int x = 0; x < _size.X; ++x) {
		for (int y = 0; y < _size.Y; ++y) {
			Node* node = UnSafeGetNode(Vector2(x, y));
			if (node != nullptr) {
				nodesJson[x][y] = node->Code();
			}
		}
	}
	json["nodes"] = nodesJson;

	CodeSubClassType<NodeMap>(json); 
	return json;
}

void NodeMap::Decode(Json::Value json) {

	_size = Vector2(json["size"]["x"].asInt(), json["size"]["y"].asInt());

	_offset = Vector2(json["offset"]["x"].asInt(), json["offset"]["y"].asInt());


	for (NodeColumn* column : _grid) {
		for (Node* node : *column) {
			delete node;
		}
		delete column;
	}
	_grid.clear();
		

	Json::Value nodesJson = json["nodes"];
	for (int x = 0; x < _size.X; ++x) {
		NodeColumn* column = new NodeColumn();
		for (int y = 0; y < _size.Y; ++y) {
			Node* node = new Node(Vector2(x, y));
			if (!nodesJson[x][y].isNull()) {
				node->Decode(nodesJson[x][y]);
			}
			column->push_back(node);
		}
		_grid.push_back(column);
	}
}


