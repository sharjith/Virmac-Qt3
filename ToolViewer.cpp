
#include "ToolViewer.h"

ToolViewer::ToolViewer(QWidget *parent, const char *name, QGLWidget* shareWidget ) : QGLWidget(parent,name, shareWidget)
{
    makeCurrent();
    ToolObj = gluNewQuadric();
    itsType = SPHERICAL;
    itsHolderDia = 65;
    itsHolderLen = 100;
    itsShankDia = 32;
    itsShankLen = 20;
    itsTaperDiaA = 20;
    itsTaperDiaB = 32;
    itsTaperLen = 70;
    itsBodyDia = 20;
    itsBodyLen = 20;
    itsBallRad = 12.5;

    nRange = itsBallRad + itsBodyLen + itsTaperLen + itsShankLen + itsHolderLen + 150;

    opacity = 1.0f;
    ambiLight[0] = 0.3;
    ambiLight[1] = 0.3;
    ambiLight[2] = 0.3;
    ambiLight[3] = 1.0;

    diffLight[0] = 0.7f;
    diffLight[1] = 0.7f;
    diffLight[2] = 0.7f;
    diffLight[3] = 1.0f;

    specLight[0] = 1.0f;
    specLight[1] = 1.0f;
    specLight[2] = 1.0f;
    specLight[3] = 1.0f;

    ambiMat[0] = 0.7;
    ambiMat[1] = 0.7;
    ambiMat[2] = 0.7;
    ambiMat[3] = opacity;

    diffMat[0] = 0.7f;
    diffMat[1] = 0.7f;
    diffMat[2] = 0.7f;
    diffMat[3] = opacity;

    specMat[0] = 1.0f;
    specMat[1] = 1.0f;
    specMat[2] = 1.0f;
    specMat[3] = opacity;

    specRef[0] =  0.250f;
    specRef[1] =  0.250f;
    specRef[2] =  0.250f;
    specRef[3] =  1.0f;

    shine = 5; 
}

ToolViewer::~ToolViewer()
{
    gluDeleteQuadric(ToolObj);
}

void ToolViewer::updateView()
{
    updateGL();
}

void ToolViewer::initializeGL ()
{
    // Set up the rendering context, define display lists etc.:
    /* Insert your OpenGL initialization code here */
    // Set background color
    qglClearColor(QColor("black"));

    // Set drawing color to
    glColor3ub(170, 170, 255);
}

void ToolViewer::resizeGL ( int w, int h )
{
    // setup viewport, projection etc.:
    /* Prevent a divide by zero*/
    if(h == 0)
	h = 1;

    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w <= h)
	glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -20000.0f, 20000.0f);
    else
	glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -20000.0f, 20000.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

}

void ToolViewer::paintGL ()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);
    // Enable lighting
    glEnable(GL_LIGHTING);

    GLfloat position[] = {0.0, 0.0, 3.0, 0.0};
    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat local_view[] = {0.0};

    // Setup and enable light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);

    glDisable(GL_COLOR_MATERIAL);
    //Setup Material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specMat);


    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specRef);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS, shine);

    glShadeModel(GL_SMOOTH);   

    glPushMatrix();

    double totLen  = itsBallRad + itsBodyLen + itsTaperLen + itsShankLen + itsHolderLen + 120;
    double fact = (totLen/485.0) * 100;
    fact = 100.0 - fact;
    fact = fact/100.0;
    fact += 1;
    fact *= 2;
    glScalef(fact, fact, fact);
    glTranslatef(0,-totLen/2, 0);
    glRotatef(-90, 1, 0, 0);

    DrawCutter();

    glPopMatrix();
}

