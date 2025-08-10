#pragma once

#include <vector>
#include <bitset>

class ColorList
{
public:
	std::vector<unsigned char> colors; //does not store a value. A value will always be 255
	std::vector<int> groups;

	ColorList();

	void removeRepeatColors();
	void removeRepeatFast();
	void cutList(int axis);
	std::vector<unsigned char> averageColorList(int numberOfColors);
	std::vector<unsigned char> averageColorList256();

};