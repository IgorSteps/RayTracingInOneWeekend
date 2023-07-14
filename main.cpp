#include <iostream>
#include "colour.h"
#include "vec3.h"
#include "ray.h"

// Checks if the ray hits the sphere. 
bool hitSphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;

	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;

	auto discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

// Linearly blends white and blue depending on the height of the y coordinate
// after scaling the ray direction to unit length(so −1.0 < y < 1.0).
color rayColor(const ray& r)
{
	if (hitSphere(point3(0, 0, -1), 0.5, r))
	{
		return color(1, 0, 0);
	}

	vec3 unitDir = unitVector(r.direction());
	auto t = .5 * (unitDir.y() + 1.0);

	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.5, 1.0);
}

int main()
{
	// image.
	const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

	// camera.
	auto viewportHeight = 2.0;
	auto viewportWidth = aspectRatio * viewportHeight;
	auto focalLength = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewportWidth, 0, 0);
	auto vertical = vec3(0, viewportHeight, 0);
	auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

	// render.
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int j = imageHeight - 1; j >= 0; --j) 
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < imageWidth; ++i) 
		{
			auto u = double(i) / (imageWidth - 1);
			auto v = double(j) / (imageHeight - 1);
			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
			color pixelColour = rayColor(r);
			writeColor(std::cout, pixelColour);
		}
	}

	std::cerr << "\nDone.\n";
}
