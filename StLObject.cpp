// StlObject.cpp: implementation of the CStLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "Virmac.h"
#include "StLObject.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStLObject::CStLObject(CStLReader* r)
{
    myReader = r->Copy();
    glObjType = GLSURFACE;
    glObjID = (int)glObjType + CGLObject::GetCount();

    itsMaterial.SetName(ALUMINIUM);

    numTriangles = 0;
    itsShadeRed = 125;
    itsShadeGreen = 125;
    itsShadeBlue = 125;

    itsRed = 200;
    itsGreen = 225;
    itsBlue = 150;

    m_bShowNormals = false;

    displayMode = GLSHADED;

    pointList = new CListOfCPoint3D;
    myFacetList = new CListOfCTriFacet;
    myNormalList = new CListOfCOneAxis;
    ComputePoints();
    ComputeBoundLimits();
    ComputeNormals();
    InitDisplayLists();
}

CStLObject::~CStLObject()
{
    pointList->Clear();
    delete pointList;
    myFacetList->Clear();
    delete myFacetList;
    myNormalList->Clear();
    delete myNormalList;
    delete myReader;
    glDeleteLists(facetList, 1);
    glDeleteLists(normalList, 1);
    glDeleteLists(bBoxList, 1);
}

CGLObject* CStLObject::Copy() const
{
    CStLObject* O = new CStLObject(myReader);
    O->SetColor(itsRed, itsGreen, itsBlue);
    O->SetMaterial(GetMaterial());
    O->glObjID = glObjID;
    return O;
}

void CStLObject::SetReader(CStLReader* r)
{
    if(myReader)
    {
	delete myReader;
	myReader = NULL;
    }
    myReader = r->Copy();
    glObjType = GLSURFACE;
    glObjID = (int)glObjType + CGLObject::GetCount();

    itsMaterial.SetName(ALUMINIUM);

    numTriangles = 0;
    itsShadeRed = 125;
    itsShadeGreen = 125;
    itsShadeBlue = 125;

    itsRed = 200;
    itsGreen = 225;
    itsBlue = 150;

    ComputePoints();
    ComputeBoundLimits();
    ComputeNormals();
    InitDisplayLists();
}

void CStLObject::Nullify()
{
    pointList->Clear();
    numTriangles = 0;
    myFacetList->Clear();
    myNormalList->Clear();
    glDeleteLists(facetList, 1);
    glDeleteLists(normalList, 1);
    glDeleteLists(bBoxList, 1);
}

void CStLObject::DefineDisplay()
{
    GLfloat bgcol[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, bgcol);
    GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specref);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    if(displayMode == GLSHADED)
    {	
	glPushAttrib(GL_LIGHTING_BIT);
	ApplyMaterial();
	DrawShaded();
	glPopAttrib();
    }	

    else if(displayMode == GLWIREFRAME)
    {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);
	DrawWired();
	glPopAttrib();
    }
    else if(displayMode == GLHLREMOVED)
    {	
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glColor3f(bgcol[0], bgcol[1], bgcol[2]);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(5, 5);
	DrawShaded();
	DrawWired();
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPopAttrib();
    }

    glDisable(GL_BLEND);
}

void CStLObject::Display(const GLDisplayMode& dMode)
{
    displayMode = dMode;
    DefineDisplay();
}

void CStLObject::Hilight(const GLDisplayMode& /*dMode*/)
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);
    itsRed = 0; itsGreen = 255; itsBlue = 255;
    glColor3ub(itsRed, itsGreen, itsBlue);
    DrawWired();
    glPopAttrib();
}

void CStLObject::SetColor(const GLubyte& red, const GLubyte& green, const GLubyte& blue)
{
    if(displayMode == GLWIREFRAME)
    {
	itsRed = red;
	itsGreen = green;
	itsBlue = blue;
    }
    else
    {
	itsShadeRed = red;
	itsShadeGreen = green;
	itsShadeBlue = blue;
    }
    DefineDisplay();
}

void CStLObject::GetColor(GLubyte* colVect) const
{
    colVect = new GLubyte[3];
    if(displayMode == GLWIREFRAME)
    {
	colVect[0] = itsRed;
	colVect[1] = itsGreen;
	colVect[2] = itsBlue;
    }
    else
    {
	colVect[0] = itsShadeRed;
	colVect[1] = itsShadeGreen;
	colVect[2] = itsShadeBlue;
    }
}

