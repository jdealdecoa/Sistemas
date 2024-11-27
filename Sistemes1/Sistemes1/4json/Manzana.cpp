//#include "Manzana.h"
//
//Json::Value Manzana::Code()
//{
//	Json::Value json = Fruta::Code();
//	json["isPoisioned"] = isPoisioned;
//	json[GetDecodeKey()] = typeid(Manzana).name();
//
//	return json;
//}
//
//void Manzana::Decode(Json::Value json)
//{
//	Fruta::Decode(json);
//	isPoisioned = json["isPoisioned"].asInt();
//}
