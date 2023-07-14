#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "hittable.h"

#include <vector>

class hittableList: public hittable
{
public:
	hittableList() {}
	hittableList(std::shared_ptr<hittable> object) 
	{
		add(object);
	}

	void clear()
	{
		objects.clear();
	}

	void add(std::shared_ptr<hittable> object)
	{
		objects.push_back(object);
	}

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;

public:
	std::vector<std::shared_ptr<hittable>> objects;
};

#endif