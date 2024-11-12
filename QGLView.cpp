/***************************************************************************
  QGLView.cpp  -  description
  -------------------
begin                : |15 - Feb|
copyright            : (C) |2003| by |Sharjith N.|
email                : |sharjith_ssn@hotmail.com|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "QGLView.h"
#include "MMath.h"

#include "VirmacView.h"
#include "VirmacDoc.h"

QGLView::QGLView(QGLWidget* w, CGLDisplayContext* ctx) : theWidget(w), theContext(ctx)
{
    ldown = mdown = rdown = false;

    myCamera = new CGLCamera;
    winW = winH = 0;
    viewLabel.sprintf("Axonometric");
    /* Rotation amounts*/
    xRot = -45.0f;
    zRot = -45.0f;
    yRot = 0;
    xTrans = yTrans = zTrans = 0.0f;

    myViewType = GLAXONVIEW;

    nRange = 500;

    m_bkRed = 0.2;
    m_bkGreen = 0.2;
    m_bkBlue = 0.39607843;

    m_bkBotRed = 0.670588235294117647058823529412;
    m_bkBotGreen = 0.674509803921568627450980392157;
    m_bkBotBlue = 0.756862745098039215686274509804;

    displayMode = GLSHADED;
    projectionType = GLORTHOGRAPHIC;

    m_antialias = false;

    ZoomON = false;
    PanON = false;
    RotON = false;

    gridOn = false;

    multiViewActive = false;

    myXmin=0;
    myYmin=0;
    myXmax=0;
    myYmax=0;

    myTrihedron = new CGLTrihedron;
    myGrid = new CGLGrid;


    /* Light values and coordinates*/
    ambientLight[0] = 0.3f;
    ambientLight[1] = 0.3f;
    ambientLight[2] = 0.3f;
    ambientLight[3] = 1.0f;
    diffuseLight[0] = 0.7f;
    diffuseLight[1] = 0.7f;
    diffuseLight[2] = 0.7f;
    diffuseLight[3] = 1.0f;
    specular[0] = 1.0f;
    specular[1] = 1.0f;
    specular[2] = 1.0f;
    specular[3] = 1.0f;
    specref[0] =  1.0f;
    specref[1] =  1.0f;
    specref[2] =  1.0f;
    specref[3] =  1.0f;
}

QGLView::~QGLView()
{
    delete myTrihedron;
    delete myGrid;
    delete myCamera;
}

void QGLView::InitGL()
{
    // Set up the rendering context, define display lists etc.:
    /* Insert your OpenGL initialization code here */
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,128);


    // Set drawing color to
    glColor3ub(156, 128, 255);

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

void QGLView::ReSize( int w, int h )
{
    // setup viewport, projection etc.:
    //

    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    /* Prevent a divide by zero*/
    winW = w;
    winH = h;
    int width = w; int height = h;
    if(h == 0)
	h = 1;

    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(projectionType == GLORTHOGRAPHIC)//orthographic projection
    {
	if(w <= h)
	    glOrtho(-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		    (nRange*h/w)-yTrans, -(nRange*50000.0f)-zTrans, (nRange*50000.0f)-zTrans);
	else
	    glOrtho(-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		    nRange-yTrans, -nRange*50000.0f, nRange*50000.0f);

    }

    if(projectionType == GLPERSPECTIVE)//perspective projection
    {
	if(w <= h)
	    gluPerspective(30, (GLdouble)h/(GLdouble)w, 1, nRange*50000.0f);
	else
	    gluPerspective(30, (GLdouble)w/(GLdouble)h, 1, nRange*50000.0f);
	glTranslatef(0, 0, -nRange*4);
    }

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    /* store limits for gradient background */
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject(0, 0, 1, modelMatrix, projMatrix, viewport, 
	    &win_xmin, &win_ymin, &winz);
    gluUnProject(width, height, 1, modelMatrix, projMatrix, viewport, 
	    &win_xmax, &win_ymax, &winz);
}

void QGLView::Draw()
{
    // call RenderScene():
    RenderScene(GL_RENDER);	
}

