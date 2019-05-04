#pragma once
#include"DelaunayDataStructure.h"
#include<vector>
#include<list>
#include<ctime>
#include<cstdlib>
#include<iostream>
#include"Eigen/Dense"
#include <fstream>

using namespace Eigen;
using namespace std;
namespace MeshLib {
	class DelaunayOperation {
	public:
		DelaunayOperation(int num = 100) {
			v_num = num;
			
		}

		~DelaunayOperation(){}

		void _initialize_vers();

		void _create_convex_hull();

		

		void _write_m(const char* outdir);
		
		void _project_down();
	protected:
		CVertex * idVertex(int id) {
			assert(vertices.size() + 1 > id);
			return vertices[id - 1];
		}
		/*计算四面体体积*/
		double _get_v(CFace * cface, CVertex * cver);
		/*给每个最终的面赋id*/
		void _face_id();

		void _initialize_hull();
	protected:
		int v_num;
		double range = -1;//表示点的x，y坐标从-1到1
		std::vector<CVertex *> vertices;
		std::list<CFace * > faces;
	};

	void MeshLib::DelaunayOperation::_initialize_vers()
	{
		CVertex * tmp;
		for (int i = 0; i < v_num; i++) {
			double x = double(rand() % 2000) / 1000 - 1;
			double y = double(rand() % 2000) / 1000 - 1;
			double z = x * x + y * y;
			tmp = new CVertex(x, y, z);
			//std::cout << tmp->point()[0] << "," << tmp->point()[1] << "," << tmp->point()[2] << "\n";
			tmp->id() = i + 1;
			vertices.push_back(tmp);
		}
		//system("PAUSE");
	}
	inline void DelaunayOperation::_initialize_hull()
	{
		//找到四个比较远的点，以逆时针顺序，构造四个面
		//先找最矮的一个点
		CVertex * verZ = vertices[0];
		double ztmp = verZ->point()[2];
		for (std::vector<CVertex*> ::iterator viter = vertices.begin();
			viter != vertices.end(); viter++) {
			CVertex * tmp = *viter;
			if (tmp->point()[2] < ztmp ) {
				verZ = tmp;
				ztmp = tmp->point()[2];
			}
		}

		//找到y最大的一个点
		CVertex * verY = vertices.front();
		double ymax = verY->point()[1];
		for (std::vector<CVertex*> ::iterator viter = vertices.begin();
			viter != vertices.end(); viter++) {
			CVertex * tmp = *viter;
			if (tmp->point()[1] > ymax && tmp!=verZ) {
				verY = tmp;
				ymax = tmp->point()[1];
			}
		}

		//找到x最小的和x最大的
		CVertex * verX1 = vertices.front();
		double xmin = -range;
		for (std::vector<CVertex*> ::iterator viter = vertices.begin();
			viter != vertices.end(); viter++) {
			CVertex * tmp = *viter;
			if (tmp->point()[0] <xmin&&tmp!=verY&&tmp!=verZ) {
				verX1 = tmp;
				xmin = tmp->point()[0];
			}
		}

		CVertex * verX2 = vertices.front();
		double xmax = range;
		for (std::vector<CVertex*> ::iterator viter = vertices.begin();
			viter != vertices.end(); viter++) {
			CVertex * tmp = *viter;
			if (tmp->point()[0] > xmax && tmp!= verX1 &&tmp!= verZ &&tmp!=verY) {
				verX2 = tmp;
				xmax = tmp->point()[0];
			}
		}


		assert(verX1 != NULL && verX2 != NULL && verY != NULL &&verZ!=NULL);
		verX1->visited() = true;
		verX2->visited() = true;
		verY->visited() = true;
		verZ->visited() = true;
		//构造四个面
		CFace* tmp1 = new CFace(verZ,verX1,verX2);
		CFace* tmp2 = new CFace(verZ, verX2, verY);
		CFace* tmp3 = new CFace(verZ, verY, verX1);
		CFace* tmp4 = new CFace(verY, verX2, verX1);

		faces.push_back(tmp1);
		faces.push_back(tmp2);
		faces.push_back(tmp3);
		faces.push_back(tmp4);

		double a = _get_v(tmp1, verY);
		std::cout << _get_v(tmp1, verY) << std::endl;
		std::cout << _get_v(tmp2, verX1) << std::endl;
		std::cout << _get_v(tmp3, verX2) << std::endl;

		if (a > 0) {
			std::cout << _get_v(tmp1, verY) << std::endl;
		}
		
	}
	inline void DelaunayOperation::_create_convex_hull()
	{
		_initialize_hull();
		assert(faces.size() > 0);
		for (std::vector<CVertex*>::iterator viter = vertices.begin();
			viter != vertices.end(); viter++) {
			CVertex* vtmp = *viter;
			if (vtmp->visited()) {
				continue;
			}
			vtmp->visited() = true;
			//没被访问的点则联系所有的面进行判断
			//去掉一些面
			//faces.unique();
			std::list<CFace * > deletefaces;
			for (std::list<CFace*> ::iterator fiter = faces.begin();
				fiter != faces.end();) {
				CFace * ftmp = *fiter;
				if (_get_v(ftmp, vtmp) < 0) {//负数表示没在凸包里的点
					deletefaces.push_back(ftmp);
					faces.erase(fiter++);
				}
				else {
					fiter++;
				}
			}
			//增加新的面，如果有的话
			if (deletefaces.size() > 0) {
				//构造边界
				std::list<CEdge> boundarytmp;
				std::cout << deletefaces.size() << "\n";
				for (std::list<CFace*> ::iterator fiter = deletefaces.begin();
					fiter != deletefaces.end(); fiter++) {
					CFace * ftmp = *fiter;
					//每个面的三个边加进去
					boundarytmp.push_back(CEdge(ftmp->vers()[0]->id(), ftmp->vers()[1]->id()));
					boundarytmp.push_back(CEdge(ftmp->vers()[1]->id(), ftmp->vers()[2]->id()));
					boundarytmp.push_back(CEdge(ftmp->vers()[2]->id(), ftmp->vers()[0]->id()));		
				}

				//边界去重
				std::list<CEdge> boundary;
				int i = 0;
				for (std::list<CEdge>::iterator eiter = boundarytmp.begin();
					eiter != boundarytmp.end(); eiter++,i ++) {
					CEdge tmp = *eiter;
					bool flag = true;
					int j = 0;
					for (std::list<CEdge>::iterator eiter2 = boundarytmp.begin();
						eiter2 != boundarytmp.end(); eiter2++,j++) {
						if (i == j) continue;
						CEdge tmp2 = *eiter2;
						if ((tmp2.v1() == tmp.v2() && tmp2.v2() == tmp.v1())) {
							flag = false;
							break;
						}
					}
					if (flag) {
						boundary.push_back(tmp);
					}

				}
				//验证边界
				for (std::list<CEdge>::iterator eiter = boundary.begin();
					eiter != boundary.end(); eiter++){
						CEdge tmp = * eiter;
						std::cout << tmp.v1() << "," << tmp.v2() << "\n";
					}
				//根据边界加面
				for (std::list<CEdge>::iterator eiter = boundary.begin();
					eiter != boundary.end(); eiter++) {
					CEdge tmp = *eiter;
					CFace * ftmp = new CFace(idVertex(tmp.v1()), idVertex(tmp.v2()), vtmp);
					faces.push_back(ftmp);
				}

				deletefaces.clear();
				boundarytmp.clear();
				//std::cout << deletefaces.size() << "\n";
			}
		}
		_face_id();
	}
	inline double DelaunayOperation::_get_v(CFace * cface, CVertex * cver)
	{
		assert(cface != NULL && cver != NULL);
		Matrix3d c;
		CPoint t1 = cface->vers()[1]->point() - cface->vers()[0]->point();
		CPoint t2 = cface->vers()[2]->point() - cface->vers()[0]->point();
		CPoint t3 = cver->point() - cface->vers()[0]->point();

		c << t1[0], t1[1], t1[2],
			t2[0], t2[1], t2[2],
			t3[0], t3[1], t3[2];
		return (1.0 / 6)*c.determinant();
	}
	inline void DelaunayOperation::_face_id()
	{
		int i = 1;
		for (std::list<CFace * >::iterator fiter = faces.begin();
			fiter != faces.end(); fiter++) {
			(*fiter)->id() = i++;
		}
	}
	inline void DelaunayOperation::_write_m(const char* outdir)
	{
		std::fstream _os(outdir, std::fstream::out);
		if (_os.fail())
		{
			fprintf(stderr, "Error is opening file %s\n", outdir);
			return;
		}
		for (std::vector<CVertex*>::iterator viter = vertices.begin(); viter != vertices.end(); viter++)
		{
			CVertex* v = *viter;

			_os << "Vertex " << v->id();

			for (int i = 0; i < 3; i++)
			{
				_os << " " << v->point()[i];
			}
			/*if (v->string().size() > 0)
			{
				_os << " " << "{" << v->string() << "}";
			}*/
			_os << std::endl;
		}

		for (std::list<CFace*>::iterator fiter = faces.begin(); fiter != faces.end(); fiter++)
		{
			CFace* f = *fiter;

			_os << "Face " << f->id();
			for (int i = 0; i < 3; i++) {
				_os << " " << f->vers()[i]->id();
			}
			
			_os << std::endl;
		}
	}
	inline void DelaunayOperation::_project_down()
	{
		for (std::list<CFace * >::iterator fiter = faces.begin();
			fiter != faces.end();) {
			CFace * ftmp = *fiter;
			double z = ((ftmp->vers()[1]->point() - ftmp->vers()[0]->point())
				^ (ftmp->vers()[2]->point() - ftmp->vers()[1]->point()))[2];
			if (z < 0) {
				faces.erase(fiter++);
			}
			else {
				fiter++;
			}
		}
		for (std::vector<CVertex*>::iterator viter = vertices.begin();
			viter != vertices.end(); viter++) {
			(*viter)->point()[2] = 0.0;
		}
		_face_id();
	}
}