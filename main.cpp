#include <iostream>
#include "mathUtils.h"

#include "colour.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"

// Linearly blends white and blue depending on the height of the y coordinate
// after scaling the ray direction to unit length(so −1.0 < y < 1.0).
color rayColor(const ray& r, const hittable& world)
{
	hitRecord rec;
	if (world.hit(r, 0, infinity, rec))
	{
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	vec3 unitDir = unitVector(r.direction());
	auto t = 0.5 * (unitDir.y() + 1.0);

	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	// image.
	const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samplesPerPixel = 100;

	// world.
	hittableList world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// camera.
	camera cam;

	// render.
	for (int j = imageHeight - 1; j >= 0; --j) 
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < imageWidth; ++i) 
		{
			color pixelColor(0, 0, 0);
			// For a given pixel we have 100 samples within that pixel and send rays through
			// each of the samples. The colors of these rays are then averaged to achieve
			// smoother edges.
			for (int s = 0; s < samplesPerPixel; ++s)
			{
				auto u = (i + randomDouble()) / (imageWidth - 1);
				auto v = (j + randomDouble()) / (imageHeight - 1);
				ray r = cam.getRay(u, v);
				pixelColor += rayColor(r, world);
			}
			writeColor(std::cout, pixelColor, samplesPerPixel);
		}
	}

	std::cerr << "\nDone.\n";
}
