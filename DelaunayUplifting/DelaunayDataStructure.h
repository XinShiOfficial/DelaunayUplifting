#pragma once

#include"Point.h"
namespace MeshLib {

	class CVertex {
	public:
		CPoint & point() { return m_point; };
		CVertex(){}
		~CVertex (){}
		CVertex(double x, double y, double z) {
			m_point[0] = x; m_point[1] = y;  m_point[2] = z;
			m_id = -1;
		}
		int& id() { return m_id; }
		bool & visited() { return m_visited; }
	protected:
		CPoint m_point;
		int m_id ;
		bool m_visited = false;
	};

	class CFace {
	public:
		CFace(){}
		CVertex ** vers() { return m_vers; }
		~CFace() {}
		CFace(CVertex * v1, CVertex*v2, CVertex* v3) {
			m_vers[0] = v1; m_vers[1] = v2; m_vers[2] = v3;
		}
		int & id() { return m_id; };
	protected:
		int m_id;
		CVertex* m_vers[3] ;
	};

	class CEdge {
	public:
		CEdge() {} 
		CEdge(int vv1,int vv2){
			m_v1 = vv1;
			m_v2 = vv2;
		}
		int  v1() { return m_v1; }
		int  v2() { return m_v2; }
	protected:
		int m_v1;
		int m_v2;
	};
}