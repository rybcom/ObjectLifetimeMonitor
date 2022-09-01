
#include <iostream>
#include <vector>
#include "InstancingMonitor.h"

using namespace instancing_monitor;

Sphere const getConstSphere()
{
	TRACE(return Sphere(););
}
Sphere getSphere()
{
	TRACE(return Sphere(););
}

void copy_assign_const()
{
	TRACE_FUNC();
	TRACE(Sphere s{});
	TRACE(s = getConstSphere());
	TRACE();
}

void copy_assign()
{
	TRACE_FUNC();
	TRACE(Sphere s{});
	TRACE(s = getSphere());
	TRACE();
}

void simple_rvo()
{
	TRACE_FUNC();
	TRACE(const Sphere s = getSphere());
	TRACE();
}

void vector_emplace_ok()
{
	TRACE_FUNC();
	std::vector<Prism> prisms;
	TRACE(prisms.emplace_back("Red"));
	TRACE();
}

void vector_emplace_wrong()
{
	TRACE_FUNC();
	std::vector<Prism> prisms;
	TRACE(prisms.emplace_back(Prism("Red")));
	TRACE();
}

struct VectorDataConst
{
	const Cube c;
};

struct VectorData
{
	Cube c;
};

void vector_resize_with_const_element()
{
	TRACE_FUNC();
	std::vector<VectorDataConst> shapes;
	TRACE(shapes.emplace_back());
	TRACE(shapes.emplace_back());
	TRACE();
}

void vector_resize()
{
	TRACE_FUNC();
	std::vector<VectorData> shapes;
	TRACE(shapes.emplace_back());
	TRACE(shapes.emplace_back());
	TRACE();
}

Cube rvo_wrong(bool switch_on)
{
	TRACE_FUNC();

	if (switch_on)
	{
		TRACE(Cube c1{});
		return c1;
	}
	else
	{
		TRACE(Cube c2{});
		return c2;
	}
}

Cube rvo_ok(bool switch_on)
{
	TRACE_FUNC();
	
	if (switch_on)
	{
		TRACE(return Cube{});
	}
	else
	{
		TRACE(return Cube{});
	}
}

void base()
{
	TRACE_FUNC();
	TRACE(const Sphere a{});
	TRACE(Sphere b = a);
	TRACE(Sphere c = std::move(b));
	TRACE(Sphere const& d_ref = c);
	TRACE(Sphere e{});
	TRACE(Sphere f{});
	TRACE(e = std::move(c));
	TRACE(f = e);
}

int main()
{
	TRACE_FUNC();

	base();

	copy_assign();
	copy_assign_const();

	simple_rvo();

	vector_emplace_wrong();
	vector_emplace_ok();

	vector_resize();
	vector_resize_with_const_element();

	Cube c1 = rvo_wrong(true);
	Cube c2 = rvo_ok(true);
	
	TRACE();
}