void ToolViewer::DrawCutter()
{
    int parts = 15;
    //******************************Ball End Tool***********************************
    if(itsType == BALLNOSE)
    {
	glPushMatrix();

	//Ball
	glTranslatef(0,0,itsBallRad);
	gluSphere(ToolObj,itsBallRad,parts,parts);

	//Body
	gluCylinder(ToolObj,itsBallRad,itsBallRad,itsBodyLen,parts,parts); 

	//Taper
	glTranslatef(0,0,itsBodyLen);
	gluCylinder(ToolObj,itsTaperDiaA/2,itsTaperDiaB/2,itsTaperLen,parts,parts);

	//Shank
	glTranslatef(0,0,itsTaperLen);
	gluCylinder(ToolObj,itsShankDia/2,itsShankDia/2,itsShankLen,parts,parts);

	//holder
	glTranslatef(0,0,itsShankLen);
	gluDisk(ToolObj,0,itsHolderDia/2,parts,parts);//Cover Top
	gluCylinder(ToolObj,itsHolderDia/2,itsHolderDia/2, itsHolderLen,parts,parts);

	//holder step;
	glTranslatef(0,0, itsHolderLen);
	gluDisk(ToolObj,0,50,parts,parts);//Cover Top
	gluCylinder(ToolObj, 50, 50, 20, parts, parts);

	//holder taper
	glTranslatef(0,0, 20);
	gluCylinder(ToolObj,35,15,100,parts,parts);
	glPopMatrix();
	// End Holder
    }
    //******************************* Flat End Tool**********************************
    else if(itsType == SPHERICAL)			
    {
	glPushMatrix();

	//Ball
	glTranslatef(0,0,itsBallRad);
	gluSphere(ToolObj,itsBallRad,parts,parts);

	//Body
	//gluCylinder(ToolObj,itsBallRad,itsBallRad,itsBodyLen,parts,parts); 

	//Taper
	glTranslatef(0,0,itsBodyLen-itsBallRad);
	gluCylinder(ToolObj,itsTaperDiaA/2,itsTaperDiaB/2,itsTaperLen,parts,parts);

	//Shank
	glTranslatef(0,0,itsTaperLen);
	gluCylinder(ToolObj,itsShankDia/2,itsShankDia/2,itsShankLen,parts,parts);

	//holder
	glTranslatef(0,0,itsShankLen);
	gluDisk(ToolObj,0,itsHolderDia/2,parts,parts);//Cover Top
	gluCylinder(ToolObj,itsHolderDia/2,itsHolderDia/2, itsHolderLen,parts,parts);

	//holder step;
	glTranslatef(0,0, itsHolderLen);
	gluDisk(ToolObj,0,50,parts,parts);//Cover Top
	gluCylinder(ToolObj, 50, 50, 20, parts, parts);

	//holder taper
	glTranslatef(0,0, 20);
	gluCylinder(ToolObj,35,15,100,parts,parts);
	glPopMatrix();
	// End Holder
    }	
    //******************************Toroidal Tool***********************************
    else if(itsType == TORICAL)
    {
	glPushMatrix();

	//Ball
	glTranslatef(0,0,itsBallRad);
	//gluSphere(ToolObj,itsBallRad,parts,parts);
	GLfloat iRad, oRad;
	oRad = ((itsBodyDia-2*itsBallRad)/2);
	iRad = itsBallRad;
	//glutSolidTorus(iRad,oRad,parts,parts);
	drawTorus(parts, parts, oRad, iRad);

	//Body
	gluCylinder(ToolObj,itsBodyDia/2,itsBodyDia/2,itsBodyLen,parts,parts); 

	//Taper
	glTranslatef(0,0,itsBodyLen);
	gluCylinder(ToolObj,itsTaperDiaA/2,itsTaperDiaB/2,itsTaperLen,parts,parts);

	//Shank
	glTranslatef(0,0,itsTaperLen);
	gluCylinder(ToolObj,itsShankDia/2,itsShankDia/2,itsShankLen,parts,parts);

	//holder
	glTranslatef(0,0,itsShankLen);
	gluDisk(ToolObj,0,itsHolderDia/2,parts,parts);//Cover Top
	gluCylinder(ToolObj,itsHolderDia/2,itsHolderDia/2, itsHolderLen,parts,parts);

	//holder step;
	glTranslatef(0,0, itsHolderLen);
	gluDisk(ToolObj,0,50,parts,parts);//Cover Top
	gluCylinder(ToolObj, 50, 50, 20, parts, parts);

	//holder taper
	glTranslatef(0,0, 20);
	gluCylinder(ToolObj,35,15,100,parts,parts);
	glPopMatrix();
	// End Holder
    }
    else if(itsType == CYLINDRICAL)
    {
	glPushMatrix();

	//Body
	gluCylinder(ToolObj,itsBodyDia/2,itsBodyDia/2,itsBodyLen,parts,parts); 

	//Taper
	glTranslatef(0,0,itsBodyLen);
	gluCylinder(ToolObj,itsTaperDiaA/2,itsTaperDiaB/2,itsTaperLen,parts,parts);

	//Shank
	glTranslatef(0,0,itsTaperLen);
	gluCylinder(ToolObj,itsShankDia/2,itsShankDia/2,itsShankLen,parts,parts);

	//holder
	glTranslatef(0,0,itsShankLen);
	gluDisk(ToolObj,0,itsHolderDia/2,parts,parts);//Cover Top
	gluCylinder(ToolObj,itsHolderDia/2,itsHolderDia/2, itsHolderLen,parts,parts);

	//holder step;
	glTranslatef(0,0, itsHolderLen);
	gluDisk(ToolObj,0,50,parts,parts);//Cover Top
	gluCylinder(ToolObj, 50, 50, 20, parts, parts);

	//holder taper
	glTranslatef(0,0, 20);
	gluCylinder(ToolObj,35,15,100,parts,parts);
	glPopMatrix();
	// End Holder

    }    
}

