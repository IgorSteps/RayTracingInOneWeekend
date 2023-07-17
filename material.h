#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "mathUtils.h"
#include "hittable.h"

/*
	Material needs to do two things:
		- Produce a scattered ray (or say it absorbed the incident ray).
		- If scattered, say how much the ray should be attenuated.
*/
class material {
public:
	// It can either scatter always and attenuate by its reflectance R,
	// or it can scatter with no attenuation but absorb the fraction 1-R  of the rays,
	// or it could be a mixture of those strategies.
	virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const = 0;
};


class lambertian : public material {
public:
	lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override 
	{
		auto scatterDir = rec.normal + randomUnitVector();

		// catch degenerate scatter direction.
		if (scatterDir.nearZero())
		{
			scatterDir = rec.normal;
		}

		scattered = ray(rec.p, scatterDir);
		attenuation = albedo;

		return true;
	}
	
public:
	color albedo;
};


class metal : public material {
public:
	metal(const color& a, double fuzzines) : albedo(a), fuzzines(fuzzines < 1 ? fuzzines : 1) {}

	virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override
	{
		vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzzines * randomInUnitSphere());
		attenuation = albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzzines;
};

class dielectric : public material {
public:
	dielectric(double refractionInx) : refractionInd(refractionInx) {}

	virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double refractionRation = rec.frontFace ? (1.0 / refractionInd) : refractionInd;

		vec3 unit_direction = unitVector(rIn.direction());
		vec3 refracted = refract(unit_direction, rec.normal, refractionRation);

		scattered = ray(rec.p, refracted);
		return true;
	}

public:
	double refractionInd; 
};

#endif