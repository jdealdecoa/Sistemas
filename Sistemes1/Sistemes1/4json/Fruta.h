#pragma once
#include "ICodable.h"
#include <iostream>
class Fruta : public ICodable
{
public:
	unsigned int semillas = 0;
	std::string hexColor = "";

	// Inherited via ICodable
	Json::Value Code() override;
	void Decode(Json::Value json) override;
};

