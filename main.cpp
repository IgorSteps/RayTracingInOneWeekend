#include "mathUtils.h"

#include "camera.h"
#include "colour.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

#include <iostream>

// Linearly blends white and blue depending on the height of the y coordinate
// after scaling the ray direction to unit length(so −1.0 < y < 1.0).
color rayColor(const ray& r, const hittable& world, int depth)
{
	// if we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
	{
		return color(0, 0, 0);
	}

	hitRecord rec;
	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if (rec.p_Material->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * rayColor(scattered, world, depth - 1);
		}

		return color(0, 0, 0);
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
	const int maxDepth = 50;

	// world.
	hittableList world;
	auto materialGround = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto materialCenter = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto materialLeft = make_shared<dielectric>(1.5);
	auto materialRight = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, materialGround));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, materialCenter));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, materialLeft));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, materialRight));

	// camera.
	camera cam;

	// render.
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

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
				pixelColor += rayColor(r, world, maxDepth);
			}
			writeColor(std::cout, pixelColor, samplesPerPixel);
		}
	}

	std::cerr << "\nDone.\n";
}
