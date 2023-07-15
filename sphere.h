#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "mathUtils.h"
#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), p_Material(m) {};

    virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> p_Material;
};

#endif