void QGLView::RenderScene(GLenum /*mode*/)
{
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // gradient background
    GLint mode;
    int w = winW; int h = winH;
    glGetIntegerv(GL_RENDER_MODE, &mode);
    if(mode == GL_RENDER)
    {
	glPushMatrix();	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBegin(GL_POLYGON);
	glColor3f(m_bkRed, m_bkGreen, m_bkBlue);	
	glVertex3f (win_xmin, win_ymax, nRange*5000);
	glVertex3f (win_xmax, win_ymax, nRange*5000);
	glColor3f(m_bkBotRed, m_bkBotGreen, m_bkBotBlue);
	glVertex3f (win_xmax, win_ymin, nRange*5000);
	glVertex3f (win_xmin, win_ymin, nRange*5000);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
    }
    // end gradient background
        
    if(m_antialias)
    {
	glEnable (GL_POINT_SMOOTH);
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth (1.5125);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_DEPTH_TEST);
    }
    else
    {
	glDisable (GL_LINE_SMOOTH);
	glLineWidth (1);
	glDepthFunc(GL_LESS);
	glDisable (GL_BLEND);
    }

    glPushMatrix();

    myCamera->Render();
    //Scene and View Rotations
    glRotatef(xRot,1,0,0);		//I am not happy with this b'cos the rotations are always
    glRotatef(yRot,0,1,0);		//about the origin and not about the screen center but
    glRotatef(zRot,0,0,1);		//haven,t yet found out any other method

    myTrihedron->SetSize(nRange/2);
    myTrihedron->Display();

    // grid displayed for projections only
    if(gridOn && ((myViewType == GLTOPVIEW) || (myViewType == GLBOTTOMVIEW) ||
		(myViewType == GLFRONTVIEW) || (myViewType == GLBACKVIEW) ||
		(myViewType == GLLEFTVIEW) || (myViewType == GLRIGHTVIEW)))
	myGrid->Display();

    //Do The Rendering
    if(mode == GL_RENDER)
    {
	theContext->Render(displayMode);
    }

    //Rendering Of Block Points
    if(!((VirmacView*)theWidget)->getDocument()->myBlocPointList->IsEmpty())
    {
	glInitNames();
	glPushName(0);
	CListIteratorOfListOfNCBlock anIter = ((VirmacView*)theWidget)->getDocument()->myBPListIter;
	for(anIter.Init(); anIter.More(); anIter.Next())
	{
	    CNCBlock aBloc = anIter.Current();
	    glPushAttrib(GL_COLOR_BUFFER_BIT);
	    glColor3ub(255, 255, 255);
	    glDisable(GL_LIGHTING);
	    glEnable (GL_POINT_SMOOTH);
	    glEnable (GL_BLEND);
	    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

	    glPointSize(3);
	    glLoadName(aBloc.N);
	    //QString str; str.sprintf("N %ld", aBloc.N);
	    //QMessageBox::information(this, "Info", str, QMessageBox::Ok, 0, 0);
	    glBegin(GL_POINTS);
	    glVertex3d(aBloc.X, aBloc.Y, aBloc.Z);
	    glEnd();
	    glPointSize(1);
	    glDisable (GL_POINT_SMOOTH);
	    glDisable (GL_BLEND);
	    glPopAttrib();
	}
    }
    //Rendering Of Selected Bloc;
    if(!((VirmacView*)theWidget)->getDocument()->mySelePointList->IsEmpty())
    {
	CListIteratorOfListOfNCBlock anIter = ((VirmacView*)theWidget)->getDocument()->mySPListIter;
	for(anIter.Init(); anIter.More(); anIter.Next())
	{
	    CNCBlock aBloc = anIter.Current();
	    glPushAttrib(GL_COLOR_BUFFER_BIT);
	    glColor3ub(255, 255, 0);
	    glDisable(GL_LIGHTING);
	    glEnable (GL_POINT_SMOOTH);
	    glEnable (GL_BLEND);
	    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

	    glPointSize(5);
	    glBegin(GL_POINTS);
	    glVertex3d(aBloc.X, aBloc.Y, aBloc.Z);
	    glEnd();
	    glPointSize(1);
	    glDisable (GL_POINT_SMOOTH);
	    glDisable (GL_BLEND);
	    glPopAttrib();
	}
    }



    glPopMatrix();

    glFlush();
}

