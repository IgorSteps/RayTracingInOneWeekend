#include <iostream>
#include "colour.h"
#include "vec3.h"

int main()
{
	// image.
	const int imageWidth = 256;
	const int imageHeight = 256;

	// render.
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int j = imageHeight - 1; j >= 0; --j) 
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < imageWidth; ++i) 
		{
			color pixelColour(double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0.25);
			writeColour(std::cout, pixelColour);
		}
	}

	std::cerr << "\nDone.\n";
}
