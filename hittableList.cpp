#include "hittableList.h"

bool hittableList::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const
{
	hitRecord tempRec;
	bool hitAnything = false;
	auto closestSoFar = tMax;

	for (const auto& obj : objects) {
		if (obj->hit(r, tMin, closestSoFar, tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}

	return hitAnything;
}