#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <fstream>
#include <chrono>
#include <algorithm>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::microseconds;
using std::cout;
using std::ifstream;
using std::max;

using olc::Pixel;

class Visualizer : public olc::PixelGameEngine
{
public:
	uint32_t ACTIVATIONS;
	uint32_t RUNS;
	float* data;

	uint32_t mode;

	Visualizer(uint32_t activations, uint32_t runs, float* data)
	{
		sAppName = "Probability Visualizer";
		ACTIVATIONS = activations;
		RUNS = runs;
		this->data = data;
	}

public:
	bool OnUserCreate() override
	{
		mode = 3;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(Pixel(0, 0, 0));
		
		if (GetKey(olc::UP).bPressed) mode++;
		if (GetKey(olc::DOWN).bPressed) mode--;
		mode -= (mode == 4) * 4;
		mode += (mode == -1) * 4;

		uint32_t idx = 0;
		for (uint32_t activation = ACTIVATIONS; activation--;)
		{
			Pixel color;
			switch (activation)
			{
			case 0:
				color = Pixel(255, 0, 0);
				break;
			case 1:
				color = Pixel(0, 255, 0);
				break;
			case 2:
				color = Pixel(0, 0, 255);
				break;
			}

			for (uint32_t run = RUNS; run--;)
			{
				for (uint32_t i = ScreenWidth(); i--;)
				{
					uint32_t height = (1 - data[idx++]) * 999;
					if ((mode == 3) || (mode == activation))
						Draw(ScreenWidth() - i, height, color);
				}
			}
		}

		return true;
	}
};

int main()
{
	uint32_t ACTIVATIONS;
	uint32_t RUNS;
	uint32_t ITERATIONS;
	uint32_t SIZE;
	float* data;

	ifstream file("data.txt");
	file >> ACTIVATIONS >> RUNS >> ITERATIONS;
	SIZE = ACTIVATIONS * RUNS * ITERATIONS;

	data = new float[SIZE];
	for (int i = 0; i < SIZE; i++)
		file >> data[i];

	/*for (int i = 0; i < SIZE; i++)
		cout << data[i] << '\n';*/

	file.close();

	uint32_t totalXElements = ITERATIONS;
	uint32_t xPixels = 1920 / totalXElements;
	uint32_t totalYElements = 1000;
	uint32_t yPixels = 1080 / totalYElements;

	Visualizer visualizer(ACTIVATIONS, RUNS, data);
	if (visualizer.Construct(totalXElements, totalYElements, xPixels, yPixels))
		visualizer.Start();
	return 0;
}