void CStLObject::ComputeBoundLimits()
{
    double lx=0,ly=0,lz=0,sx=0,sy=0,sz=0;
    CPoint3D P;
    double *pxArray = 0, *pyArray = 0, *pzArray = 0;
    unsigned long cnt = 4;
    unsigned long pCnt=0;
    unsigned long i=0,j=0,inc=0;
    double v=0;

    CListIteratorOfListOfCPoint3D it(pointList);

    for(it.Init(); it.More(); it.Next())
    {
	++pCnt;
    }

    pxArray = new double[pCnt+10];
    pyArray = new double[pCnt+10];
    pzArray = new double[pCnt+10];

    cnt = 4;
    for(it.Init(); it.More(); it.Next())
    {
	P = it.Current();
	pxArray[i] = P.GetX();
	pyArray[i] = P.GetY();
	pzArray[i] = P.GetZ();
	++i;
    }

    i=0;
    //Find Largest And Smallest X
    inc =1;
    do
    {
	inc *= 3;
	inc++;
    }while(inc <= pCnt);
    do
    {
	inc /= 3;

	for(i=inc; i<=pCnt; i++)
	{
	    v = pxArray[i];
	    j=i;
	    while(pxArray[j-inc] > v)

	    {
		pxArray[j] = pxArray[j-inc];
		j -= inc;

		if(j <= inc)
		    break;
	    }
	    pxArray[j] = v;
	}

    }while(inc > 1);

    sx = pxArray[0];  lx = pxArray[pCnt];

    //Find Largest And Smallest Y
    inc =1;
    do
    {
	inc *= 3;
	inc++;
    }while(inc <= pCnt);
    do
    {
	inc /= 3;
	for(i=inc; i<=pCnt; i++)
	{

	    v = pyArray[i];
	    j=i;
	    while(pyArray[j-inc] > v)
	    {
		pyArray[j] = pyArray[j-inc];
		j -= inc;
		if(j <= inc)
		    break;
	    }
	    pyArray[j] = v;
	}
    }while(inc > 1);

    sy = pyArray[0];  ly = pyArray[pCnt];

    //Find Largest And Smallest Z
    inc =1;
    do
    {
	inc *= 3;
	inc++;
    }while(inc <= pCnt);
    do
    {
	inc /= 3;
	for(i=inc; i<=pCnt; i++)
	{
	    v = pzArray[i];
	    j=i;
	    while(pzArray[j-inc] > v)
	    {
		pzArray[j] = pzArray[j-inc];
		j -= inc;
		if(j <= inc)
		    break;
	    }
	    pzArray[j] = v;
	}
    }while(inc > 1);

    sz = pzArray[0];  lz = pzArray[pCnt];

    itsBox.SetLimits(1,1,1,1,1,1);
    itsBox.SetLimits(sx,lx,sy,ly,sz,lz);
    delete[] pxArray;
    delete[] pyArray;
    delete[] pzArray;

}

void CStLObject::DrawWired()
{
    CListIteratorOfListOfCPoint3D myPList(pointList);

    glColor3ub(itsRed, itsGreen, itsBlue);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glCallList(facetList);

    if(m_bShowNormals)
	DrawNormals();
}

void CStLObject::DrawShaded()
{
    CListIteratorOfListOfCPoint3D myPList(pointList);

    if(displayMode == GLSHADED) 
	glColor3ub(itsShadeRed, itsShadeGreen, itsShadeBlue);
    //else HLR

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glCallList(facetList);

    if(m_bShowNormals)
	DrawNormals();
}

