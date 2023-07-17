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
		double refractionRatio = rec.frontFace ? (1.0 / refractionInd) : refractionInd;

		vec3 unitDir = unitVector(rIn.direction());
		double cosTheta = fmin(dot(-unitDir, rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		vec3 direction;

		if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
		{
			direction = reflect(unitDir, rec.normal);
		}
		else
		{
			direction = refract(unitDir, rec.normal, refractionRatio);
		}

		scattered = ray(rec.p, direction);;
		return true;
	}

public:
	double refractionInd;

private:
	// use Schlick's approximation for reflectance.
	static double reflectance(double cosine, double refIdx) {
		auto r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

};

#endif