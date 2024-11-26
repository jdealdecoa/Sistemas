#include "Bannana.h"

Json::Value Bannana::Code()
{
	Json::Value json = Fruta::Code();

	json["sex"] = sex;
	//json[GetDecodeKey()] = typeid(Bannana).name();
	CodeSubClassType<Bannana>(json);
	return json;
}

void Bannana::Decode(Json::Value json)
{
	Fruta::Decode(json);
	sex = (Sexo)json["sex"].asInt();
}