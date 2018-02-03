//jsonParser.h
#pragma once
#include <utility>
#include "json.hpp"
#include "converter.h"

#pragma warning(disable:4996)

using json = nlohmann::json;
using namespace std;
struct item {
	int LocationX{};
	int LocationY{};
	int Width{};
	int Height{};
	string ComponentType;
	string TextAlignment;
	string Icon;
	string TextColor;
	string BackColor;
	string ClickEvent;
	string Font;
	string Name;
	string Text;
	string Tooltip;
	LPCWSTR childs[DROPSIZE]{};
	bool Disabled{};
	bool Visible{};
	bool Newgroup{};
	bool Horizontal{};

};

class json_parser
{
	int i = 0;
	int childLocation = 0;
	item items[MAXCONTROLS];
	char text[MAXTEXT]{};
	char info[MAXTEXT]{};
	char dropItemText[MAXCONTROLS][MAXTEXT]{};
public:
	item* initialize(json j);
	void parse_json(json j, item items[]);
	char* getText(int i);
	char* getInfo(int i);
	char* getDropItem(char* saveLocation, const string& cs, int i);
	LPCWSTR* getChilds(int i);
	static void setEndChar(char* saveLocation, const string& from);
	static void setLocalChar(char* saveLocation, const string& from);
};

inline void json_parser::setLocalChar(char* saveLocation, const string& from) {
	strcpy(saveLocation, from.c_str());
}
inline void json_parser::setEndChar(char* saveLocation, const string& from)
{
	for (unsigned int q = 0; q < MAXTEXT; q++) {
		if (q >= from.length()) saveLocation[q] = '0';
	}
}

inline char* json_parser::getText(int i) {
	setLocalChar(text, items[i].Text);
	setEndChar(text, items[i].Text);
	return text;
}
inline char* json_parser::getInfo(int i) {
	setLocalChar(info, items[i].Tooltip);
	setEndChar(info, items[i].Tooltip);
	return info;
}
inline LPCWSTR* json_parser::getChilds(int i) {
	return items[i].childs;
}

inline char* json_parser::getDropItem(char* saveLocation, const string& cs, int i) {
	setLocalChar(saveLocation, cs);
	setEndChar(text, cs);
	return dropItemText[i];
}

inline item* json_parser::initialize(json j) {
	json_parser::parse_json(std::move(j), items);
	return items;
}


inline void json_parser::parse_json(json j, item items[])
{

	int temp = 1;

	for (json::iterator it = j.begin(); it != j.end(); ++it) {

		json j2 = it.value();
		const string s2 = j2.type_name();
		if (s2 == "object")
		{
			temp++;
			parse_json(it.value(), items);

		}

		else {
			try {
				if (it.key() == "Child") {
					//its an array so should use a for loop
					for (int f = 0; f < DROPSIZE; f++) {
						try {
							const string itemSTR = j["Child"].at(f);
							items[i].childs[f] = charArray2utf(getDropItem(dropItemText[childLocation], itemSTR, childLocation), i + childLocation + 1);
							childLocation++;
						}
						catch (exception e) {
							break;
						}
					}
				}

				items[i].ComponentType = j["ComponentType"].get<std::string>();
				if (items[i].ComponentType == "radiobutton")
				{
					items[i].Newgroup = j["NewGroup"].get<bool>();
				}
				items[i].Name = j["Name"].get<std::string>();
				items[i].LocationX = j["LocationX"].get<int>();
				items[i].LocationY = j["LocationY"].get<int>();
				items[i].Width = j["Width"].get<int>();
				items[i].Height = j["Height"].get<int>();
				items[i].TextAlignment = j["TextAlignment"].get<std::string>();
				items[i].Disabled = j["Disabled"].get<bool>();
				items[i].Visible = j["Visible"].get<bool>();
				items[i].Text = j["Text"].get<std::string>();
				if (items[i].Text.empty())items[i].Text = "empty";



				items[i].Icon = j["Icon"].get<std::string>();
				items[i].Tooltip = j["Tooltip"].get<std::string>();
				items[i].ClickEvent = j["ClickEvent"].get<std::string>();
				items[i].Font = j["Font"].get<std::string>();
				items[i].BackColor = j["BackColor"].get<std::string>();
			}
			catch (const std::exception& ex)
			{
				// do what ever if array cell is empty 
				_RPT1(0, "exception occured at putting values into structure.\n", ex);

			}

			i++;
			return;
		}
	}
}