void CStLObject::ComputePoints()
{
    CPoint3D curP;
    CListIteratorOfListOfCPoint3D myPList(myReader->pointList);

    if(!myFacetList->IsEmpty())
	myFacetList->Clear();

    if(!pointList->IsEmpty())
	pointList->Clear();
    CPoint3D P1, P2, P3; CVector3D V;
    for(myPList.Init(); myPList.More();)
    {
	pointList->Append(myPList.Current());
	V.SetParam(myPList.Current().GetX(), myPList.Current().GetY(), myPList.Current().GetZ());
	myPList.Next();

	pointList->Append(myPList.Current());
	P1.SetParam(myPList.Current().GetX(), myPList.Current().GetY(), myPList.Current().GetZ());
	myPList.Next();

	pointList->Append(myPList.Current());
	P2.SetParam(myPList.Current().GetX(), myPList.Current().GetY(), myPList.Current().GetZ());
	myPList.Next();

	pointList->Append(myPList.Current());
	P3.SetParam(myPList.Current().GetX(), myPList.Current().GetY(), myPList.Current().GetZ());
	myPList.Next();

	CTriFacet F;
	F.P1 = P1; F.P2 = P2; F.P3 = P3; F.N = V;
	myFacetList->Append(F);
	++numTriangles;
    }
}

void CStLObject::DrawBoundBox()
{
    glCallList(bBoxList);
}

void CStLObject::DrawNormals()
{
    glCallList(normalList);
}

void CStLObject::ComputeNormals(const int& /*size*/)
{
    if(!myNormalList->IsEmpty())
	myNormalList->Clear();

    CListIteratorOfListOfCTriFacet it(myFacetList);
    for(it.Init(); it.More(); it.Next())
    {
	CTriFacet F = it.Current();//myFacetList.GetNext(pos);
	CPoint3D P, P1, P2, P3;
	P1 = F.P1; P2 = F.P2; P3 = F.P3;

	P.SetX((P1.GetX()+P2.GetX()+P3.GetX())/3);
	P.SetY((P1.GetY()+P2.GetY()+P3.GetY())/3);
	P.SetZ((P1.GetZ()+P2.GetZ()+P3.GetZ())/3);

	COneAxis Ax(P, F.N);
	myNormalList->Append(Ax);
    }
}

void CStLObject::InitDisplayLists()
{
    //facet list
    facetList = glGenLists(1); 
    glNewList(facetList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    CListIteratorOfListOfCTriFacet it(myFacetList);
    for(it.Init(); it.More(); it.Next())
    {
	CTriFacet F = it.Current();
	glNormal3d(F.N.GetX(), F.N.GetY(), F.N.GetZ());
	glVertex3d(F.P1.GetX(), F.P1.GetY(), F.P1.GetZ());
	glVertex3d(F.P2.GetX(), F.P2.GetY(), F.P2.GetZ());
	glVertex3d(F.P3.GetX(), F.P3.GetY(), F.P3.GetZ());

    }
    glEnd();
    glEndList();
    
    //normal list
    normalList = glGenLists(1);
    glNewList(normalList, GL_COMPILE);
    CListIteratorOfListOfCOneAxis itr(myNormalList);
    for(itr.Init(); itr.More(); itr.Next())
    {
	COneAxis A = itr.Current();
	CVector3D P = A.GetPosition();
	CPoint3D D = P;
	D.Translate(A, 10);

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glColor3ub(255,0,0);
	glBegin(GL_LINES);
	glVertex3d(P.GetX(), P.GetY(), P.GetZ());
	glVertex3d(D.GetX(), D.GetY(), D.GetZ());
	glEnd();
    }
    glEndList();

    //bBox list
    bBoxList = glGenLists(1);
    glNewList(bBoxList, GL_COMPILE);
    double xmin, xmax, ymin, ymax, zmin, zmax;
    GetBoundingBox().GetLimits(xmin, xmax, ymin, ymax, zmin, zmax);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    //bottom
    glVertex3d(xmin, ymin, zmin);
    glVertex3d(xmax, ymin, zmin);
    glVertex3d(xmax, ymax, zmin);
    glVertex3d(xmin, ymax, zmin);
    //top
    glVertex3d(xmin, ymin, zmax);
    glVertex3d(xmax, ymin, zmax);
    glVertex3d(xmax, ymax, zmax);
    glVertex3d(xmin, ymax, zmax);
    //front
    glVertex3d(xmin, ymin, zmin);
    glVertex3d(xmax, ymin, zmin);
    glVertex3d(xmax, ymin, zmax);
    glVertex3d(xmin, ymin, zmax);
    //front
    glVertex3d(xmin, ymax, zmin);
    glVertex3d(xmax, ymax, zmin);
    glVertex3d(xmax, ymax, zmax);
    glVertex3d(xmin, ymax, zmax);
    glEnd();
    glEndList();
}

