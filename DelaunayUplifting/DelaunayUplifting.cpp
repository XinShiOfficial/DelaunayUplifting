// DelaunayUplifting.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"DelaunayOperation.h"
using namespace MeshLib;

int main()
{
	srand((unsigned)time(NULL));//加在这可以吗
	DelaunayOperation t;
	t._initialize_vers();
	t._create_convex_hull();
	t._write_m("./convex_hull.m");

	t._project_down();
	t._write_m("./dalaunay.m");
    return 0;
}

