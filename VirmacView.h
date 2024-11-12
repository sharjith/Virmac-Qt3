/***************************************************************************
  virmacview.h  -  description
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

#ifndef VIRMACVIEW_H
#define VIRMACVIEW_H

#include "Virmac.h"

// include files for Qt
#include <qwidget.h>
#include <qgl.h>                 
#include <qlabel.h>
#include <qhbox.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qpoint.h>
#include <qsplitter.h>

#include "QGLView.h"

class VirmacDoc;
class QGLView;

class VirmacView : public QGLWidget
{
    Q_OBJECT

	friend class VirmacDoc;

    public:
    VirmacView(VirmacDoc* pDoc, QWidget* parent, const char *name, int wflags);
    ~VirmacView();

    VirmacDoc *getDocument() const;

    void update(VirmacView* pSender);
    void print(QPrinter *pPrinter);

    protected:
    virtual void closeEvent(QCloseEvent*);

    VirmacDoc *doc;
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    private:
    void Popup();
    void DrawRectangle (const int  MinX  ,
	    const int  MinY  ,
	    const int  MaxX  ,
	    const int  MaxY  ,
	    const bool  Draw
	    );
    public:
    bool winZoom;
    bool dShaded;
    bool antialiased;
    bool gridOn;
    bool showArc;
    bool showPath;
    bool showTool;
    bool showTrail;
    bool showModel;
    bool showNormal;
    bool showCoord;
    bool pickMode;
    bool pickNplay;
    bool pickNplayActive;
    bool trimAfter;
    bool trimBefore;
    bool multiViewActive;
    bool animActive;
    bool editorActive;

    QString viewLabel;
    QLabel *coordViewer;
    private:
    bool ldown;
    bool mdown;
    bool rdown;

    QPoint  lPoint;
    QPoint  mPoint;
    QPoint  rPoint;

    int     myXmin;
    int     myYmin;
    int     myXmax;
    int     myYmax;

    protected:
    void keyPressEvent ( QKeyEvent *  );
    void mousePressEvent( QMouseEvent *);
    void mouseReleaseEvent( QMouseEvent *);
    void mouseMoveEvent( QMouseEvent *);
    void wheelEvent( QWheelEvent *);
    void resizeEvent( QResizeEvent *);
    void timerEvent(QTimerEvent*);
    public:
    void TopView();
    void BottomView();
    void FrontView();
    void BackView();
    void RightView();
    void LeftView();
    void AxonView();
    void MultiView(bool doIt);
    void SetDisplayMode(const GLDisplayMode& aMode);
    GLDisplayMode GetDisplayMode() const { return myView->GetDisplayMode(); }
    QGLView* GetView() const { return myView; }
    void ShowPickedBlockInfo(QPoint point);
    void PickBlock();
    void AddCircularEntry();
    void AddCircularExit();
    private:
    QGLView* myView;

    private:
    int mouseX;
    int mouseY;
};

#endif
