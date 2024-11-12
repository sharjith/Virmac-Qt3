/***************************************************************************
  virmacdoc.cpp  -  description
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
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qmsgbox.h>
#include <qfiledialog.h>


// application specific includes
#include "VirmacDoc.h"
#include "Virmac.h"
#include "VirmacView.h"

#include <Triangle.h>
#include <Point3D.h>
#include <ListIteratorOfListOfCTriangle.h>
#include <ListIteratorOfListOfCPoint3D.h>
#include <ListIteratorOfListOfCTriFacet.h>

VirmacDoc::VirmacDoc()
{
    pViewList = new QList<VirmacView>;
    pViewList->setAutoDelete(false);
    dContext = new CGLDisplayContext(this);
    m_bGCodeLoaded = false;
    m_bModelLoaded = false;
    myPath = NULL;
    myTrail = NULL;
    myCutter = new CMillCutter;
    dContext->Display(myCutter);

    m_bCheckIntersection = false;
    myStLObj = NULL;
    myPointsPerFrame = 10;
    myBlocPointList = new CListOfNCBlock;
    mySelePointList = new CListOfNCBlock;
    myTriangleList = new CListOfCTriangle;
    myFacetList = new CListOfCTriFacet;
    myBPListIter.SetList(myBlocPointList);
    mySPListIter.SetList(mySelePointList);
    DX = 0;
    DY = 0;
    DZ = 0;
    CX = 0;
    CY = 0;
    CZ = 0;
    RX = 0;
    RY = 0;
    RZ = 0;
    XP = 1;
    YP = 1;
    ZP = 1;
}

VirmacDoc::~VirmacDoc()
{
    delete pViewList;
    delete dContext;
    delete myBlocPointList;
    delete mySelePointList;
    delete myTriangleList;
    delete myFacetList;
}

void VirmacDoc::addView(VirmacView *view)
{
    pViewList->append(view);
    changedViewList();
}

void VirmacDoc::removeView(VirmacView *view)
{
    pViewList->remove(view);
    if(!pViewList->isEmpty())
	changedViewList();
    else
	deleteContents();
}

void VirmacDoc::changedViewList(){	

    VirmacView *w;
    if((int)pViewList->count() == 1){
	w=pViewList->first();
	w->setCaption(m_title);
    }
    else{
	int i;
	for( i=1,w=pViewList->first(); w!=0; i++, w=pViewList->next())
	    w->setCaption(QString(m_title+":%1").arg(i));	
    }
}

bool VirmacDoc::isLastView() {
    return ((int) pViewList->count() == 1);
}


void VirmacDoc::updateAllViews(VirmacView *sender)
{
    VirmacView *w;
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
	w->update(sender);
    }

}

void VirmacDoc::setPathName(const QString &name)
{
    m_filename=name;
    m_title=QFileInfo(name).fileName();
}

const QString& VirmacDoc::pathName() const
{
    return m_filename;
}

void VirmacDoc::setTitle(const QString &title)
{
    m_title=title;
}

const QString &VirmacDoc::title() const
{
    return m_title;
}


void VirmacDoc::closeDocument()
{
    VirmacView *w;
    if(!isLastView())
    {
	for(w=pViewList->first(); w!=0; w=pViewList->next())
	{
	    if(!w->close(true))
		break;
	}
    }
    if(isLastView())
    {
	w=pViewList->first();
	w->close(true);
    }
}

bool VirmacDoc::newDocument()
{
    /////////////////////////////////////////////////
    // TODO: Add your document initialization code here
    /////////////////////////////////////////////////
    modified=false;
    return true;
}

bool VirmacDoc::openDocument(const QString &filename, const char* /* format =0 */)
{

    QFile f( filename );
    if ( !f.open( IO_ReadOnly ) )
	return false;
    /////////////////////////////////////////////////
    // TODO: Add your document opening code here
    /////////////////////////////////////////////////
    readGCode(filename);

    f.close();

    modified=false;
    m_filename=filename;
    m_title=filename;//QFileInfo(f).fileName();
    return true;
}

bool VirmacDoc::saveDocument(const QString &filename, const char * /*format =0*/)
{
    QFile f( filename );
    if ( !f.open( IO_WriteOnly ) )
	return false;

    /////////////////////////////////////////////////
    // TODO: Add your document saving code here
    /////////////////////////////////////////////////

    f.close();

    modified=false;
    m_filename=filename;
    m_title=QFileInfo(f).fileName();
    return true;
}

void VirmacDoc::deleteContents()
{
    /////////////////////////////////////////////////
    // TODO: Add implementation to delete the document contents
    /////////////////////////////////////////////////

}