void QGLView::ZoomView(QPoint dPoint, QPoint mPoint)
{
    if(!ZoomON)
	DownPoint = dPoint;
    ZoomON = true;

    if(DownPoint.x() > mPoint.x() || DownPoint.y() < mPoint.y())
	nRange *= 1.05;
    else
	nRange /= 1.05;

    DownPoint = mPoint;

    ReSize(winW, winH);
    theWidget->updateGL();
}

void QGLView::ZoomViewBy(const int amount)
{
    if(amount < 0)
	nRange *= abs(amount)/114.2857142857;
    else
	nRange /= abs(amount)/114.2857142857;


    ReSize(winW, winH);
    theWidget->updateGL();
}

void QGLView::PanView(QPoint dPoint, QPoint mPoint)
{
    if(!PanON)
	DownPoint = dPoint;
    PanON = true;
    QPoint pan = DownPoint - mPoint;
    DownPoint = mPoint;	

    myCamera->MoveAcross(0.01*nRange/2*pan.x());
    myCamera->Move(0.0, -0.01*nRange/2*pan.y(), 0.0);

    DownPoint = mPoint;

    ReSize(winW, winH);
    theWidget->updateGL();
}

void QGLView::RotateView(QPoint dPoint, QPoint mPoint)
{
    if(!RotON)
	DownPoint = dPoint;
    RotON = true;
    QPoint rotate = DownPoint - mPoint;
    myCamera->RotateX(rotate.y()/2);
    myCamera->RotateY(rotate.x()/2);
    DownPoint = mPoint;
    myViewType = GLUNDEFINEDVIEW;
    theWidget->updateGL();
}


void QGLView::TopView()
{	
    xRot = 0;
    yRot = 0;
    zRot = 0;
    myCamera->ResetAll();
    myViewType = GLTOPVIEW;
    viewLabel.sprintf("Top View");
    theWidget->updateGL();	
}

void QGLView::BottomView()
{	
    xRot = -180;
    yRot = 0;
    zRot = 0;
    myCamera->ResetAll();
    myViewType = GLBOTTOMVIEW;
    viewLabel.sprintf("Bottom View");
    theWidget->updateGL();	
}

void QGLView::FrontView()
{	
    xRot = -90;
    yRot = 0;
    zRot = 0;
    myCamera->ResetAll();
    myViewType = GLFRONTVIEW;
    viewLabel.sprintf("Front View");
    theWidget->updateGL();	
}

void QGLView::BackView()
{	
    xRot = -90;
    yRot = 0;
    zRot = -180;
    myCamera->ResetAll();
    myViewType = GLBACKVIEW;
    viewLabel.sprintf("Back View");
    theWidget->updateGL();	
}

void QGLView::LeftView()
{

    xRot = -90;
    yRot = 0;
    zRot = -90;
    myCamera->ResetAll();
    myViewType = GLLEFTVIEW;
    viewLabel.sprintf("Left View");
    theWidget->updateGL();	
}

void QGLView::RightView()
{

    xRot = -90;
    yRot = 0;
    zRot = 90;
    myCamera->ResetAll();
    myViewType = GLRIGHTVIEW;
    viewLabel.sprintf("Right View");
    theWidget->updateGL();	
}

void QGLView::AxonView()
{

    xRot = -45;
    yRot = 0;
    zRot = -45;
    myCamera->ResetAll();
    myViewType = GLAXONVIEW;
    viewLabel.sprintf("Axonometric");
    theWidget->updateGL();	
}

void QGLView::MultiView(bool status)
{
    multiViewActive = status;
}

#include <qimage.h>
void QGLView::SnapShot(const QString& fName)
{
    QImage px = theWidget->grabFrameBuffer();
    px.save(fName, "PNG");
}

bool QGLView::IsPlanarView() const
{
    bool b = (myViewType==GLTOPVIEW || myViewType==GLBOTTOMVIEW ||
	    myViewType==GLFRONTVIEW || myViewType==GLBACKVIEW ||
	    myViewType==GLLEFTVIEW || myViewType==GLRIGHTVIEW);
    return b;
}

