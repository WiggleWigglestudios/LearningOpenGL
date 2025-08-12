#include <iostream>
#include "ColorList.h"

ColorList::ColorList()
{
	colors = std::vector<unsigned char>();
	groups.resize(1);
	groups[0] = 0;
}

void ColorList::removeRepeatColors()
{
	for (int i = 0; i < ColorList::colors.size(); i += 3)
	{
		for (int c = i + 3; c < ColorList::colors.size(); c += 3)
		{
			if (ColorList::colors[i] == ColorList::colors[c] &&
				ColorList::colors[i + 1] == ColorList::colors[c + 1] &&
				ColorList::colors[i + 2] == ColorList::colors[c + 2])
			{
				ColorList::colors.erase(ColorList::colors.begin() + c);
				ColorList::colors.erase(ColorList::colors.begin() + c);
				ColorList::colors.erase(ColorList::colors.begin() + c);
				c -= 3;
			}
		}
	}

}

void ColorList::removeRepeatFast()
{
	static std::bitset<256 * 256 * 256> seen;
	seen.reset();
	int writes = 0;
	for (int i = 0; i < colors.size(); i += 3)
	{
		unsigned int colorKey = (static_cast<unsigned int>(colors[i]) << 16) |
			(static_cast<unsigned int>(colors[i + 1]) << 8) |
			(static_cast<unsigned int>(colors[i + 2]));
		if (!seen[colorKey])
		{
			seen[colorKey] = true;
			colors[writes++] = colors[i];
			colors[writes++] = colors[i + 1];
			colors[writes++] = colors[i + 2];

		}
	}
	colors.resize(writes);
}

void ColorList::cutList(int axis)
{

	axis = axis % 3;

	for (int i = 0; i < ColorList::groups.size(); i++)
	{
		int lowerBound = ColorList::groups[i];
		int upperBound = 0;
		if (i + 1 == ColorList::groups.size())
		{
			upperBound = ColorList::colors.size() / 3;
		}
		else
		{
			upperBound = ColorList::groups[i + 1];
		}
		for (int c = lowerBound; c < upperBound; c++)
		{
			for (int d = c + 1; d < upperBound; d++)
			{
				if (ColorList::colors[c * 3 + axis] < ColorList::colors[d * 3 + axis])
				{
					unsigned char tempR = ColorList::colors[c * 3];
					unsigned char tempG = ColorList::colors[c * 3 + 1];
					unsigned char tempB = ColorList::colors[c * 3 + 2];
					ColorList::colors[c * 3] = ColorList::colors[d * 3];
					ColorList::colors[c * 3 + 1] = ColorList::colors[d * 3 + 1];
					ColorList::colors[c * 3 + 2] = ColorList::colors[d * 3 + 2];
					ColorList::colors[d * 3] = tempR;
					ColorList::colors[d * 3 + 1] = tempG;
					ColorList::colors[d * 3 + 2] = tempB;
				}
			}
		}
	}

	std::vector<int> newGroups = std::vector<int>();

	for (int i = 0; i < ColorList::groups.size(); i++)
	{
		int lowerBound = ColorList::groups[i];
		int upperBound = 0;
		if (i + 1 == ColorList::groups.size())
		{
			upperBound = ColorList::colors.size() / 3;
		}
		else
		{
			upperBound = ColorList::groups[i + 1];
		}
		newGroups.push_back(lowerBound);
		newGroups.push_back(floor((upperBound + lowerBound) / 2));
	}

	ColorList::groups = newGroups;
}


std::vector<unsigned char> ColorList::averageColorList(int numberOfColors)
{
	numberOfColors = pow(2, floor(log2(numberOfColors)));

	if (ColorList::colors.size() / 3 > numberOfColors)
	{
		ColorList::groups.resize(1);
		groups[0] = 0;
		for (int i = 0; i < log2(numberOfColors); i++)
		{
			this->cutList(i % 3);
		}
		std::vector<unsigned char> outputColors = std::vector<unsigned char>();
		for (int i = 0; i < ColorList::groups.size(); i++)
		{
			int lowerBound = ColorList::groups[i];
			int upperBound = 0;
			if (i + 1 == ColorList::groups.size())
			{
				upperBound = ColorList::colors.size() / 3;
			}
			else
			{
				upperBound = ColorList::groups[i + 1];
			}
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(255);
			double r = 0;
			double b = 0;
			double g = 0;
			for (int c = lowerBound; c < upperBound; c++)
			{
				r += double(ColorList::colors[c * 3]);
				g += double(ColorList::colors[c * 3 + 1]);
				b += double(ColorList::colors[c * 3 + 2]);
			}
			outputColors[i * 4] = r / double(upperBound - lowerBound);
			outputColors[i * 4 + 1] = g / double(upperBound - lowerBound);
			outputColors[i * 4 + 2] = b / double(upperBound - lowerBound);
		}


		return outputColors;
	}
	else
	{
		int colorsMissing = numberOfColors - (ColorList::colors.size() / 3);
		std::vector<unsigned char> outputColors = std::vector<unsigned char>();

		for (int i = 0; i < ColorList::colors.size(); i += 3)
		{
			outputColors.push_back(ColorList::colors[i]);
			outputColors.push_back(ColorList::colors[i + 1]);
			outputColors.push_back(ColorList::colors[i + 2]);
			outputColors.push_back(255);
		}


		for (int i = 0; i < colorsMissing; i++)
		{
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(255);
		}


		return outputColors;
	}

	//return NULL;
}


