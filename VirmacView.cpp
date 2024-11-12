/***************************************************************************
  virmacview.cpp  -  description
  -------------------
begin                : Thu Dec  4 09:55:15 IST 2003
copyright            : (C) 2003 by Sharjith Nair
email                : sharjith_ssn@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "VirmacView.h"
#include "VirmacDoc.h"
#include "Virmac.h"

#include "images/pancursor.xpm"
#include "images/rotatecursor.xpm"
#include "images/zoomcursor.xpm"
#include "images/crosscursor.xpm"

    VirmacView::VirmacView(VirmacDoc* pDoc, QWidget *parent, const char* name, int /*wflags*/)
: QGLWidget(parent, name)
{
    doc=pDoc;

    viewLabel.sprintf("Axonometric");

    myView = new QGLView(this, doc->GetDisplayContext());

    ldown = mdown = rdown = false;
    winZoom = false;
    dShaded = false;
    antialiased = false;
    gridOn = false;
    showArc = false;
    showPath = true;
    showTool = true;
    showTrail = true;
    showModel = true;
    showNormal = false;
    showCoord = false;
    pickMode = false;
    pickNplay = false;
    pickNplayActive = false;
    trimAfter = false;
    trimBefore = false;
    multiViewActive = false;
    animActive = false;
    editorActive = false;

    setMouseTracking(TRUE);
}

VirmacView::~VirmacView()
{
    if(myView)
	delete myView;
}

VirmacDoc *VirmacView::getDocument() const
{
    return doc;
}

    void VirmacView::update(VirmacView* pSender){
	if(pSender != this)
	    repaint();
    }

void VirmacView::print(QPrinter *pPrinter)
{
    if (pPrinter->setup(this))
    {
	QPainter p;
	p.begin(pPrinter);

	///////////////////////////////
	// TODO: add your printing code here
	///////////////////////////////

	p.end();
    }
}

void VirmacView::closeEvent(QCloseEvent*)
{
    // LEAVE THIS EMPTY: THE EVENT FILTER IN THE VirmacApp CLASS TAKES CARE FOR CLOSING
    // QWidget closeEvent must be prevented.
}

void VirmacView::initializeGL()
{
    myView->InitGL();    
}

void VirmacView::resizeGL(int w, int h)
{
    myView->ReSize(w, h); 
}

void VirmacView::paintGL()
{
    myView->Draw();

    //View Label
    glDisable(GL_LIGHTING);
    glColor3ub(255, 255, 0);
    renderText(10, 20, viewLabel);
    //End Label

    //Dynamic Coord
    if(myView->IsPlanarView() && !pickMode && myView->GetProjectionType() == GLORTHOGRAPHIC && !ldown && !mdown && !rdown && showCoord)
    {
	VirmacApp *top = (VirmacApp*)topLevelWidget()->topLevelWidget();
	glColor3ub(255, 255, 0);
	renderText(mouseX, mouseY, top->cViewer->text());
    }
    //End Coord
}

void VirmacView::resizeEvent( QResizeEvent* /*e*/)
{
    QRect aRect = geometry();
    resizeGL(aRect.width(), aRect.height());
    update(NULL);
}

