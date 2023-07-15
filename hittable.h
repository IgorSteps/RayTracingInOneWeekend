#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "mathUtils.h"

class material;

struct hitRecord {
    point3 p;
    vec3 normal;
    shared_ptr<material> p_Material;
    double t;   
    bool frontFace;

    // determine which side of the surface that the ray is coming from.
    inline void setFaceNormal(const ray& r, const vec3& outwardNormal) 
    {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const = 0;
};

#endif