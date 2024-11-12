//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	StereoLithography Data File Object

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// StlObject.h: interface for the CStLObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __STLOBJECT_H__
#define __STLOBJECT_H__

#include "GLObject.h"
#include "StLReader.h"
#include "ListOfCPoint3D.h"
#include "ListIteratorOfListOfCPoint3D.h"
#include "ListOfCTriFacet.h"
#include "ListOfCOneAxis.h"
#include "ListIteratorOfListOfCTriFacet.h"
#include "ListIteratorOfListOfCOneAxis.h"
#include "GLOneAxis.h"
#include "TriFacet.h"

class VirmacDoc;
class CStLObject : public CGLObject  
{
    friend class VirmacDoc;
    public:
	CStLObject(){}
	CStLObject(CStLReader*);
	virtual ~CStLObject();
	virtual CGLObject* Copy() const;
	virtual void DefineDisplay();
	virtual void Display(const GLDisplayMode& = GLWIREFRAME);
	virtual void Hilight(const GLDisplayMode&);
	virtual void SetColor(const GLubyte&, const GLubyte&, const GLubyte&);
	virtual void GetColor(GLubyte*) const;
	void ShowNormals(const bool& flag) { m_bShowNormals = flag; }
	void SetReader(CStLReader*);
	void Nullify();
	bool IsNormalShown() const { return m_bShowNormals; }
	unsigned long NumTriangles() const { return numTriangles; }
    private:
	void DrawWired();
	void DrawShaded();
	void DrawBoundBox();
	void DrawNormals();
	void ComputePoints();
	void ComputeBoundLimits();
	void ComputeNormals(const int& size = 5);
	void InitDisplayLists();
    private:
	CStLReader *myReader;
	CListOfCPoint3D* pointList;
	CListOfCTriFacet* myFacetList;
	CListOfCOneAxis* myNormalList;
	bool m_bShowNormals;
	int  facetList;
	int  normalList;
	int  bBoxList;
	unsigned long numTriangles;
};

#endif