std::vector<unsigned char> ColorList::averageColorList256()
{
	if (ColorList::colors.size() / 3 > 256)
	{
		ColorList::groups.resize(1);
		groups[0] = 0;
		for (int i = 0; i < 8; i++)
		{
			this->cutList(i % 3);
		}

		std::vector<unsigned char> outputColors = std::vector<unsigned char>();
		for (int i = 0; i < ColorList::groups.size(); i++)
		{
			int lowerBound = ColorList::groups[i];
			int upperBound = 0;
			if (i + 1 == ColorList::groups.size())
			{
				upperBound = ColorList::colors.size() / 3;
			}
			else
			{
				upperBound = ColorList::groups[i + 1];
			}
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(255);
			double r = 0;
			double b = 0;
			double g = 0;
			for (int c = lowerBound; c < upperBound; c++)
			{
				r += double(ColorList::colors[c * 3]);
				g += double(ColorList::colors[c * 3 + 1]);
				b += double(ColorList::colors[c * 3 + 2]);
			}
			outputColors[i * 4] = r / double(upperBound - lowerBound);
			outputColors[i * 4 + 1] = g / double(upperBound - lowerBound);
			outputColors[i * 4 + 2] = b / double(upperBound - lowerBound);
		}

		float distance = 100000;
		int closest = 1;
		for (int i = 1; i < 256; i++)
		{
			float currentDistance = sqrt((float(outputColors[0]) - float(outputColors[i * 4])) * (float(outputColors[0]) - float(outputColors[i * 4])) +
				(float(outputColors[1]) - float(outputColors[i * 4 + 1])) * (float(outputColors[1]) - float(outputColors[i * 4 + 1])) +
				(float(outputColors[2]) - float(outputColors[i * 4 + 2])) * (float(outputColors[2]) - float(outputColors[i * 4 + 2])));


			if (currentDistance < distance)
			{
				distance = currentDistance;
				closest = i;
			}
		}
		outputColors[closest] /= 2;
		outputColors[closest] += outputColors[0] / 2;
		outputColors[closest + 1] /= 2;
		outputColors[closest + 1] += outputColors[1] / 2;
		outputColors[closest + 2] /= 2;
		outputColors[closest + 2] += outputColors[2] / 2;





		return outputColors;
	}
	else if ((ColorList::colors.size() / 3) == 256)
	{
		float distance = 100000;
		int closest = 1;
		for (int i = 1; i < 256; i++)
		{
			float currentDistance = sqrt((float(ColorList::colors[0]) - float(ColorList::colors[i * 3])) * (float(ColorList::colors[0]) - float(ColorList::colors[i * 3])) +
				(float(ColorList::colors[1]) - float(ColorList::colors[i * 3 + 1])) * (float(ColorList::colors[1]) - float(ColorList::colors[i * 3 + 1])) +
				(float(ColorList::colors[2]) - float(ColorList::colors[i * 3 + 2])) * (float(ColorList::colors[2]) - float(ColorList::colors[i * 3 + 2])));
			if (currentDistance < distance)
			{
				distance = currentDistance;
				closest = i;
			}
		}

		ColorList::colors[closest * 3] /= 2;
		ColorList::colors[closest * 3] += ColorList::colors[0] / 2;
		ColorList::colors[closest * 3 + 1] /= 2;
		ColorList::colors[closest * 3 + 1] += ColorList::colors[1] / 2;
		ColorList::colors[closest * 3 + 2] /= 2;
		ColorList::colors[closest * 3 + 2] += ColorList::colors[2] / 2;


		std::vector<unsigned char> outputColors = std::vector<unsigned char>();

		outputColors.push_back(0);
		outputColors.push_back(0);
		outputColors.push_back(0);
		outputColors.push_back(255);

		for (int i = 3; i < ColorList::colors.size(); i += 3)
		{
			outputColors.push_back(ColorList::colors[i]);
			outputColors.push_back(ColorList::colors[i + 1]);
			outputColors.push_back(ColorList::colors[i + 2]);
			outputColors.push_back(255);
		}
		return outputColors;
	}
	{
		int colorsMissing = 255 - (ColorList::colors.size() / 3);
		std::vector<unsigned char> outputColors = std::vector<unsigned char>();

		outputColors.push_back(0);
		outputColors.push_back(0);
		outputColors.push_back(0);
		outputColors.push_back(255);

		for (int i = 0; i < ColorList::colors.size(); i += 3)
		{
			outputColors.push_back(ColorList::colors[i]);
			outputColors.push_back(ColorList::colors[i + 1]);
			outputColors.push_back(ColorList::colors[i + 2]);
			outputColors.push_back(255);
		}


		for (int i = 1; i < colorsMissing; i++)
		{
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(0);
			outputColors.push_back(255);
		}


		return outputColors;
	}

	//return NULL;
}