void QGLView::FitAll()
{	
    if(theContext->IsEmpty())
	return;
    FitView();

    QRect Rect = theWidget->geometry();
    int w = Rect.width();
    int h = Rect.height();

    ReSize( w, h);
    theWidget->updateGL();	
}

void QGLView::ZoomWindow(QRect zcRect)
{		
    QRect rcRect = theWidget->geometry();

    int rcWidth, rcHeight, zcWidth, zcHeight;
    QPoint rcCentre, zcCentre;
    int dx, dy, dw, dh;
    GLdouble wRatio, hRatio;
    GLdouble dRange;

    GLint viewport[4];
    GLdouble mvmatrix[16],projmatrix[16];
    GLint realy;

    GLdouble cx, cy, cz;
    GLdouble mx, my, mz;

    rcWidth = rcRect.width();
    rcHeight = rcRect.height();

    zcWidth = zcRect.width();
    zcHeight = zcRect.height();

    rcCentre = rcRect.center();
    zcCentre = zcRect.center();

    theWidget->makeCurrent();
    glPushMatrix();
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    realy=viewport[3]-(GLint)zcCentre.y()-1;
    gluUnProject((GLdouble)zcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
	    &mx, &my, &mz);

    realy=viewport[3]-(GLint)rcCentre.y()-1;
    gluUnProject((GLdouble)rcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
	    &cx, &cy, &cz);

    glPopMatrix();

    dx = (int)(cx - mx);
    dy = (int)(cy - my);

    dw = rcWidth - zcWidth;
    dh = rcHeight - zcHeight;

    wRatio = (GLdouble) zcWidth/rcWidth;
    hRatio = (GLdouble) zcHeight/rcHeight;

    dRange = (wRatio < hRatio) ? hRatio : wRatio;

    nRange *= dRange/0.8;

    myCamera->MoveAcross(-dx);
    myCamera->Move(0.0, -dy, 0.0);

    ReSize(rcWidth, rcHeight);
    theWidget->updateGL();
    theWidget->erase();
}

void QGLView::ResetView()
{

    xRot = -45;
    yRot = 0;
    zRot = -45;
    xTrans = 0;
    yTrans = 0;
    zTrans = 0;
    nRange = 500;
    myCamera->ResetAll();

    myViewType = GLAXONVIEW;
    viewLabel.sprintf("Axonometric");

    QRect rect = theWidget->geometry();

    int w = rect.width();
    int h = rect.height();

    ReSize(w, h);
    theWidget->updateGL();
}

void QGLView::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b)
{
    m_bkRed = r;
    m_bkGreen = g;
    m_bkBlue = b;
    m_bkBotRed = r/0.4101;
    m_bkBotGreen = g/0.4101;
    m_bkBotBlue = b/0.4101;
    theWidget->makeCurrent();
    glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0f);
    theWidget->updateGL();
}

void QGLView::SetDisplayMode(const GLDisplayMode& aMode)
{
    displayMode = aMode;
    theWidget->updateGL();	
}

void QGLView::SetProjectionType(const GLProjectionType& aProj)
{
    projectionType = aProj;
    QRect rect = theWidget->geometry();

    int w = rect.width();
    int h = rect.height();

    ReSize(w, h);
    theWidget->updateGL();	
}

void QGLView::SetAntiAliasing(const bool& mode)
{
    m_antialias = mode;
    theWidget->updateGL();		
}

void QGLView::SetGridOn(const bool& mode)
{
    gridOn = mode;
    theWidget->updateGL();	
}

void QGLView::SetGridPara(const CPlane& plane, const double& size, const double& step)
{
    myGrid->SetPlane(plane);
    myGrid->SetSize(size);
    myGrid->SetStep(step);
    theWidget->updateGL();	
}

void QGLView::SetGridColor(const GLubyte& r, const GLubyte& g, const GLubyte& b)
{
    myGrid->SetColor(r, g, b);
}

void QGLView::PointToScreen(const CPoint3D& P, int& x, int& y)
{
    CPoint3D p = P;
    GLint viewport[4];
    GLdouble mvmatrix[16],projmatrix[16];
    GLdouble wx, wy, wz;
    theWidget->makeCurrent();
    glPushMatrix();    
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    myCamera->Render();
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
    gluProject((GLdouble)p.GetX(), p.GetY(), p.GetZ(), mvmatrix, projmatrix, viewport,
	    &wx, &wy, &wz);
    wx = sround(wx);
    wy = sround(wy);
    x = (int)wx; y = (int)wy;
    glPopMatrix();
}