void VirmacView::keyPressEvent ( QKeyEvent * event )
{
    //int x, y;
    switch(event->key())
    {
	case Qt::Key_Left:
	    if(event->state() & Qt::ShiftButton)
	    {
		if(myView->IsPlanarView())
		{
		    VirmacApp* pApp = (VirmacApp*)qApp->mainWidget();
		    pApp->setMessage(tr("Rotation allowed in axonometric view only!"));
		}
		else
		{
		    myView->GetCamera()->RotateY(5);
		}
	    }
	    else if(event->state() & Qt::AltButton/*ALT Key*/)
	    {
		myView->GetCamera()->MoveAcross(5);
	    }
	    /*
	    else if(myView->IsPlanarView())
	    {
		//move the cursor
		gdk_display_get_pointer(gtk_widget_get_display(widget),
			NULL,
			&x,
			&y,
			NULL);
		gdk_display_warp_pointer(gtk_widget_get_display(widget),
			gtk_window_get_screen(GTK_WINDOW(widget)),
			x-5,
			y);
	    }
	    */
	    break;
	case Qt::Key_Right:
	    if(event->state() & Qt::ShiftButton)
	    {
		if(myView->IsPlanarView())
		{
		    VirmacApp* pApp = (VirmacApp*)qApp->mainWidget();
		    pApp->setMessage(tr("Rotation allowed in axonometric view only!"));		    
		}
		else
		{
		    myView->GetCamera()->RotateY(-5);
		}
	    }
	    else if(event->state() & Qt::AltButton/*ALT Key*/)
	    {
		myView->GetCamera()->MoveAcross(-5);
	    }
	    /*
	    else if(myView->IsPlanarView())
	    {
		//move the cursor
		gdk_display_get_pointer(gtk_widget_get_display(widget),
			NULL,
			&x,
			&y,
			NULL);
		gdk_display_warp_pointer(gtk_widget_get_display(widget),
			gtk_window_get_screen(GTK_WINDOW(widget)),
			x+5,
			y);
	    }
	    */
	    break;
	case Qt::Key_Up:
	    if(event->state() & Qt::ShiftButton)
	    {
		if(myView->IsPlanarView())
		{
		    VirmacApp* pApp = (VirmacApp*)qApp->mainWidget();
		    pApp->setMessage(tr("Rotation allowed in axonometric view only!"));
		}
		else
		{
		    myView->GetCamera()->RotateX(5);
		}
	    }
	    else if(event->state() & Qt::AltButton/*ALT Key*/)
	    {
		myView->GetCamera()->Move(0.0, -5.0, 0.0);
	    }
	    else if(event->state() & Qt::ControlButton/*CTRL Key*/)
	    {
		myView->ZoomViewBy(120);
	    }
	    /*
	    else if(myView->IsPlanarView())
	    {
		//move the cursor
		gdk_display_get_pointer(gtk_widget_get_display(widget),
			NULL,
			&x,
			&y,
			NULL);
		gdk_display_warp_pointer(gtk_widget_get_display(widget),
			gtk_window_get_screen(GTK_WINDOW(widget)),
			x,
			y-5);
	    }
	    */
	    break;
	case Qt::Key_Down:
	    if(event->state() & Qt::ShiftButton)
	    {
		if(myView->IsPlanarView())
		{
		    VirmacApp* pApp = (VirmacApp*)qApp->mainWidget();
		    pApp->setMessage(tr("Rotation allowed in axonometric view only!"));
		}
		else
		{
		    myView->GetCamera()->RotateX(-5);
		}
	    }
	    else if(event->state() & Qt::AltButton/*ALT Key*/)
	    {
		myView->GetCamera()->Move(0.0, 5.0, 0.0);
	    }
	    else if(event->state() & Qt::ControlButton/*CTRL Key*/)
	    {
		myView->ZoomViewBy(-120);
	    }
	    /*
	    else if(myView->IsPlanarView())
	    {
		//move the cursor
		gdk_display_get_pointer(gtk_widget_get_display(widget),
			NULL,
			&x,
			&y,
			NULL);
		gdk_display_warp_pointer(gtk_widget_get_display(widget),
			gtk_window_get_screen(GTK_WINDOW(widget)),
			x,
			y+5);
	    }
	    */
	    break;
	case Qt::Key_PageUp:
	    myView->ZoomViewBy(120);
	    break;
	case Qt::Key_PageDown:
	    myView->ZoomViewBy(-120);
	    break;
	case Qt::Key_M:
	    showCoord = !showCoord;
	    break;
    }
    updateGL();
}
	
