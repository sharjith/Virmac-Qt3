/***************************************************************************
  qglview.h  -  description
  -------------------
begin                : |15- Feb|
copyright            : (C) |2003| by |Sharjith N.|
email                : |sharjith_ssn@hormail.com|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGLVIEW_H
#define QGLVIEW_H

//Geometry
#include "Geometry.h"
#include "GeomException.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Line3D.h"
#include "Plane.h"

//Graphics
#include "GLObject.h"
#include "GLAxisSystem.h"
#include "GLTrihedron.h"
#include "GLGrid.h"
#include "GLDisplayContext.h"
#include "GLCamera.h"

#include <qwidget.h>
#include <qrect.h>
#include <qpoint.h>
#include <qgl.h>

/**
 *@author Sharjith
 */

class VirmacDoc;
class VirmacView;
class CGLDisplayContext;



class QGLView
{
    public:
	QGLView(QGLWidget* w, CGLDisplayContext* ctx);
	virtual ~QGLView();
	void InitGL();
	void ReSize( int w, int h );
	void Draw();
	GLfloat GetRange() const { return nRange; }
    private:
	bool ldown;
	bool mdown;
	bool rdown;

	QPoint  lPoint;
	QPoint  mPoint;
	QPoint  rPoint;

	/* Light values and coordinates*/
	GLfloat  ambientLight[4];
	GLfloat  diffuseLight[4];
	GLfloat  specular[4];
	GLfloat  specref[4];
	/*Viewing Volume Range*/

	int     myXmin;
	int     myYmin;
	int     myXmax;
	int     myYmax;

	int	winW;
	int	winH;

	GLdouble win_xmin, 
	    win_ymin, 
	    win_xmax, 
	    win_ymax,
	    winz;

	GLdouble nRange;
	GLdouble xRot;
	GLdouble yRot;
	GLdouble zRot;
	GLdouble xTrans;
	GLdouble yTrans;
	GLdouble zTrans;
	GLfloat m_bkRed;
	GLfloat m_bkGreen;
	GLfloat m_bkBlue;
	GLfloat m_bkBotRed;
	GLfloat m_bkBotGreen;
	GLfloat m_bkBotBlue;
	GLDisplayMode displayMode;
	GLProjectionType projectionType;
	bool m_antialias;

	QPoint DownPoint;
	bool ZoomON;
	bool PanON;
	bool RotON;

	bool gridOn;

	bool multiViewActive;

	QGLWidget* theWidget;
	QString    viewLabel;
	CGLCamera* myCamera;
	CGLDisplayContext* theContext;
	CGLTrihedron* myTrihedron;
	CGLGrid* myGrid;
	GLViewType myViewType;
	// Operations
    public:
	void RenderScene(GLenum mode);

	void ZoomView(QPoint, QPoint);
	void ZoomViewBy(const int amount);
	void PanView(QPoint, QPoint);
	void RotateView(QPoint, QPoint);

	void StopZooming(){ ZoomON = false; }
	void StopPanning(){ PanON = false; }
	void StopRotation(){ RotON = false; }

	void TopView();
	void BottomView();
	void FrontView();
	void BackView();
	void LeftView();
	void RightView();
	void AxonView();
	void MultiView(bool);

	void SnapShot(const QString& fName);

	GLViewType GetViewType() const { return myViewType; }
	bool IsPlanarView() const;
	CGLGrid* GetGrid() { return myGrid; }

	CGLCamera* GetCamera() { return myCamera; }

	void FitAll();
	void ZoomWindow(QRect zcRect);
	void ResetView();
	void Refresh() { theWidget->updateGL();}

	void PointToScreen(const CPoint3D& P, int& x, int& y);
	void ScreenToPoint(const int& x, const int& y, CPoint3D&  P);
	void FitView();
	void Fit3d(QRect zcRect);

	void SetBackgroundColor(GLfloat, GLfloat, GLfloat);
	void SetDisplayMode(const GLDisplayMode& aMode);
	GLDisplayMode GetDisplayMode() const { return displayMode; }

	void SetProjectionType(const GLProjectionType& aProj);
	GLProjectionType GetProjectionType() const { return projectionType; }

	void SetAntiAliasing(const bool& mode);

	void SetGridOn(const bool& mode);
	void SetGridPara(const CPlane& plane, const double& size, const double& step);
	void SetGridPlane(const CPlane& plane) { myGrid->SetPlane(plane); }
	void SetGridStep(const float& step) { myGrid->SetStep(step); }
	void SetGridColor(const GLubyte& r, const GLubyte& g, const GLubyte& b);
	bool IsGridOn() const { return gridOn; }


	void Select(const int& x, const int& y);
	void MultiSelect(const int& x, const int& y);
	void SweepSelect(const QRect& swRect);

	int  ProcessSelection(const int& x, const int& y, const int& sensitivity = 5);
	int  ProcessSweepSelection(const QRect& rect, unsigned long int* id);

};

#endif
