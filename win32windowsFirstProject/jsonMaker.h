#pragma once
#include "json.hpp"
using namespace std;

class jsonMaker {
public:
	static void makeJson(item *items, int current, HWND hwnd);

};

inline void jsonMaker::makeJson(item *items, int current, HWND hwnd) {
	json j;
	j["Return Buttons Name"] = {};
	if ("returnBtnsName" == items[current].ClickEvent) {
		OutputDebugString(L"\n j[eventName]=  givin value from pointer");
		const json j2 = {
			"text 0",
			{ "text", items[0].Name },
			"text 1",
			{ "text", items[1].Name },
			"text 2",
			{ "text", items[2].Name },
			"text 3",
			{ "text", items[3].Name },
			"text 4",
			{ "text", items[4].Name }
		};

		//json data = { "text 0", {"text",items[0].Name},"text 1",{"text",items[1].Name} };
		//j["Return Buttons Name"].insert(data.begin(), data.end());
		std::ofstream o("pretty.json");            //this line gonna make pretty json file
		o << std::setw(4) << j2 << std::endl; //and writing json file into .json format
											  //get buttons names and put into j
		MessageBox(hwnd, TEXT("pretty.json has created successfully"), TEXT("Json Created"), 0);
		//-----------json writer--------------
		//std::ofstream o("pretty.json");       //this line gonna make pretty json file
		//o << std::setw(4) << j << std::endl; //and writing json file into .json format

	}
}