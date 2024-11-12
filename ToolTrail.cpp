// ToolTrail.cpp: implementation of the CToolTrail class.
//
//////////////////////////////////////////////////////////////////////

#include "ToolTrail.h"
#include "Point3D.h"
#include "Arc.h"
#include "AxisSystem.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolTrail::CToolTrail() : CToolPath()
{
    itsRed = 0;
    itsGreen = 255;
    itsBlue = 0;

    g00Red = 255;
    g00Green = 0;
    g00Blue = 0;

    g01Red = 0;
    g01Green = 255;
    g01Blue = 0;

    g02Red = 255;
    g02Green = 255;
    g02Blue = 0;

    g03Red = 255;
    g03Green = 155;
    g03Blue = 125;

    glObjType = GLSIRUS;
    glObjID = (int)glObjType + CGLObject::GetCount();
    blocCount = 0;
}

CToolTrail::~CToolTrail()
{

}

CGLObject* CToolTrail::Copy() const
{
    CToolTrail* aPath = new CToolTrail();
    aPath->SetColor(itsRed, itsGreen, itsBlue);
    aPath->SetMaterial(GetMaterial());
    CListIteratorOfListOfNCBlock it(myGCodeList);
    for(it.Init(); it.More(); it.Next())
	aPath->myGCodeList->Append(it.Current());
    return aPath;
}

void CToolTrail::DefineDisplay()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glColor3ub(itsRed, itsGreen, itsBlue);
    DrawPath();    
    glDisable(GL_BLEND);

}

void CToolTrail::Display()
{
    DefineDisplay();
}

void CToolTrail::DrawPath()
{
    if(!myGCodeList->IsEmpty())
    {
	glPushMatrix();
	glTranslated(m_dTransX,m_dTransY,m_dTransZ);
	glTranslated(m_dRotCX,m_dRotCY,m_dRotCZ);
	glRotated(m_dRotRX,1,0,0);
	glRotated(m_dRotRY,0,1,0);
	glRotated(m_dRotRZ,0,0,1);
	glScaled(m_dScaleX, m_dScaleY, m_dScaleZ);
	glPushMatrix();
	glTranslatef(-m_dRotCX,-m_dRotCY,-m_dRotCZ);

	CNCBlock G1, G2;
	CListIteratorOfListOfNCBlock it(myGCodeList);
	it.Init();	
	for(int i = 0; i < blocCount-1; i++)
	{

	    G1 = it.Current();
	    if(it.More())
	    {
		it.Next();
		G2 = it.Current();
	    }

	    if(G1.G == 17)
		myArcPlane = G02_03_XOY;
	    if(G1.G == 18)
		myArcPlane = G02_03_ZOX;
	    if(G1.G == 19)
		myArcPlane = G02_03_YOZ;

	    if(G2.G == 0)
		glColor3ub(g00Red,g00Green,g00Blue);
	    else if(G2.G == 1)
		glColor3ub(g01Red,g01Green,g01Blue);
	    else if(G2.G == 2)
		glColor3ub(g02Red,g02Green,g02Blue);
	    else if(G2.G == 3)
		glColor3ub(g03Red,g03Green,g03Blue);

	    if(G2.G==0)
	    {
		G2.myArcMode = ARC_STR;
	    }
	    if(G2.G==1)
	    {
		G2.myArcMode = ARC_STR;
	    }
	    if(G2.G == 2)
	    {
		G2.myArcMode = ARC_CW;
	    }
	    if(G2.G == 3)
	    {
		G2.myArcMode = ARC_CCW;
	    }

	    CPoint3D P1(G1.X, G1.Y, G1.Z);
	    CPoint3D P2(G2.X, G2.Y, G2.Z);
	    glBegin(GL_LINE_STRIP);
	    if(G2.myArcMode == ARC_CW && m_bShowArc)
	    {
		CPoint3D cp(G2.I,G2.J,G2.Z);
		CPoint3D sp(G1.X,G1.Y,G1.Z);
		CPoint3D ep(G2.X,G2.Y,G2.Z);

		//double rad = cp.Distance(sp);
		glVertex3f(G1.X,G1.Y,G1.Z);

		CAxisSystem cs;
		if(myArcPlane == G02_03_XOY)
		{
		    cs.SetDirection(0,0,1);
		}
		if(myArcPlane == G02_03_YOZ)
		{
		    cs.SetDirection(1,0,0);
		}
		if(myArcPlane == G02_03_ZOX)
		{
		    cs.SetDirection(0,1,0);
		}
		CArc anArc((int)myArcPlane, sp, ep, cp, PI/360, true);
		CListIteratorOfListOfCPoint3D it(anArc.ArcPoints);
		for(it.Init();it.More();it.Next())
		{
		    CPoint3D P = it.Current();
		    glVertex3f(P.GetX(), P.GetY(), P.GetZ());
		}


		glVertex3f(G2.X, G2.Y, G2.Z);
	    }
	    else if(G2.myArcMode == ARC_CCW && m_bShowArc)
	    {
		CPoint3D cp(G2.I,G2.J,G2.Z);
		CPoint3D sp(G1.X,G1.Y,G1.Z);
		CPoint3D ep(G2.X,G2.Y,G2.Z);

		//double rad = cp.Distance(sp);
		glVertex3f(G1.X,G1.Y,G1.Z);

		CAxisSystem cs;
		if(myArcPlane == G02_03_XOY)
		{
		    cs.SetDirection(0,0,1);
		}
		if(myArcPlane == G02_03_YOZ)
		{
		    cs.SetDirection(1,0,0);
		}
		if(myArcPlane == G02_03_ZOX)
		{
		    cs.SetDirection(0,1,0);
		}
		CArc anArc((int)myArcPlane, sp, ep, cp, PI/360, false);
		CListIteratorOfListOfCPoint3D it(anArc.ArcPoints);
		for(it.Init();it.More();it.Next())
		{
		    CPoint3D P = it.Current();
		    glVertex3f(P.GetX(), P.GetY(), P.GetZ());
		}


		glVertex3f(G2.X, G2.Y, G2.Z);
	    }
	    else
	    {
		glVertex3d(P1.GetX(), P1.GetY(), P1.GetZ());
		glVertex3d(P2.GetX(), P2.GetY(), P2.GetZ());
	    }
	    glEnd();
	}
	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
	glPopMatrix();
    }
}