void VirmacView::mousePressEvent( QMouseEvent *e)
{
    if(e->button() & QMouseEvent::LeftButton)
    {
	// ask window to give us events even if outside the view
	if(!pickMode)
	    grabMouse();

	//  save the current mouse coordinate in min
	myXmin=e->x();  myYmin=e->y();
	myXmax=e->x();  myYmax=e->y();

	ldown = true;
	lPoint.setX(e->x());
	lPoint.setY(e->y());
	if(!(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
	{
	    getDocument()->EraseSelectedBloc();
	    myView->Select(e->x(), e->y());

	}
	if ((e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
	{
	    myView->MultiSelect(e->x(), e->y());

	}	
    }
    if(e->button() & QMouseEvent::MidButton)
    {
	mdown = true;
	mPoint.setX(e->x());
	mPoint.setY(e->y());
    }
    if(e->button() & QMouseEvent::RightButton)
    {
	rdown = true;
	rPoint.setX(e->x());
	rPoint.setY(e->y());
	if(!(e->state() & ControlButton) && !(e->state() & ShiftButton))
	    Popup();
    }
}

void VirmacView::mouseReleaseEvent( QMouseEvent *e)
{
    if(e->button() & QMouseEvent::LeftButton)
    {
	ldown = false;
	myView->StopZooming();

	if(!(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
	{
	    DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
	    myXmax=e->x();
	    myYmax=e->y();
	    QPoint point(e->x(), e->y());
	    QRect arect(lPoint, point);
	    arect.normalize();
	    if(arect.width() && arect.height())
		myView->SweepSelect(arect);
	}

	if((e->state() & ControlButton))
	{
	    setCursor(Qt::ArrowCursor);
	}

	if(winZoom)
	{
	    DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
	    myXmax=e->x();
	    myYmax=e->y();
	    QRect myZoomRect(myXmin, myYmax, abs(myXmax-myXmin), abs(myYmax-myYmin));
	    if ((abs(myXmin-myXmax)>1) || (abs(myYmin-myYmax)>1))
		// Test if the zoom window size is not null
	    {
		myView->ZoomWindow(myZoomRect);
	    }
	    setCursor(Qt::ArrowCursor);
	}

	if(pickMode && !(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
	{
	    QPoint point(e->x(), e->y());
	    ShowPickedBlockInfo(point);
	}

	winZoom = false;
	releaseMouse();
    }
    if(e->button() & QMouseEvent::MidButton)
    {
	mdown = false;
	myView->StopPanning();
	QWidget::update();
	if((e->state() & ControlButton))
	{
	    setCursor(Qt::ArrowCursor);
	}
    }

    if(e->button() & QMouseEvent::RightButton)
    {
	rdown = false;
	myView->StopRotation();
	if((e->state() & ControlButton))
	{
	    setCursor(Qt::ArrowCursor);
	}
    }
}

void VirmacView::mouseMoveEvent( QMouseEvent *e)
{
    QPoint point(e->x(), e->y());
    mouseX = e->x();
    mouseY = e->y();

    VirmacApp *top = (VirmacApp*)topLevelWidget()->topLevelWidget();
    QString str;
    CPoint3D C;
    myView->ScreenToPoint(e->x(), e->y(), C);
    GLViewType vt = myView->GetViewType();
    bool b = (myView->GetProjectionType() == GLORTHOGRAPHIC);
    if(vt == GLTOPVIEW || vt == GLBOTTOMVIEW)
	str.sprintf(" X  %0.3lf  Y  %0.3lf",C.GetX(), C.GetY());
    else if(vt == GLFRONTVIEW || vt == GLBACKVIEW)
	str.sprintf(" X  %0.3lf  Z  %0.3lf",C.GetX(), C.GetZ());
    else if(vt == GLLEFTVIEW || vt == GLRIGHTVIEW)
	str.sprintf(" Y  %0.3lf  Z  %0.3lf", C.GetY(), C.GetZ());
    if(b)
	top->cViewer->setText(str);

    if(!(e->state() & ControlButton) && !(e->state() & ShiftButton))
    {
	if(myView->IsPlanarView() && !pickMode)
	{
	    setCursor(QCursor(crosscursor));
	}
    }
    if(ldown && !(e->state() & ControlButton) && !(e->state() & ShiftButton))
    {
	myXmax = point.x(); myYmax = point.y();
	DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
	DrawRectangle(myXmin,myYmin,myXmax,myYmax,true);
    }
    if(e->state() & ControlButton)
    {
	if(ldown)
	{
	    myView->ZoomView(lPoint, point);
	    setCursor(QCursor(zoomcursor));
	}
	else if(mdown)
	{
	    myView->PanView(mPoint, point);
	    setCursor(QCursor(pancursor));
	}
	else if(rdown)
	{
	    if(myView->GetViewType() == GLAXONVIEW || myView->GetViewType() == GLUNDEFINEDVIEW)
	    {
		myView->RotateView(rPoint, point);
		setCursor(QCursor(rotatecursor));
	    }
	    else
	    {
		VirmacApp* pApp = (VirmacApp*)qApp->mainWidget();
		pApp->setMessage(tr("Rotation allowed in axonometric view only!"));
	    }
	}
    }
    if(myView->IsPlanarView()&& !ldown && !mdown && !rdown)
    {
	updateGL();
    }
}

void VirmacView::wheelEvent(QWheelEvent *e)
{
    myView->ZoomViewBy(e->delta());
}

void VirmacView::timerEvent(QTimerEvent* /*e*/)
{
    VirmacApp* app = (VirmacApp*)qApp->mainWidget();
    VirmacDoc* doc = getDocument();
    if(!doc)
	return;
    doc->AnimFunction();
    app->getBlockLabel()->setText(doc->blocString);
    myView->Refresh();
}

void VirmacView::SetDisplayMode(const GLDisplayMode& aMode)
{
    myView->SetDisplayMode(aMode);
}

void VirmacView::TopView()
{
    myView->SetGridPlane(CPlane::XOY());
    viewLabel.sprintf("Top View");
    myView->TopView();
}

void VirmacView::BottomView()
{
    myView->SetGridPlane(CPlane::XOY());
    viewLabel.sprintf("Bottom View");
    myView->BottomView();
}

void VirmacView::FrontView()
{
    myView->SetGridPlane(CPlane::ZOX());
    viewLabel.sprintf("Front View");
    myView->FrontView();
}

void VirmacView::BackView()
{
    myView->SetGridPlane(CPlane::ZOX());
    viewLabel.sprintf("Back View");
    myView->BackView();
}

void VirmacView::RightView()
{
    myView->SetGridPlane(CPlane::YOZ());
    viewLabel.sprintf("Right View");
    myView->RightView();
}

void VirmacView::LeftView()
{
    myView->SetGridPlane(CPlane::YOZ());
    viewLabel.sprintf("Left View");
    myView->LeftView();
}

void VirmacView::AxonView()
{
    viewLabel.sprintf("Axonometric");
    myView->AxonView();
}


void VirmacView::DrawRectangle (const int  MinX  ,
	const int  MinY  ,
	const int  MaxX  ,
	const int  MaxY  ,
	const bool  Draw  )
{
    QPainter thePainter(this);
    thePainter.setRasterOp(Qt::XorROP);
    thePainter.setPen(Qt::white);

    static		int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
    static		bool m_IsVisible;

    QRect aRect;
    if ( m_IsVisible && !Draw) // move or up  : erase at the old position
    {
	aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
	thePainter.drawRect(aRect);
	m_IsVisible = FALSE;
    }

    StoredMinX = (MinX < MaxX) ? MinX: MaxX ;
    StoredMinY = (MinY < MaxY) ? MinY: MaxY ;
    StoredMaxX = (MinX > MaxX) ? MinX: MaxX ;
    StoredMaxY = (MinY > MaxY) ? MinY: MaxY ;

    if (Draw) // move : draw
    {
	aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
	thePainter.drawRect(aRect);
	m_IsVisible = TRUE;
    }
}

void VirmacView::Popup()
{

    VirmacApp* pApp = (VirmacApp*)qApp->mainWidget();
    QPopupMenu *pPopup = new QPopupMenu();
    pPopup->insertTearOffHandle();
    pApp->bgColor->addTo(pPopup);
    pPopup->insertSeparator();
    pPopup->insertItem(tr("Grid"), pApp->pGridMenu);
    pPopup->popup(QCursor::pos());
    rdown = false;
}

#include "RetractPara.h"
#include "CircEntryPara.h"
void VirmacView::ShowPickedBlockInfo(QPoint point)
{
    VirmacDoc* doc = getDocument();
    if(doc)
    {
	pickMode = false;
	CNCBlock aBloc;
	CPoint3D P;
	unsigned long int id = myView->ProcessSelection(point.x(), point.y(), 5);
	for(doc->myPathIter.Init(); doc->myPathIter.More();doc->myPathIter.Next())
	{
	    aBloc = doc->myPathIter.Current();
	    if(aBloc.N == id)
		break;
	}
	QString str; str.sprintf("N%d  X%lf  Y%lf Z%lf", (int)aBloc.N, aBloc.X, aBloc.Y, aBloc.Z);
	((VirmacApp*)qApp->mainWidget())->setMessage(str);
	QMessageBox::information(this, "NC Block Information", str, QMessageBox::Ok);
	doc->ShowSelectedBloc(aBloc);
	doc->HidePath(false);
	if(pickNplay)
	{
	    pickNplay = false;
	    VirmacApp* mw = (VirmacApp*)qApp->mainWidget();
	    str.sprintf("Simulation Starting At Block N%d  X%lf  Y%lf Z%lf", (int)aBloc.N, aBloc.X, aBloc.Y, aBloc.Z);
	    mw->setMessage(str);                 
	    pickNplayActive = true;
	    mw->slotPlay();
	}
	if(trimAfter)
	{
	    CListOfNCBlock* myNewList = new CListOfNCBlock;
	    CListIteratorOfListOfNCBlock myNewListIter(myNewList);                
	    for(doc->myPathIter.Init(); doc->myPathIter.More(); doc->myPathIter.Next())
	    {
		myNewList->Append(doc->myPathIter.Current());
		if(doc->myPathIter.Current().N == aBloc.N)
		    break;
	    }
	    doc->myPath->myGCodeList->Clear();
	    doc->myPath->SetBlockCount(0);
	    int cnt = 0;
	    for(myNewListIter.Init(); myNewListIter.More(); myNewListIter.Next())
	    {
		doc->myPath->AddBlock(myNewListIter.Current());
		++cnt;
	    }
	    doc->myPath->SetBlockCount(cnt);
	    QString str;
	    str.sprintf("Path Trimmed After Bloc N%ld", aBloc.N);
	    ((VirmacApp*)qApp->mainWidget())->setMessage(str);
	    ((VirmacApp*)qApp->mainWidget())->setMessage("Do You Want To Add A Circular Exit?");
	    if(QMessageBox::information(this, "Tool Exit Option", "Do You Want To Add A Circular Exit?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
	    {
		AddCircularExit();
		((VirmacApp*)qApp->mainWidget())->setMessage("Circular Exit Added!");
	    }

	    RetractPara dlg;
	    if(dlg.exec() == QDialog::Accepted)
	    {
		CNCBlock H = doc->myPath->myGCodeList->Last();
		CNCBlock P;

		//Disengagement
		P.G = (1);
		P.X = H.X;
		P.Y = H.Y;
		P.Z = (H.Z+dlg.lineEditEng->text().toDouble());
		doc->myPath->myGCodeList->Append(P);
		doc->myPath->SetBlockCount(doc->myPath->GetBlockCount() + 1);
		//Clearance Retract
		P.G = 0;
		P.Z = (H.Z+dlg.lineEdit6->text().toDouble());
		doc->myPath->myGCodeList->Append(P);
		doc->myPath->myGCodeList->Append(P);
		doc->myPath->SetBlockCount(doc->myPath->GetBlockCount() + 2);
	    }
	    trimAfter = false;
	    doc->myPathIter.Init();
	    doc->myPath->RefreshPath();
	}
	else if(trimBefore)
	{
	    CListOfNCBlock* myNewList = new CListOfNCBlock;
	    CListIteratorOfListOfNCBlock myNewListIter(myNewList);
	    for(doc->myPathIter.Init(); doc->myPathIter.More(); doc->myPathIter.Next())
	    {                    
		if(doc->myPathIter.Current().N == aBloc.N)
		    break;
	    }
	    while(doc->myPathIter.More())
	    {
		myNewList->Append(doc->myPathIter.Current());
		doc->myPathIter.Next();          
	    }
	    doc->myPath->myGCodeList->Clear();
	    doc->myPath->SetBlockCount(0);
	    int cnt = 0;
	    for(myNewListIter.Init(); myNewListIter.More(); myNewListIter.Next())
	    {
		doc->myPath->AddBlock(myNewListIter.Current());
		++cnt;
	    }
	    doc->myPath->SetBlockCount(cnt);
	    QString str;
	    str.sprintf("Path Trimmed Before Bloc N%ld", aBloc.N);
	    ((VirmacApp*)qApp->mainWidget())->setMessage(str);
	    ((VirmacApp*)qApp->mainWidget())->setMessage("Do You Want To Add A Circular Entry?");
	    if(QMessageBox::information(this, "Tool Exit Option", "Do You Want To Add A Circular Entry?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
	    {
		AddCircularEntry();
		((VirmacApp*)qApp->mainWidget())->setMessage("Circular Entry Added!");
	    }

	    RetractPara dlg;
	    if(dlg.exec() == QMessageBox::Accepted)
	    {
		CNCBlock H = doc->myPath->myGCodeList->First();
		CNCBlock P;
		//Engagement
		P.G = 0;
		P.X = H.X;
		P.Y = H.Y;
		P.Z = (H.Z + dlg.lineEditEng->text().toDouble());
		doc->myPath->myGCodeList->Prepend(P);
		doc->myPath->SetBlockCount(doc->myPath->GetBlockCount() + 1);
		//Positioning
		P.G = 0;
		P.X = H.X;
		P.Y = H.Y;
		P.Z = (H.Z + dlg.lineEdit6->text().toDouble());
		doc->myPath->myGCodeList->Prepend(P);
		doc->myPath->SetBlockCount(doc->myPath->GetBlockCount() + 1);
		//Clearance Plane
		P.G = 0;
		P.Z = (H.Z + dlg.lineEdit6->text().toDouble());
		doc->myPath->myGCodeList->Prepend(P);
		doc->myPath->SetBlockCount(doc->myPath->GetBlockCount() + 1);
	    }
	    trimBefore = false;
	    doc->myPathIter.Init();
	    doc->myPath->RefreshPath();
	}

	doc->ShowPoints(false);
	doc->HideTool(false);
	setCursor(QCursor(Qt::ArrowCursor));
	if(!pickNplayActive)
	    doc->myPathIter.Init();
	myView->Refresh();
    }
}

#include "images/pickcursor.xpm"
void VirmacView::PickBlock()
{
    pickMode = true;
    //doc->HidePath(true);
    doc->ShowPoints(true);
    doc->HideTool(true);
    myView->Refresh();
    setCursor(QCursor(pickcursor));
    ((VirmacApp*)qApp->mainWidget())->setPrompt("Pick A Point To Get The Block Number");
}

void VirmacView::AddCircularEntry()
{
    VirmacDoc* pDoc = getDocument();
    CNCBlock stBlock = pDoc->myPath->myGCodeList->First();
    CNCBlock nxBlock = pDoc->myPathIter.ValueAt(2);
    CVector3D OP(stBlock.X, stBlock.Y, 0);
    CVector3D XP(nxBlock.X, nxBlock.Y, 0);
    CVector3D V1(OP, XP);
    CVector3D Vz(OP, CVector3D(OP.GetX(),OP.GetY(),OP.GetZ()+10)), Vr;

    bool CW = true;
    double rad = 10;
    CircEntryPara dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
	CW = dlg.radioButton1->isOn();
	rad = dlg.lineEditVal->text().toDouble();
    }

    V1 = V1.Unit();
    Vz = Vz.Unit();
    if(CW)
	Vr = V1.Crossed(Vz);
    else
	Vr = Vz.Crossed(V1);
    Vr = Vr.Unit();
    Vr *= rad;
    CVector3D cen(stBlock.X+Vr.GetX(), stBlock.Y+Vr.GetY(), stBlock.Z);

    CVector3D aV1(OP, cen);
    CVector3D aVz(OP, CVector3D(OP.GetX(),OP.GetY(),OP.GetZ()+10)), aVr;
    aV1 = aV1.Unit();
    aVz = aVz.Unit();
    if(CW)
	aVr = aV1.Crossed(aVz);
    else
	aVr = aVz.Crossed(aV1);
    aVr = aVr.Unit();
    aVr *= rad;

    CNCBlock &aBloc = pDoc->myPath->myGCodeList->First();

    if(CW)
	aBloc.G = 2;
    else
	aBloc.G = 3;
    aBloc.I = cen.GetX(); aBloc.J = cen.GetY();

    CNCBlock G1;
    G1.G = 1;
    G1.X = (cen.GetX()+aVr.GetX());  G1.Y = (cen.GetY()+aVr.GetY());  G1.Z = (cen.GetZ());
    pDoc->myPath->myGCodeList->Prepend(G1);

}

void VirmacView::AddCircularExit()
{
    VirmacDoc* pDoc = getDocument();
    CNCBlock endBlock = pDoc->myPath->myGCodeList->Last();
    CNCBlock prevBlock = pDoc->myPathIter.ValueAt(pDoc->myPath->GetBlockCount() - 1);
    CVector3D OP(prevBlock.X, prevBlock.Y, 0);
    CVector3D XP(endBlock.X, endBlock.Y, 0);
    CVector3D V1(OP, XP);
    CVector3D Vz(OP, CVector3D(OP.GetX(),OP.GetY(),OP.GetZ()+10)), Vr;

    bool CW = true;
    double rad = 10;
    CircEntryPara dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
	CW = dlg.radioButton1->isOn();
	rad = dlg.lineEditVal->text().toDouble();
    }

    V1 = V1.Unit();
    Vz = Vz.Unit();
    if(CW)
	Vr = V1.Crossed(Vz);
    else
	Vr = Vz.Crossed(V1);
    Vr = Vr.Unit();
    Vr *= rad;
    CVector3D cen(endBlock.X+Vr.GetX(), endBlock.Y+Vr.GetY(), endBlock.Z);

    CVector3D aV1(XP, cen);
    CVector3D aVz(XP, CVector3D(XP.GetX(),XP.GetY(),XP.GetZ()+10)), aVr;
    aV1 = aV1.Unit();
    aVz = aVz.Unit();
    if(CW)
	aVr = aV1.Crossed(aVz);
    else
	aVr = aVz.Crossed(aV1);
    aVr = aVr.Unit();
    aVr *= rad;

    CNCBlock aBloc;

    if(CW)
	aBloc.G = 2;
    else
	aBloc.G = 3;
    aBloc.X = (cen.GetX()-aVr.GetX());  aBloc.Y = (cen.GetY()-aVr.GetY());  aBloc.Z = (cen.GetZ());
    aBloc.I = (cen.GetX()); aBloc.J = (cen.GetY());
    pDoc->myPath->myGCodeList->Append(aBloc);   
}

void VirmacView::MultiView(bool doIt)
{
    if(doIt)
    {
	multiViewActive = true;
	myView->MultiView(true);
    }
    else
    {
	multiViewActive = false;
	myView->MultiView(false);
    }
}