bool VirmacDoc::canCloseFrame(VirmacView* pFrame)
{
    if(!isLastView())
	return true;

    bool ret=false;
    if(isModified())
    {
	QString saveName;
	switch(QMessageBox::information(pFrame, title(), tr("The current file has been modified.\n"
			"Do you want to save it?"),QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel ))
	{
	    case QMessageBox::Yes:
		if(title().contains(tr("Untitled")))
		{
		    saveName=QFileDialog::getSaveFileName(0, 0, pFrame);
		    if(saveName.isEmpty())
			return false;
		}
		else
		    saveName=pathName();

		if(!saveDocument(saveName))
		{
		    switch(QMessageBox::critical(pFrame, tr("I/O Error !"), tr("Could not save the current document !\n"
				    "Close anyway ?"),QMessageBox::Yes ,QMessageBox::No))

		    {
			case QMessageBox::Yes:
			    ret=true;
			case QMessageBox::No:
			    ret=false;
		    }
		}
		else
		    ret=true;
		break;
	    case QMessageBox::No:
		ret=true;
		break;
	    case QMessageBox::Cancel:
	    default:
		ret=false;
		break;
	}
    }
    else
	ret=true;

    return ret;
}

bool VirmacDoc::readGCode(const QString& fName)
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    if (!fName.isEmpty())
    {
	if(!myPath)
	{
	    myPath = new CToolPath(fName);
	    if(myPath->IsDone())
	    {
		dContext->Display(myPath);
		myPathIter.SetList(myPath->myGCodeList);
		myPathIter.Init();
		m_bGCodeLoaded = true;
		myTrail = new CToolTrail();
		dContext->Display(myTrail);
		HidePath(false);
		modified=false;
		m_filename=fName;
		m_title=fName;
		if(!myCutter)
		{
		    myCutter = new CMillCutter; 
		    dContext->Display(myCutter);		    
		}
	    }
	    else
	    {
		((VirmacApp*)qApp)->slotEditGCode();
		myPath = NULL;
	    }
	}
	else
	{
	    myPath->SetFile(fName);
	    myTrail->Nullify();
	    if(myPath->IsDone())
	    {
		myPathIter.SetList(myPath->myGCodeList);
		myPathIter.Init();
		m_bGCodeLoaded = true;
		dContext->RefreshVolume();
	    }
	    modified=false;
	    m_filename=fName;
	    m_title=fName;
	    HidePath(false);
	}
    }
    QApplication::restoreOverrideCursor();
    return m_bGCodeLoaded;
}

#include "StLObject.h"
#include "StLReader.h"
bool VirmacDoc::readStLFile(const QString& fName)
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    if (!fName.isEmpty())
    {
	CStLReader* m_Reader = new CStLReader(fName.ascii());
	if(m_Reader->IsFileValid())
	{
	    if(!myStLObj)
	    {
		myStLObj = new CStLObject(m_Reader);
		dContext->Display(myStLObj);
		myStLObj->SetMaterial(GOLD);
		m_bModelLoaded = true;
	    }
	    else
	    {
		myStLObj->SetReader(m_Reader);
		myStLObj->SetMaterial(GOLD);
		m_bModelLoaded = true;
		dContext->RefreshVolume();
	    }	    
	}
	delete m_Reader;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

int VirmacDoc::AnimFunction()
{
    CNCBlock aBloc;
    for(int pnt = 0; pnt < myPointsPerFrame; pnt++)
    {
	if(myPathIter.More())
	{	    
	    aBloc = myPathIter.Current();
	    myPathIter.Next();
	    CPoint3D P(aBloc.X, aBloc.Y, aBloc.Z);
	    myCutter->MoveTo(P);
	    myCutter->Rotate(aBloc.A, aBloc.B, aBloc.C);
	    myTrail->AddBlock(aBloc);
	    blocString.sprintf("N%ld  G%.2d  X%0.3lf  Y%0.3lf  Z%0.3lf  A%0.3lf  B%0.3lf  C%0.3lf",
		    aBloc.N,aBloc.G,aBloc.X,aBloc.Y,aBloc.Z,aBloc.A,aBloc.B, aBloc.C);
	    if(m_bCheckIntersection && m_bGCodeLoaded && m_bModelLoaded)//check intersection
	    {
		// TODO
		// 1) Make AABB of cutter at current block
		// 2) Sort model triangles lying in AABB
		// and store in model triangle list
	    }
	    //end intersection
	}
    }
    if(myPathIter.More())
	return 1;
    else
	return -1;
}

//helpers
void VirmacDoc::ShowPoints(const bool& flag)
{
    if(flag)
    {
	if(!myBlocPointList->IsEmpty())
	    myBlocPointList->Clear();
	if(!mySelePointList->IsEmpty())
	    mySelePointList->Clear();
	for(myPathIter.Init(); myPathIter.More(); myPathIter.Next())
	{
	    CNCBlock aBloc = myPathIter.Current();
	    myBlocPointList->Append(aBloc);
	}

    }
    else
	myBlocPointList->Clear();
}

void VirmacDoc::ShowSelectedBloc(CNCBlock& bloc)
{
    if(!mySelePointList->IsEmpty())
	mySelePointList->Clear();
    mySelePointList->Append(bloc);
}

void VirmacDoc::EraseSelectedBloc()
{
    if(!mySelePointList->IsEmpty())
	mySelePointList->Clear();
}