void QGLView::ScreenToPoint(const int& x, const int& y, CPoint3D&  P)
{
    QPoint point(x,y);
    GLint viewport[4];
    GLdouble mvmatrix[16],projmatrix[16];
    GLint realy;
    GLdouble wx, wy, wz;
    theWidget->makeCurrent();
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    myCamera->Render();
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
    realy=viewport[3]-(GLint)point.y()-1;
    gluUnProject((GLdouble)point.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
	    &wx, &wy, &wz);
    P.SetParam(wx, wy, wz);
    glPopMatrix();
}

void QGLView::FitView()
{
    GLint viewport[4];
    GLdouble mvmatrix[16],projmatrix[16];
    GLdouble cx, cy, cz;
    GLdouble mx, my, mz;
    GLdouble lx, ly, lz, sx, sy, sz;
    CBoundingBox B = theContext->GetBoundingBox();
    lx = B.XMax(); ly = B.YMax(); lz = B.ZMax();
    sx = B.XMin(); sy = B.YMin(); sz = B.ZMin();

    theWidget->makeCurrent();
    glPushMatrix();

    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    myCamera->Render();

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(lx, ly, lz, mvmatrix, projmatrix, viewport,
	    &mx, &my, &mz);

    gluProject(sx, sy, sz, mvmatrix, projmatrix, viewport,
	    &cx, &cy, &cz);

    glPopMatrix();

    QPoint topr((int)cx,(int)cy), botl((int)mx,(int)my);
    QRect volRect(topr, botl);;
    volRect.normalize();

    Fit3d(volRect);
    theWidget->updateGL();
}

void QGLView::Fit3d(QRect rect)
{	
    QRect zcRect = rect;
    QRect rcRect = theWidget->geometry();

    int rcWidth, rcHeight, zcWidth, zcHeight;
    QPoint rcCentre, zcCentre;
    int dx, dy, dw, dh;
    GLdouble wRatio, hRatio;
    GLdouble dRange;

    GLint viewport[4];
    GLdouble mvmatrix[16],projmatrix[16];
    GLint realy;

    GLdouble cx, cy, cz;
    GLdouble mx, my, mz;

    rcWidth = rcRect.width();
    rcHeight = rcRect.height();

    zcWidth = zcRect.width();
    zcHeight = zcRect.height();

    rcCentre = rcRect.center();
    zcCentre = zcRect.center();

    theWidget->makeCurrent();

    glPushMatrix();

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    realy=viewport[3]-(GLint)zcCentre.y()-1;
    gluUnProject((GLdouble)zcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
	    &mx, &my, &mz);

    realy=viewport[3]-(GLint)rcCentre.y()-1;
    gluUnProject((GLdouble)rcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
	    &cx, &cy, &cz);

    glPopMatrix();

    dx = (int)(cx - mx);
    dy = (int)(cy - my);

    dw = rcWidth - zcWidth;
    dh = rcHeight - zcHeight;

    wRatio = (GLdouble) zcWidth/rcWidth;
    hRatio = (GLdouble) zcHeight/rcHeight;

    dRange = (wRatio < hRatio) ? hRatio : wRatio;

    nRange *= dRange/(double)0.8;

    myCamera->SetPosition(0,0,0);
    xTrans += dx;
    yTrans -= dy;

    theWidget->updateGL();	
}

void QGLView::Select(const int &x, const int &y)
{
    theContext->Select(this, x, y);
    theWidget->updateGL();	
}

void QGLView::MultiSelect(const int &x, const int &y)
{
    theContext->MultiSelect(this, x, y);
    theWidget->updateGL();		
}

void QGLView::SweepSelect(const QRect& swRect)
{
    theContext->SweepSelect(this, swRect);
    theWidget->updateGL();		
}

