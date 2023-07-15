#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "mathUtils.h"
#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;

public:
    point3 center;
    double radius;
};

#endif
