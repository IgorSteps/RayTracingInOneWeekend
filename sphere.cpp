#include "sphere.h"


bool sphere::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const 
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius * radius;

    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    
    auto sqrtd = sqrt(discriminant);

    // find the nearest root that lies in the acceptable range.
    auto root = (-halfB - sqrtd) / a;
    if (root < tMin || tMax < root) 
    {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || tMax < root)
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    // determine surface side.
    vec3 outwardNormal = (rec.p - center) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.p_Material = p_Material;

    return true;
} 