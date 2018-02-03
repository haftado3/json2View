#pragma once
#include <fstream>
#include "json.hpp"
class myclass
{
public:
	myclass(std::ifstream file);
private:
	using json = nlohmann::json;
	item* ptr;
};

inline myclass::myclass(std::ifstream file)
{
	json myJ = json::parse(file);
}