int QGLView::ProcessSelection(const int& xPos, const int& yPos , const int& sensitivity)
{
    int id = 0;

    theWidget->makeCurrent();

    // Space for selection buffer
    GLuint selectBuff[512];

    // Hit counter and viewport storeage
    GLint hits, viewport[4];

    // Setup selection buffer
    glSelectBuffer(512, selectBuff);

    // Get the viewport
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Switch to projection and save the matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    // Change render mode
    glRenderMode(GL_SELECT);

    // Establish new clipping volume to be unit cube around
    // mouse cursor point (xPos, yPos) and extending n pixels
    // in the vertical and horzontal direction
    int n = sensitivity;
    glLoadIdentity();
    gluPickMatrix(xPos, viewport[3] - yPos, n, n, viewport);

    QRect rect = theWidget->geometry();

    int w = rect.width();
    int h = rect.height();

    if(projectionType == GLORTHOGRAPHIC)//orthographic projection
    {
	if(w <= h)
	    glOrtho(-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		    (nRange*h/w)-yTrans, -(nRange*50000.0f)-zTrans, (nRange*50000.0f)-zTrans);
	else
	    glOrtho(-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		    nRange-yTrans, -nRange*50000.0f, nRange*50000.0f);
    }

    if(projectionType == GLPERSPECTIVE)//perspective projection
    {
	if(w <= h)
	    gluPerspective(30, (GLdouble)h/(GLdouble)w, 1, nRange*50000.0f);
	else
	    gluPerspective(30, (GLdouble)w/(GLdouble)h, 1, nRange*50000.0f);
	glTranslatef(0, 0, -nRange*4);
    }

    // Draw the scene
    RenderScene(GL_SELECT);

    // Collect the hits
    hits = glRenderMode(GL_RENDER);

    // Restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Go back to modelview for normal rendering
    glMatrixMode(GL_MODELVIEW);

    // If a single hit occured, display the info.
    if(hits)
    {
	id =  selectBuff[3];		
    }

    return id;

}

int QGLView::ProcessSweepSelection(const QRect& swRect, unsigned long int* id)
{
    GLint i;
    GLuint j;
    theWidget->makeCurrent();

    // Space for selection buffer
    GLuint selectBuff[512];

    // Hit counter and viewport storeage
    GLint hits = 0, viewport[4];

    // Setup selection buffer
    glSelectBuffer(512, selectBuff);

    // Get the viewport
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Switch to projection and save the matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    // Change render mode
    glRenderMode(GL_SELECT);

    // Establish new clipping volume to be cube around
    // center point of sweeping rectangle and extending pixels
    // falling within the height and width of the sweep rectangle
    // in the vertical and horzontal direction
    glLoadIdentity();
    gluPickMatrix(swRect.center().x(), viewport[3] - swRect.center().y(), swRect.width(), swRect.height(), viewport);

    QRect trect = theWidget->geometry();

    int w = trect.width();
    int h = trect.height();

    if(projectionType == GLORTHOGRAPHIC)//orthographic projection
    {
	if(w <= h)
	    glOrtho(-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		    (nRange*h/w)-yTrans, -(nRange*50000.0f)-zTrans, (nRange*50000.0f)-zTrans);
	else
	    glOrtho(-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		    nRange-yTrans, -nRange*50000.0f, nRange*50000.0f);
    }

    if(projectionType == GLPERSPECTIVE)//perspective projection
    {
	if(w <= h)
	    gluPerspective(30, (GLdouble)h/(GLdouble)w, 1, nRange*50000.0f);
	else
	    gluPerspective(30, (GLdouble)w/(GLdouble)h, 1, nRange*50000.0f);
	glTranslatef(0, 0, -nRange*4);
    }

    // Draw the scene
    RenderScene(GL_SELECT);

    // Collect the hits
    hits = glRenderMode(GL_RENDER);

    // Restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Go back to modelview for normal rendering
    glMatrixMode(GL_MODELVIEW);

    // If hits occured, display the info.
    if(hits)
    {
	GLuint *ptr, names;
	ptr = (GLuint*) selectBuff;
	for(i=0; i < hits; i++)
	{
	    names = *ptr;
	    ptr++; ptr++; ptr++;
	    for(j = 0; j < names; j++)
	    {
		id[i] = *ptr;
		ptr++;
	    }
	}
    }

    return hits;
}

