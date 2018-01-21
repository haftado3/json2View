#pragma once
#include "json.h"
#define ARRAYSIZE 100 //maximum size of my array for saving json data
using json = nlohmann::json;
using namespace std;
struct item {
	int LocationX;
	int LocationY;
	int Width;
	int Height;
	int Padding;
	int Margin;
	string ComponentType;
	string TextAlignment;
	string Icon;
	string TextColor;
	string BackColor;
	string ClickEvent;
	string RightClickEvent;
	string Font;
	string Name;
	string Text;
	bool Disabled;
	bool Visible;
};
class jsonParser
{

public:
	item items[ARRAYSIZE], *iPtr;
	item* initialize(json j);
	void parse_json(json j, item items[]);
};

item* jsonParser::initialize(json j) {
	iPtr = items;
	jsonParser::parse_json(j, items);
	return iPtr;
}
int i = 0;
void jsonParser::parse_json(json j, item items[])
{
	int temp = 1;
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		json j2 = it.value();
		string s2 = j2.type_name();
		if (s2.compare("object") == 0)
		{
			temp++;
			parse_json(it.value(), items);
		}
		else {
			items[i].ComponentType = j["ComponentType"].get<std::string>();
			items[i].Name = j["Name"].get<std::string>();
			items[i].LocationX = j["LocationX"].get<int>();
			items[i].LocationY = j["LocationY"].get<int>();
			items[i].Width = j["Width"].get<int>();
			items[i].Height = j["Height"].get<int>();
			items[i].TextAlignment = j["TextAlignment"].get<std::string>();
			items[i].Disabled = j["Disabled"].get<bool>();
			items[i].Visible = j["Visible"].get<bool>();
			items[i].Text = j["Text"].get<std::string>();
			//items[i].Icon = j["Icon"].get<std::string>();
			//items[i].BackColor = j["BackColor"].get<std::string>();
			//items[i].ClickEvent = j["ClickEvent"].get<std::string>();
			//items[i].Font = j["Font"].get<std::string>();
			//items[i].Margin = j["Margin"].get<int>();
			//items[i].Padding = j["Padding"].get<int>();
			//items[i].RightClickEvent = j["RightClickEvent"].get<std::string>();
			//need try catch for this guys cause they maybe empty at all



			//cout << "ComponentType : " << items[i].ComponentType << endl;
			//cout << "Text :" << items[i].Text << endl;
			//cout << "LocationY :" << items[i].LocationY << endl;
			//cout << "Width :" << items[i].Width << endl;
			//cout << "Height :" << items[i].Height << endl;
			//cout << "TextAlignment :" << items[i].TextAlignment << endl;
			//cout << "Disabled :" << items[i].Disabled << endl;
			//cout << "Visible :" << items[i].Visible << endl;
			//cout << endl;
			i++;
			return;
		}
	}
}

