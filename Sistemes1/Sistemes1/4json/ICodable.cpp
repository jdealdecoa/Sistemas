#include "ICodable.h"

ICodable::DecodeMap* ICodable::GetDecodeMap()
{
	static DecodeMap* map = new DecodeMap();
	return map;
}

std::string ICodable::GetDecodeKey()
{
	return "ICodableType";
}

void ICodable::SaveDecodeProces(std::string className, SubclassDecode decodeProcess)
{
}
