#ifndef __TOOLVIEWER_H__
#define __TOOLVIEWER_H__

#include <qwidget.h>
#include <qgl.h>
#include <qimage.h>
#include <qcolor.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "GLObject.h"

#ifndef M_PI
#define M_PI 3.14285714285714285714 
#endif
/* Custom OpenGL Viewer Widget */

enum CutterType { BALLNOSE = 1, SPHERICAL, TORICAL, CYLINDRICAL };

class ToolViewer : public QGLWidget
{
    Q_OBJECT
    public: 
	ToolViewer(QWidget *parent=0, const char *name=0, QGLWidget* shareWidget=0);
	~ToolViewer();	
    	void updateView();
    public:
	GLfloat ambiLight[4];
	GLfloat diffLight[4];
	GLfloat specLight[4];

	GLfloat ambiMat[4];
	GLfloat diffMat[4];
	GLfloat specMat[4];

	GLfloat specRef[4];
	GLfloat opacity;
	GLint shine;

	double nRange;
	
	double itsHolderDia;
	double itsHolderLen;
	double itsShankDia;
	double itsShankLen;
	double itsTaperDiaA;
	double itsTaperDiaB;
	double itsTaperLen;
	double itsBodyDia;
	double itsBodyLen;
	double itsBallRad;

	CutterType itsType;

	GLUquadricObj* ToolObj;


    protected:
	void initializeGL();
	void resizeGL( int width, int height );
	void paintGL();
    private:
	void DrawCutter();
};

#endif
