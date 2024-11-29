#include "Node.h"
#include "../Utils/ConsoleControl.h"
void Node::SetContent(INodeContent* newContent)
{
	if (_content != newContent) {
		if (_content != nullptr) {
			delete _content; 
		}
		_content = newContent; 
	}
}
void Node::DrawContent(Vector2 offset)
{
	_content->Draw(offset);
}

void Node::Lock()
{
	_classMutex.lock();
}

void Node::Unlock()
{
	_classMutex.unlock();
}

Node::Node(Vector2 position)
{
	_position = position;
}

Vector2 Node::GetPosition()
{
	return _position;
}


INodeContent* Node::GetContent() {
	return _content;
}