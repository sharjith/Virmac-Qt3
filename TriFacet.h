#ifndef __TRIFACET_H__
#define __TRIFACET_H__

#include "Point3D.h"
#include "Vector3D.h"

class CTriFacet
{
    public:
	CTriFacet(){}
	CTriFacet(CPoint3D p1, CPoint3D p2,	CPoint3D p3, CVector3D n)
	{
	    P1 = p1;
	    P2 = p2;
	    P3 = p3;
	    N = n;
	}

	CPoint3D P1;
	CPoint3D P2;
	CPoint3D P3;
	CVector3D N;
};

#endif
