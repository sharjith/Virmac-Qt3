/***************************************************************************
  virmac.cpp  -  description
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

// Qt includes
#include <qapplication.h>
#include <qvbox.h>
#include <qaccel.h>
#include <qlayout.h>
#include <qdesktopwidget.h>
#include <qrect.h>
#include <qpoint.h>
#include <qmessagebox.h>
#include <qtoolbutton.h>
#include <qcolordialog.h>

// application specific includes
#include "VirmacView.h"
#include "VirmacDoc.h"
#include "Virmac.h"
#include "MDIWindow.h"

#include "images/filenew.xpm"
#include "images/fileopen.xpm"
#include "images/filegcode.xpm"
#include "images/filewritegcode.xpm"
#include "images/filestl.xpm"

#include "images/appicon.xpm"
#include "images/appicontr.xpm"
#include "images/top.xpm"
#include "images/bottom.xpm"
#include "images/front.xpm"
#include "images/back.xpm"
#include "images/left.xpm"
#include "images/right.xpm"
#include "images/axo.xpm"
#include "images/multi.xpm"
#include "images/ortho.xpm"
#include "images/persp.xpm"
#include "images/resetview.xpm"
#include "images/fitall.xpm"
#include "images/zoomwin.xpm"
#include "images/antial.xpm"
#include "images/snapshot.xpm"

#include "images/play.xpm"
#include "images/playstep.xpm"
#include "images/pickplay.xpm"
#include "images/rewind.xpm"
#include "images/pause.xpm"
#include "images/stop.xpm"

#include "images/trimafter.xpm"
#include "images/trimbefore.xpm"
#include "images/pickbloc.xpm"
#include "images/searchbloc.xpm"
#include "images/measdist.xpm"
#include "images/measang.xpm"
#include "images/edit.xpm"

#include "images/zoomwincursor.xpm"

MyWorkspace::MyWorkspace( QWidget * parent, const char * name ) :
    QWorkspace(parent, name)
{
}

QWidget* MyWorkspace::activeWindow() const
{
    return ((MDIWindow*) QWorkspace::activeWindow())->centralWidget();
}


VirmacApp::VirmacApp()
{

    setCaption(tr("Virmac " VERSION) );
    QPixmap appIcon = QPixmap(appicon);
    setIcon(appIcon);

    printer = new QPrinter;
    untitledCount=0;
    pDocList = new QList<VirmacDoc>();
    pDocList->setAutoDelete(true);

    mesgCnt = 0;
    promCnt = 0;

    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts

    initView();
    initActions();
    initMenuBar();
    initToolBar();
    initStatusBar();

    QDesktopWidget *d = QApplication::desktop();
    int w = d->width();     // returns desktop width
    int h = d->height();    // returns desktop height
    resize( w, h );

    bSplit = false;

    hb = new QHBox(statusBar());
    hb->setSpacing(2);
    progBar = new QProgressBar(hb);
    cViewer = new QLabel(hb);
    cViewer->setMaximumWidth(250);
    cViewer->setMinimumWidth(250);
    cViewer->setScaledContents(true);
    cViewer->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    statusBar()->addWidget(hb,0,TRUE);

    viewToolBar->setOn(true);
    viewStatusBar->setOn(true);
    viewMesgBar->setOn(true);
    viewPromBar->setOn(true);
}

VirmacApp::~VirmacApp()
{
    delete printer;
}

void VirmacApp::initActions()
{
    QPixmap openIcon, gcodeIcon, gcodeWriteIcon, stlIcon, newIcon;
    newIcon = QPixmap(filenew);
    openIcon = QPixmap(fileopen);
    gcodeIcon = QPixmap(filegcode);
    gcodeWriteIcon = QPixmap(filewritegcode);
    stlIcon = QPixmap(filestl);

    //viewer toolbar
    QPixmap topIcon, bottomIcon, frontIcon, backIcon, leftIcon, rightIcon, axoIcon, multiIcon, orthoIcon, resetIcon, fitallIcon, zoomwinIcon, antialIcon, snapshotIcon;
    topIcon = QPixmap(top);
    bottomIcon = QPixmap(bottom);
    frontIcon = QPixmap(front);
    backIcon = QPixmap(back);
    leftIcon = QPixmap(leftView);
    rightIcon = QPixmap(rightView);
    axoIcon = QPixmap(axo);
    multiIcon = QPixmap(multi);
    orthoIcon = QPixmap(ortho);
    resetIcon = QPixmap(resetview);
    fitallIcon = QPixmap(fitall);
    zoomwinIcon = QPixmap(zoomwin);
    antialIcon = QPixmap(antial);
    snapshotIcon = QPixmap(snapshot_xpm);

    //anim toolbar
    QPixmap playIcon, playstepIcon, pickplayIcon, rewindIcon, pauseIcon, stopIcon;
    playIcon = QPixmap(play);
    playstepIcon = QPixmap(playstep);
    pickplayIcon = QPixmap(pickplay);
    rewindIcon = QPixmap(rewindxpm);
    pauseIcon = QPixmap(pause_xpm);
    stopIcon = QPixmap(stop);

    //util
    QPixmap trimBefIcon, trimAftIcon, pickBlocIcon, searchBlocIcon, measDistIcon, measAngIcon, editIcon;
    trimBefIcon = QPixmap(trimbefore);
    trimAftIcon = QPixmap(trimafter);
    pickBlocIcon = QPixmap(pickbloc);
    searchBlocIcon = QPixmap(searchbloc);
    measDistIcon = QPixmap(measdist);
    measAngIcon = QPixmap(measang);
    editIcon = QPixmap(editicon);

    fileNew = new QAction(tr("New File"), newIcon, tr("&New"), QAccel::stringToKey(tr("Ctrl+N")), this);
    fileNew->setStatusTip(tr("Creates a new document"));
    fileNew->setWhatsThis(tr("New File\n\nCreates a new document"));
    connect(fileNew, SIGNAL(activated()), this, SLOT(slotFileNew()));

    /*fileOpen = new QAction(tr("Open File"), openIcon, tr("&Open..."), 0, this);
      fileOpen->setStatusTip(tr("Opens an existing document"));
      fileOpen->setWhatsThis(tr("Open File\n\nOpens an existing document"));
      connect(fileOpen, SIGNAL(activated()), this, SLOT(slotFileOpen()));*/

    fileReadGCode = new QAction(tr("Read G Code File"), gcodeIcon, tr("&Read G Code"), QAccel::stringToKey(tr("Ctrl+Shift+R")), this);
    fileReadGCode->setStatusTip(tr("Read G Code File"));
    fileReadGCode->setWhatsThis(tr("Read G Code File.\n\nReads The G Code File To Simulate"));
    connect(fileReadGCode, SIGNAL(activated()), this, SLOT(slotFileReadGCode()));

    fileWriteGCode = new QAction(tr("Write G Code File"), gcodeWriteIcon, tr("&Write G Code"), QAccel::stringToKey(tr("Ctrl+Shift+W")), this);
    fileWriteGCode->setStatusTip(tr("Write G Code File"));
    fileWriteGCode->setWhatsThis(tr("Write G Code File.\n\nWrite The G Code File To Disk"));
    connect(fileWriteGCode, SIGNAL(activated()), this, SLOT(slotFileWriteGCode()));

    fileReadStl = new QAction(tr("Read Stl File"), stlIcon, tr("Read &StL..."), QAccel::stringToKey(tr("Ctrl+Shift+S")), this);
    fileReadStl->setStatusTip(tr("Read Stl File"));
    fileReadStl->setWhatsThis(tr("Read Stl File\n\nRead Stl File"));
    connect(fileReadStl, SIGNAL(activated()), this, SLOT(slotFileReadStL()));

    fileClose = new QAction(tr("Close File"), tr("&Close"), QAccel::stringToKey(tr("Ctrl+W")), this);
    fileClose->setStatusTip(tr("Closes the actual document"));
    fileClose->setWhatsThis(tr("Close File\n\nCloses the actual document"));
    connect(fileClose, SIGNAL(activated()), this, SLOT(slotFileClose()));

    fileQuit = new QAction(tr("Exit"), tr("E&xit"), QAccel::stringToKey(tr("Ctrl+Q")), this);
    fileQuit->setStatusTip(tr("Quits the application"));
    fileQuit->setWhatsThis(tr("Exit\n\nQuits the application"));
    connect(fileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

    editTrimBefore = new QAction(tr("Trim Before"), trimBefIcon, tr("Trim &Before"), QAccel::stringToKey(tr("Shift+B")), this);
    editTrimBefore->setStatusTip(tr("Trims The G Code Before The Selected Bloc"));
    editTrimBefore->setWhatsThis(tr("Trim Before\n\nTrim The G Code Before The Selected Bloc"));
    connect(editTrimBefore, SIGNAL(activated()), this, SLOT(slotEditTrimBefore()));

    editTrimAfter = new QAction(tr("Trim After"), trimAftIcon, tr("Trim &After"), QAccel::stringToKey(tr("Shift+A")), this);
    editTrimAfter->setStatusTip(tr("Trims The G Code Before The After Bloc"));
    editTrimAfter->setWhatsThis(tr("Trim Before\n\nTrim The G Code After The Selected Bloc"));
    connect(editTrimAfter, SIGNAL(activated()), this, SLOT(slotEditTrimAfter()));

    editGCode = new QAction(tr("Edit GCode"), editIcon, tr("Edit &GCode"), QAccel::stringToKey(tr("Ctrl+E")), this);
    editGCode->setStatusTip(tr("Edit G Code File"));
    editGCode->setWhatsThis(tr("Edit G Code\n\nOpens G Code File For Editing"));
    connect(editGCode, SIGNAL(activated()), this, SLOT(slotEditGCode()));

    viewToolBar = new QAction(tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
    viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
    viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
    connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

    viewStatusBar = new QAction(tr("Statusbar"), tr("&Statusbar"), 0, this, 0, true);
    viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
    viewStatusBar->setWhatsThis(tr("Statusbar\n\nEnables/disables the statusbar"));
    connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

    viewMesgBar = new QAction(tr("Messagebar"), tr("&Messagebar"), 0, this, 0, true);
    viewMesgBar->setStatusTip(tr("Enables/disables the messagebar"));
    viewMesgBar->setWhatsThis(tr("Messagebar\n\nEnables/disables the messagebar"));
    connect(viewMesgBar, SIGNAL(toggled(bool)), this, SLOT(slotViewMesgBar(bool)));

    viewPromBar = new QAction(tr("Promptbar"), tr("&Promptbar"), 0, this, 0, true);
    viewPromBar->setStatusTip(tr("Enables/disables the promptbar"));
    viewPromBar->setWhatsThis(tr("Promptbar\n\nEnables/disables the promptbar"));
    connect(viewPromBar, SIGNAL(toggled(bool)), this, SLOT(slotViewPromBar(bool)));

    analysisPPInter = new QAction(tr("Check Point To Point Intersection"), tr("&Intersection"), 0, this, 0, true);
    analysisPPInter->setStatusTip(tr("Point To Point Tool Intersection Analysis"));
    analysisPPInter->setWhatsThis(tr("Intersection\n\nPoint To Point Tool Intersection Analysis"));
    connect(analysisPPInter, SIGNAL(toggled(bool)), this, SLOT(slotPPIntersection(bool)));

    analysisRoughStock = new QAction(tr("Rough Stock"), tr("&Rough Stock"), 0, this, 0, false);
    analysisRoughStock->setStatusTip(tr("Rough Stock For WYSIWYC Analysis"));
    analysisRoughStock->setWhatsThis(tr("Rough Stock\n\nRough Stock For WYSIWYC Analysis"));
    connect(analysisRoughStock, SIGNAL(activated()), this, SLOT(slotRoughStock()));

    analysisWYSIWYC = new QAction(tr("What-You-See-Is-What-You-Cut"), tr("&WYSIWYC"), 0, this, 0, false);
    analysisWYSIWYC->setStatusTip(tr("What-You-See-Is-What-You-Cut Analysis"));
    analysisWYSIWYC->setWhatsThis(tr("WYSIWYC\n\nWhat-You-See-Is-What-You-Cut Analysis"));
    connect(analysisWYSIWYC, SIGNAL(activated()), this, SLOT(slotWYSIWYC()));

    windowNewWindow = new QAction(tr("New Window"), tr("&New Window"), 0, this);
    windowNewWindow->setStatusTip(tr("Opens a new view for the current document"));
    windowNewWindow->setWhatsThis(tr("New Window\n\nOpens a new view for the current document"));
    connect(windowNewWindow, SIGNAL(activated()), this, SLOT(slotWindowNewWindow()));

    windowCascade = new QAction(tr("Cascade"), tr("&Cascade"), 0, this);
    windowCascade->setStatusTip(tr("Cascades all windows"));
    windowCascade->setWhatsThis(tr("Cascade\n\nCascades all windows"));
    connect(windowCascade, SIGNAL(activated()), pWorkspace, SLOT(cascade()));

    windowTile = new QAction(tr("Tile"), tr("&Tile"), 0, this);
    windowTile->setStatusTip(tr("Tiles all windows"));
    windowTile->setWhatsThis(tr("Tile\n\nTiles all windows"));
    connect(windowTile, SIGNAL(activated()), pWorkspace, SLOT(tile()));


    windowAction = new QActionGroup(this, 0, false);
    windowAction->insert(windowNewWindow);
    windowAction->insert(windowCascade);
    windowAction->insert(windowTile);

    helpAboutApp = new QAction(tr("About"), tr("&About..."), 0, this);
    helpAboutApp->setStatusTip(tr("About the application"));
    helpAboutApp->setWhatsThis(tr("About\n\nAbout the application"));
    connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

    //viewer actions
    viewTop = new QAction(tr("Top View"), topIcon, tr("Top"), QAccel::stringToKey(tr("Ctrl+T")), this);
    viewTop->setStatusTip(tr("Displays The Top View"));
    viewTop->setWhatsThis(tr("Top View\n\nDisplays The Top View"));
    connect(viewTop, SIGNAL(activated()), this, SLOT(slotTopView()));

    viewBottom = new QAction(tr("Bottom View"), bottomIcon, tr("Bottom"), QAccel::stringToKey(tr("Ctrl+B")), this);
    viewBottom->setStatusTip(tr("Displays The Bottom View"));
    viewBottom->setWhatsThis(tr("Bottom View\n\nDisplays The Bottom View"));
    connect(viewBottom, SIGNAL(activated()), this, SLOT(slotBottomView()));

    viewFront = new QAction(tr("Front View"), frontIcon, tr("Front"), QAccel::stringToKey(tr("Ctrl+F")), this);
    viewFront->setStatusTip(tr("Displays The Front View"));
    viewFront->setWhatsThis(tr("Front View\n\nDisplays The Front View"));
    connect(viewFront, SIGNAL(activated()), this, SLOT(slotFrontView()));

    viewBack = new QAction(tr("Back View"), backIcon, tr("Back"), QAccel::stringToKey(tr("Ctrl+Shift+B")), this);
    viewBack->setStatusTip(tr("Displays The Back View"));
    viewBack->setWhatsThis(tr("Back View\n\nDisplays The Back View"));
    connect(viewBack, SIGNAL(activated()), this, SLOT(slotBackView()));

    viewLeft = new QAction(tr("Left View"), leftIcon, tr("Left"), QAccel::stringToKey(tr("Ctrl+L")), this);
    viewLeft->setStatusTip(tr("Displays The Left View"));
    viewLeft->setWhatsThis(tr("Left View\n\nDisplays The Left View"));
    connect(viewLeft, SIGNAL(activated()), this, SLOT(slotLeftView()));

    viewRight = new QAction(tr("Right View"), rightIcon, tr("Right"), QAccel::stringToKey(tr("Ctrl+R")), this);
    viewRight->setStatusTip(tr("Displays The Right View"));
    viewRight->setWhatsThis(tr("Right View\n\nDisplays The Right View"));
    connect(viewRight, SIGNAL(activated()), this, SLOT(slotRightView()));

    viewAxo = new QAction(tr("Axo View"), axoIcon, tr("Axo"), QAccel::stringToKey(tr("Ctrl+A")), this);
    viewAxo->setStatusTip(tr("Displays The Axonometric View"));
    viewAxo->setWhatsThis(tr("Axo View\n\nDisplays The Axonometric View"));
    connect(viewAxo, SIGNAL(activated()), this, SLOT(slotAxoView()));

    viewMultiFour = new QAction(tr("Multiple Four Views"), multiIcon, tr("MultiFour"), QAccel::stringToKey(tr("Ctrl+4")), this);
    viewMultiFour->setStatusTip(tr("Displays The Multiple Four Views"));
    viewMultiFour->setWhatsThis(tr("Multi Four View\n\nDisplays The Multiple Four Views"));
    connect(viewMultiFour, SIGNAL(activated()), this, SLOT(slotMultiFour()));

    viewProjection = new QAction(tr("Projection"), orthoIcon, tr("Perspective"), 0, this, 0);
    viewProjection->setStatusTip(tr("Toggles View Projection"));
    viewProjection->setWhatsThis(tr("Projection\n\nToggles View Projection Between Orthographic And Perspective"));
    connect(viewProjection, SIGNAL(activated()), this, SLOT(slotProjection()));

    viewFitall = new QAction(tr("Fit All"), fitallIcon, tr("Fit All"), QAccel::stringToKey(tr("F2")), this);
    viewFitall->setStatusTip(tr("Fits The View To Screen"));
    viewFitall->setWhatsThis(tr("Fit All\n\nFits The View To Screen"));
    connect(viewFitall, SIGNAL(activated()), this, SLOT(slotFitall()));

    viewZoomwin = new QAction(tr("Zoom Win"), zoomwinIcon, tr("Zoom Win"), QAccel::stringToKey(tr("Z")), this);
    viewZoomwin->setStatusTip(tr("Zooms The View Defined By A Window"));
    viewZoomwin->setWhatsThis(tr("Zoom Win\n\nZooms The View Defined By A Window"));
    connect(viewZoomwin, SIGNAL(activated()), this, SLOT(slotZoomwin()));

    viewReset = new QAction(tr("Reset View"), resetIcon, tr("Axo"), 0, this);
    viewReset->setStatusTip(tr("Resets The View"));
    viewReset->setWhatsThis(tr("Reset View\n\nResets The View"));
    connect(viewReset, SIGNAL(activated()), this, SLOT(slotReset()));

    viewAntial = new QAction(tr("Antialias View"), antialIcon, tr("Antialias"), 0, this, 0, false);
    viewAntial->setStatusTip(tr("Toggle Antialias"));
    viewAntial->setWhatsThis(tr("Toggle Antialias\n\nToggles Antialiasing On And Off"));
    connect(viewAntial, SIGNAL(activated()), this, SLOT(slotAntial()));

    viewSnapshot = new QAction(tr("Snapshot"), snapshotIcon, tr("Snapshot"), 0, this, 0, false);
    viewSnapshot->setStatusTip(tr("Take A Snapshot"));
    viewSnapshot->setWhatsThis(tr("Take A Snapshot\n\nTakes A Snapshot Of Current Screen"));
    connect(viewSnapshot, SIGNAL(activated()), this, SLOT(slotSnapshot()));

    bgColor = new QAction(tr("Background Color"), tr("Background Color"), 0, this);
    bgColor->setStatusTip(tr("Sets The Background Color"));
    bgColor->setWhatsThis(tr("Background Color\n\nChange The Background Color"));
    connect(bgColor, SIGNAL(activated()), this, SLOT(slotBgColor()));

    //animation
    animPlay = new QAction(tr("Play Simulation"), playIcon, tr("Play"), QAccel::stringToKey(tr("Ctrl+P")), this );
    animPlay->setStatusTip(tr("Play Simulation"));
    animPlay->setWhatsThis(tr("Play Simulation\n\nStart Playing The Animation"));
    connect(animPlay, SIGNAL(activated()), this, SLOT(slotPlay()));

    animPlayStep = new QAction(tr("Play Step"), playstepIcon, tr("Play Step"), QAccel::stringToKey(tr("F5")), this );
    animPlayStep->setStatusTip(tr("Play Step"));
    animPlayStep->setWhatsThis(tr("Play Simulation Step\n\nStart Playing The Animation Step By Step"));
    connect(animPlayStep, SIGNAL(activated()), this, SLOT(slotPlayStep()));

    animPickPlay = new QAction(tr("Pick Play Simulation"), pickplayIcon, tr("Pick And Play"), QAccel::stringToKey(tr("Shift+Ctrl+P")), this );
    animPickPlay->setStatusTip(tr("Pick Play Simulation"));
    animPickPlay->setWhatsThis(tr("Pick Play Simulation\n\nStart Playing The Animation From Picked Block"));
    connect(animPickPlay, SIGNAL(activated()), this, SLOT(slotPickPlay()));

    animRewind = new QAction(tr("Rewind Simulation"), rewindIcon, tr("Rewind"), QAccel::stringToKey(tr("Alt+W")), this );
    animRewind->setStatusTip(tr("Rewind Simulation"));
    animRewind->setWhatsThis(tr("Rewind Simulation\n\nRewind The Animation"));
    connect(animRewind, SIGNAL(activated()), this, SLOT(slotRewind()));

    animPause = new QAction(tr("Pause Simulation"), pauseIcon, tr("Pause"), QAccel::stringToKey(tr("Alt+P")), this );
    animPause->setStatusTip(tr("Pause Simulation"));
    animPause->setWhatsThis(tr("Pause Simulation\n\nPause The Animation"));
    connect(animPause, SIGNAL(activated()), this, SLOT(slotPause()));

    animStop = new QAction(tr("Stop Simulation"), stopIcon, tr("Stop"), QAccel::stringToKey(tr("Alt+S")), this );
    animStop->setStatusTip(tr("Stop Simulation"));
    animStop->setWhatsThis(tr("Stop Simulation\n\nStop Playing The Animation"));
    connect(animStop, SIGNAL(activated()), this, SLOT(slotStop()));

    deleteSirus = new QAction(tr("Delete Sirus"), tr("Delete Sirus"), QAccel::stringToKey(tr("Ctrl+D")), this );
    deleteSirus->setStatusTip(tr("Delete Sirus"));
    deleteSirus->setWhatsThis(tr("Delete Sirus\n\nDelete The G Code Sirus"));
    connect(deleteSirus, SIGNAL(activated()), this, SLOT(slotDeleteSirus()));

    deleteModel = new QAction(tr("Delete Model"), tr("Delete Model"), QAccel::stringToKey(tr("Ctrl+M")), this );
    deleteModel->setStatusTip(tr("Delete Model"));
    deleteModel->setWhatsThis(tr("Delete Model\n\nDelete The StL Model"));
    connect(deleteModel, SIGNAL(activated()), this, SLOT(slotDeleteModel()));

    simulSettings = new QAction(tr("Settings"), tr("Settings"), 0, this );
    simulSettings->setStatusTip(tr("Simulation Settings"));
    simulSettings->setWhatsThis(tr("Simulation Settings\n\nSet The Simulation Parameters"));
    connect(simulSettings, SIGNAL(activated()), this, SLOT(slotSimulSettings()));

    toolLibrary = new QAction(tr("Tool Library"), tr("Tool Library"), QAccel::stringToKey(tr("Shift+T")), this );
    toolLibrary->setStatusTip(tr("Tool Library Settings"));
    toolLibrary->setWhatsThis(tr("toolLibrary Settings\n\nCall The Tool Library"));
    connect(toolLibrary, SIGNAL(activated()), this, SLOT(slotToolLibrary()));

    rotaryAxis = new QAction(tr("Rotary Axis"), tr("Rotary Axis"), 0, this );
    rotaryAxis->setStatusTip(tr("Rotary Axis Settings"));
    rotaryAxis->setWhatsThis(tr("Rotary Axis\n\nRotary Axis Settings For 5 Axis"));
    connect(rotaryAxis, SIGNAL(activated()), this, SLOT(slotRotaryAxis()));

    startBlock = new QAction(tr("Start Block"), tr("Start Block"), 0, this );
    startBlock->setStatusTip(tr("Start Block Settings"));
    startBlock->setWhatsThis(tr("Start Block\n\nSet The Start Block For The Simulation"));
    connect(startBlock, SIGNAL(activated()), this, SLOT(slotStartBlock()));

    pToolDisplayMode = new QAction(tr("Tool Display Mode"), tr("Transparent"), 0, this);
    pToolDisplayMode->setStatusTip(tr("Tool Transparent/Solid"));
    pToolDisplayMode->setWhatsThis(tr("Transparent/Solid\n\nDisplay Tool In Transparent Or Solid Mode"));
    connect(pToolDisplayMode, SIGNAL(activated()), this, SLOT(slotToolDisplayMode()));

    pToolHide = new QAction(tr("Tool Hide"), tr("Hide"), 0, this );
    pToolHide->setStatusTip(tr("Tool Hide"));
    pToolHide->setWhatsThis(tr("Hide\n\nToggle Hiding/Displaying The Tool"));
    connect(pToolHide, SIGNAL(activated()), this, SLOT(slotToolHide()));

    pToolColor = new QAction(tr("Tool Color"), tr("Color"), 0, this );
    pToolColor->setStatusTip(tr("Tool Color"));
    pToolColor->setWhatsThis(tr("Color\n\nChange Color Of The Tool"));
    connect(pToolColor, SIGNAL(activated()), this, SLOT(slotToolColor()));

    pToggleTrail = new QAction(tr("Trail Toggle"), tr("Off"), QAccel::stringToKey(tr("T")), this );
    pToggleTrail->setStatusTip(tr("Toggle Tool Trail"));
    pToggleTrail->setWhatsThis(tr("Toggle Tool Trail\n\nEnable/Disable Display Of The Tool Trail"));
    connect(pToggleTrail, SIGNAL(activated()), this, SLOT(slotToggleTrail()));

    pModelDisplayMode = new QAction(tr("Model Display Mode"), tr("Wireframe"), 0, this );
    pModelDisplayMode->setStatusTip(tr("Model Wireframe/Shaded"));
    pModelDisplayMode->setWhatsThis(tr("Wireframe/Shaded\n\nDisplay Model In Wireframe Or Shaded Mode"));
    connect(pModelDisplayMode, SIGNAL(activated()), this, SLOT(slotModelDisplayMode()));

    pModelHide = new QAction(tr("Model Hide"), tr("Hide"), 0, this );
    pModelHide->setStatusTip(tr("Model Hide"));
    pModelHide->setWhatsThis(tr("Hide\n\nToggle Hiding/Displaying The Model"));
    connect(pModelHide, SIGNAL(activated()), this, SLOT(slotModelHide()));

    pModelColor = new QAction(tr("Model Color"), tr("Color"), 0, this );
    pModelColor->setStatusTip(tr("Model Color"));
    pModelColor->setWhatsThis(tr("Color\n\nChange Color Of The Model"));
    connect(pModelColor, SIGNAL(activated()), this, SLOT(slotModelColor()));

    pModelMaterial = new QAction(tr("Model Material"), tr("Material"), 0, this );
    pModelMaterial->setStatusTip(tr("Model Material"));
    pModelMaterial->setWhatsThis(tr("Color\n\nChange Material Of The Model"));
    connect(pModelMaterial, SIGNAL(activated()), this, SLOT(slotModelMaterial()));


    pModelNormals = new QAction(tr("Model Normals"), tr("Normals"), 0, this );
    pModelNormals->setStatusTip(tr("Model Normals"));
    pModelNormals->setWhatsThis(tr("Normals\n\nToggle Display Of Normals Of The Model"));
    connect(pModelNormals, SIGNAL(activated()), this, SLOT(slotModelNormals()));

    pToggleSirus = new QAction(tr("Sirus Toggle"), tr("Hide Sirus"), QAccel::stringToKey(tr("S")), this );
    pToggleSirus->setStatusTip(tr("Toggle Sirus"));
    pToggleSirus->setWhatsThis(tr("Toggle Sirus\n\nEnable/Disable Display Of The Sirus"));
    connect(pToggleSirus, SIGNAL(activated()), this, SLOT(slotToggleSirus()));

    pColToolPath = new QAction(tr("ToolPath Color"), tr("Tool Path"), 0, this );
    pColToolPath->setStatusTip(tr("ToolPath Color"));
    pColToolPath->setWhatsThis(tr("Color\n\nChange Color Of The ToolPath"));
    connect(pColToolPath, SIGNAL(activated()), this, SLOT(slotColToolPath()));

    pColFeedTrail = new QAction(tr("FeedTrail Color"), tr("Feed Trail"), 0, this );
    pColFeedTrail->setStatusTip(tr("FeedTrail Color"));
    pColFeedTrail->setWhatsThis(tr("Color\n\nChange Color Of The FeedTrail"));
    connect(pColFeedTrail, SIGNAL(activated()), this, SLOT(slotColFeedTrail()));

    pColRapidTrail = new QAction(tr("RapidTrail Color"), tr("Rapid Trail"), 0, this );
    pColRapidTrail->setStatusTip(tr("RapidTrail Color"));
    pColRapidTrail->setWhatsThis(tr("Color\n\nChange Color Of The RapidTrail"));
    connect(pColRapidTrail, SIGNAL(activated()), this, SLOT(slotColRapidTrail()));

    pColG02Trail = new QAction(tr("G02 Color"), tr("G02 Trail"), 0, this );
    pColG02Trail->setStatusTip(tr("G02 Color"));
    pColG02Trail->setWhatsThis(tr("Color\n\nChange Color Of The G02 Interpolation"));
    connect(pColG02Trail, SIGNAL(activated()), this, SLOT(slotColG02Trail()));

    pColG03Trail = new QAction(tr("G03 Color"), tr("G03 Trail"), 0, this );
    pColG03Trail->setStatusTip(tr("G03 Color"));
    pColG03Trail->setWhatsThis(tr("Color\n\nChange Color Of The G03 Interpolation"));
    connect(pColG03Trail, SIGNAL(activated()), this, SLOT(slotColG03Trail()));

    pGridSettings = new QAction(tr("Grid Settings"), tr("Settings"), 0, this );
    pGridSettings->setStatusTip(tr("Grid Settings"));
    pGridSettings->setWhatsThis(tr("Settings\n\nChange Dimensions Of The Grid"));
    connect(pGridSettings, SIGNAL(activated()), this, SLOT(slotGridSettings()));

    pColGrid = new QAction(tr("Grid Color"), tr("Color"), 0, this );
    pColGrid->setStatusTip(tr("Grid Color"));
    pColGrid->setWhatsThis(tr("Color\n\nChange Color Of The Grid"));
    connect(pColGrid, SIGNAL(activated()), this, SLOT(slotColGrid()));

    pToggleGrid = new QAction(tr("Toggle Grid"), tr("On"), QAccel::stringToKey(tr("G")), this );
    pToggleGrid->setStatusTip(tr("Toggle Grid"));
    pToggleGrid->setWhatsThis(tr("Toggle Grid\n\nToggle On/Off Display Of The Grid"));
    connect(pToggleGrid, SIGNAL(activated()), this, SLOT(slotToggleGrid()));

    pToggleCircInt = new QAction(tr("Toggle CircInt"), tr("Show Circular Interp"), QAccel::stringToKey(tr("C")), this );
    pToggleCircInt->setStatusTip(tr("Toggle Circular Interpolation"));
    pToggleCircInt->setWhatsThis(tr("Toggle CircInt\n\nToggle On/Off Display Of The Circular Interpolation"));
    connect(pToggleCircInt, SIGNAL(activated()), this, SLOT(slotToggleCircInt()));

    pTransform = new QAction(tr("Transformations"), tr("Transformations"), 0, this );
    pTransform->setStatusTip(tr("Set Transformations"));
    pTransform->setWhatsThis(tr("Transformations\n\nSet Transformations For The Sirus"));
    connect(pTransform, SIGNAL(activated()), this, SLOT(slotTransform()));

    //util actions
    pSearchBlock = new QAction(tr("Search Block"), searchBlocIcon, tr("Search Block"), 0, this );
    pSearchBlock->setStatusTip(tr("Search An NC Block"));
    pSearchBlock->setWhatsThis(tr("Search Block\n\nSearch A Specified NC Block In The Sirus"));
    connect(pSearchBlock, SIGNAL(activated()), this, SLOT(slotSearchBlock()));

    pPickBlock = new QAction(tr("Pick Block"), pickBlocIcon, tr("Pick Block"), 0, this );
    pPickBlock->setStatusTip(tr("Pick Block"));
    pPickBlock->setWhatsThis(tr("Pick Block\n\nPick Block In The Sirus From The Screen"));
    connect(pPickBlock, SIGNAL(activated()), this, SLOT(slotPickBlock()));

    pMeasDist = new QAction(tr("Measure Distance"), measDistIcon, tr("Measure Distance"), 0, this );
    pMeasDist->setStatusTip(tr("Measure Distance"));
    pMeasDist->setWhatsThis(tr("Measure Distance\n\nMeasure Distance Between Two Blocks In The Sirus From The Screen"));
    connect(pMeasDist, SIGNAL(activated()), this, SLOT(slotMeasDist()));

     pMeasAng = new QAction(tr("Measure Angle"), measDistIcon, tr("Measure Angle"), 0, this );
    pMeasAng->setStatusTip(tr("Measure Angle"));
    pMeasAng->setWhatsThis(tr("Measure Angle\n\nMeasure Vector Angle Between Three Blocks In The Sirus From The Screen"));
    connect(pMeasAng, SIGNAL(activated()), this, SLOT(slotMeasAng()));    
    
    pSimulRep = new QAction(tr("Simul Rep"), tr("Simulation Report"), 0, this );
    pSimulRep->setStatusTip(tr("Show Simulation Report"));
    pSimulRep->setWhatsThis(tr("Simul Report\n\nShow The Simulation Report"));
    connect(pSimulRep, SIGNAL(activated()), this, SLOT(slotSimulRep()));

}

void VirmacApp::initMenuBar()
{
    ///////////////////////////////////////////////////////////////////
    // MENUBAR

    ///////////////////////////////////////////////////////////////////
    // menuBar entry pFileMenu
    pFileMenu=new QPopupMenu();
    pFileMenu->insertTearOffHandle();
    fileNew->addTo(pFileMenu);
    //fileOpen->addTo(pFileMenu);
    fileReadGCode->addTo(pFileMenu);
    fileWriteGCode->addTo(pFileMenu);
    fileReadStl->addTo(pFileMenu);
    fileClose->addTo(pFileMenu); 
    pFileMenu->insertSeparator();
    fileQuit->addTo(pFileMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry editMenu
    pEditMenu=new QPopupMenu();
    pEditMenu->insertTearOffHandle();
    editGCode->addTo(pEditMenu);
    pEditMenu->insertSeparator();
    editTrimBefore->addTo(pEditMenu);
    editTrimAfter->addTo(pEditMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry viewMenu
    pViewMenu=new QPopupMenu();
    pViewMenu->setCheckable(true);
    pViewMenu->insertTearOffHandle();
    viewTop->addTo(pViewMenu);
    viewBottom->addTo(pViewMenu);
    viewFront->addTo(pViewMenu);
    viewBack->addTo(pViewMenu);
    viewLeft->addTo(pViewMenu);
    viewRight->addTo(pViewMenu);
    viewAxo->addTo(pViewMenu);
    viewMultiFour->addTo(pViewMenu);
    pViewMenu->insertSeparator();
    viewProjection->addTo(pViewMenu);
    pViewMenu->insertSeparator();
    viewFitall->addTo(pViewMenu);
    viewZoomwin->addTo(pViewMenu);
    viewReset->addTo(pViewMenu);
    viewAntial->addTo(pViewMenu);
    pViewMenu->insertSeparator();
    viewSnapshot->addTo(pViewMenu);
    pViewMenu->insertSeparator();
    viewToolBar->addTo(pViewMenu);
    viewStatusBar->addTo(pViewMenu);
    pViewMenu->insertSeparator();
    viewMesgBar->addTo(pViewMenu);
    viewPromBar->addTo(pViewMenu);
    ///////////////////////////////////////////////////////////////////
    // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE

    //simulation
    pSimulMenu=new QPopupMenu();
    pSimulMenu->setCheckable(true);
    pSimulMenu->insertTearOffHandle();
    animPlay->addTo(pSimulMenu);
    animPlayStep->addTo(pSimulMenu);
    animPickPlay->addTo(pSimulMenu);
    animRewind->addTo(pSimulMenu);
    animPause->addTo(pSimulMenu);
    animStop->addTo(pSimulMenu);
    pSimulMenu->insertSeparator();
    deleteSirus->addTo(pSimulMenu);
    deleteModel->addTo(pSimulMenu);
    pSimulMenu->insertSeparator();
    simulSettings->addTo(pSimulMenu);
    toolLibrary->addTo(pSimulMenu);
    rotaryAxis->addTo(pSimulMenu);
    startBlock->addTo(pSimulMenu);

    //preferences
    pPrefMenu=new QPopupMenu();
    pPrefMenu->setCheckable(true);
    pPrefMenu->insertTearOffHandle();

    pToolMenu = new QPopupMenu(pPrefMenu);
    pPrefMenu->insertItem(tr("&Tool"), pToolMenu);
    pToolMenu->insertTearOffHandle();
    pToolDisplayMode->addTo(pToolMenu);
    pToolHide->addTo(pToolMenu);
    pToolColor->addTo(pToolMenu);
    connect(pToolMenu, SIGNAL(aboutToShow()), this, SLOT(toolMenuATS()));

    pTrailMenu = new QPopupMenu(pToolMenu);
    pToolMenu->insertItem(tr("T&rail"), pTrailMenu);
    pToggleTrail->addTo(pTrailMenu);
    connect(pTrailMenu, SIGNAL(aboutToShow()), this, SLOT(trailMenuATS()));

    pModelMenu = new QPopupMenu(pPrefMenu);
    pPrefMenu->insertItem(tr("&Model"), pModelMenu);
    pModelMenu->insertTearOffHandle();
    pModelDisplayMode->addTo(pModelMenu);
    pModelHide->addTo(pModelMenu);
    pModelColor->addTo(pModelMenu);
    pModelMaterial->addTo(pModelMenu);
    pModelNormals->addTo(pModelMenu);
    connect(pModelMenu, SIGNAL(aboutToShow()), this, SLOT(modelMenuATS()));

    pToggleSirus->addTo(pPrefMenu);

    pColorMenu = new QPopupMenu(pPrefMenu);
    pPrefMenu->insertItem(tr("&Color"), pColorMenu);
    pColorMenu->insertTearOffHandle();
    pColToolPath->addTo(pColorMenu);
    pColFeedTrail->addTo(pColorMenu);
    pColRapidTrail->addTo(pColorMenu);
    pColG02Trail->addTo(pColorMenu);
    pColG03Trail->addTo(pColorMenu);

    pGridMenu = new QPopupMenu(pPrefMenu);
    pPrefMenu->insertItem(tr("&Grid"), pGridMenu);
    pGridMenu->insertTearOffHandle();
    pGridSettings->addTo(pGridMenu);
    pColGrid->addTo(pGridMenu);
    pToggleGrid->addTo(pGridMenu);
    connect(pGridMenu, SIGNAL(aboutToShow()), this, SLOT(gridMenuATS()));

    pPrefMenu->insertSeparator();
    pToggleCircInt->addTo(pPrefMenu);
    pTransform->addTo(pPrefMenu);

    //analysis
    pAnalysisMenu = new QPopupMenu();
    pAnalysisMenu->insertTearOffHandle();
    analysisPPInter->addTo(pAnalysisMenu);
    pAnalysisMenu->insertSeparator();
    analysisRoughStock->addTo(pAnalysisMenu);
    analysisWYSIWYC->addTo(pAnalysisMenu);

    //utilities
    pUtilMenu=new QPopupMenu();
    pUtilMenu->setCheckable(true);
    pUtilMenu->insertTearOffHandle();
    pSearchBlock->addTo(pUtilMenu);
    pPickBlock->addTo(pUtilMenu);
    pUtilMenu->insertSeparator();
    pMeasDist->addTo(pUtilMenu);
    pMeasAng->addTo(pUtilMenu);
    pUtilMenu->insertSeparator();
    pSimulRep->addTo(pUtilMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry windowMenu
    pWindowMenu = new QPopupMenu(this);
    pWindowMenu->insertTearOffHandle();
    pWindowMenu->setCheckable(true);
    connect(pWindowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

    ///////////////////////////////////////////////////////////////////
    // menuBar entry helpMenu
    pHelpMenu=new QPopupMenu();
    pHelpMenu->insertTearOffHandle();
    helpAboutApp->addTo(pHelpMenu);
    pHelpMenu->insertSeparator();
    pHelpMenu->insertItem(tr("What's &This"), this, SLOT(whatsThis()), SHIFT+Key_F1);

    menuBar()->insertItem(tr("&File"), pFileMenu);
    menuBar()->insertItem(tr("&Edit"), pEditMenu);
    menuBar()->insertItem(tr("&View"), pViewMenu);
    menuBar()->insertItem(tr("&Simulation"), pSimulMenu);
    menuBar()->insertItem(tr("&Preferences"), pPrefMenu);
    menuBar()->insertItem(tr("&Analysis"), pAnalysisMenu);
    menuBar()->insertItem(tr("&Utilities"), pUtilMenu);
    menuBar()->insertItem(tr("&Window"), pWindowMenu);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("&Help"), pHelpMenu);

}

void VirmacApp::initToolBar()
{
    ///////////////////////////////////////////////////////////////////
    // TOOLBAR
    fileToolbar = new QToolBar(this, "file operations");
    fileNew->addTo(fileToolbar);
    //fileOpen->addTo(fileToolbar);
    fileReadGCode->addTo(fileToolbar);
    fileReadStl->addTo(fileToolbar);
    fileToolbar->addSeparator();
    QWhatsThis::whatsThisButton(fileToolbar);


    viewBar = new QToolBar(this, " view Tool Bar");
    moveDockWindow( viewBar, Left );
    viewTop->addTo(viewBar);
    viewBottom->addTo(viewBar);
    viewFront->addTo(viewBar);
    viewBack->addTo(viewBar);
    viewLeft->addTo(viewBar);
    viewRight->addTo(viewBar);
    viewAxo->addTo(viewBar);
    viewMultiFour->addTo(viewBar);
    viewBar->addSeparator();
    viewProjection->addTo(viewBar);
    viewBar->addSeparator();
    viewFitall->addTo(viewBar);
    viewZoomwin->addTo(viewBar);
    viewReset->addTo(viewBar);
    viewAntial->addTo(viewBar);
    viewSnapshot->addTo(viewBar);

    animBar = new QToolBar(this, " Anim Tool Bar");
    moveDockWindow( animBar, Bottom );

    //animation buttons
    pPlayButton = new QToolButton(QPixmap(play), "Play", "Play The Simulation",  this, "slotPlay", animBar, "name");
    pPlayButton->setUsesBigPixmap(true);
    connect(pPlayButton, SIGNAL(clicked()), this, SLOT(slotPlay()));

    pPlayStepButton = new QToolButton(QPixmap(playstep), "Step", "Play The Simulation Step By Step",  this, "slotPlayStep", animBar, "name");
    pPlayStepButton->setUsesBigPixmap(true);
    connect(pPlayStepButton, SIGNAL(clicked()), this, SLOT(slotPlayStep()));

    pPickPlayButton = new QToolButton(QPixmap(pickplay), "Pick And Play", "Play The Simulation From Picked Block",  this, "slotPickPlay", animBar, "name");
    pPickPlayButton->setUsesBigPixmap(true);
    connect(pPickPlayButton, SIGNAL(clicked()), this, SLOT(slotPickPlay()));

    pRewindButton = new QToolButton(QPixmap(rewindxpm), "Rewind", "Rewind The Simulation",  this, "slotRewind", animBar, "name");
    pRewindButton->setUsesBigPixmap(true);
    connect(pRewindButton, SIGNAL(clicked()), this, SLOT(slotRewind()));

    pPauseButton = new QToolButton(QPixmap(pause_xpm), "Pause", "Pause The Simulation",  this, "slotPause", animBar, "name");
    pPauseButton->setUsesBigPixmap(true);
    connect(pPauseButton, SIGNAL(clicked()), this, SLOT(slotPause()));

    pStopButton = new QToolButton(QPixmap(stop), "Stop", "Stop The Simulation",  this, "slotStop", animBar, "name");
    pStopButton->setUsesBigPixmap(true);
    connect(pStopButton, SIGNAL(clicked()), this, SLOT(slotStop()));

    //Slider
    animBar->addSeparator();
    pSlider = new QSlider(1, 100, 10, 10, Qt::Horizontal, animBar);
    pSlider->setTickmarks(QSlider::Above);
    connect(pSlider, SIGNAL(valueChanged(int)), this, SLOT(slotPointsPerFrame(int)));

    //Points Per Frame
    animBar->addSeparator();
    pPointsPerFrame = new QLCDNumber(3, animBar);
    pPointsPerFrame->setSegmentStyle(QLCDNumber::Filled);
    pPointsPerFrame->display(10);
    connect(pSlider, SIGNAL(valueChanged(int)), pPointsPerFrame, SLOT(display(int)));
    ppfLabel = new QLabel(" Points Per Frame", animBar);

    //G Code Blocks
    animBar->addSeparator();
    blockStr.sprintf("N%d G%.2d X%0.3f Y%0.3f Z%0.3f A%0.3f B%0.3f C%0.3f", 1, 2, 100., 100., 100., 10., 10. ,90.);
    blockLabel = new QLabel(blockStr, animBar);
    blockLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    blockLabel->setFrameShape(QLabel::Box);
    blockLabel->setFrameShadow(QLabel::Raised);
    animBar->setStretchableWidget(blockLabel);

    utilBar = new QToolBar(this, " Anim Tool Bar");
    //  utilBar->setPaletteBackgroundColor(QColor(190,190,190));
    moveDockWindow( utilBar, Right );

    fileWriteGCode->addTo(utilBar);
    pPickBlock->addTo(utilBar);
    pSearchBlock->addTo(utilBar);
    editTrimBefore->addTo(utilBar);
    editTrimAfter->addTo(utilBar);
    pMeasDist->addTo(utilBar);
    pMeasAng->addTo(utilBar);


    //prompt bar
    promWinFrame = new QToolBar(this, "Prompt Bar");
    promWinFrame->setNewLine(true);

    mesgWinFrame = new QToolBar(this, "Message Bar");
    moveDockWindow( promWinFrame, DockBottom);
    moveDockWindow( mesgWinFrame, DockBottom);

    promWinFrame->setResizeEnabled(true);
    promWinFrame->setMovingEnabled(true);
    promWinFrame->setCloseMode(QToolBar::Always);
    promWinFrame->setVerticallyStretchable(true);
    promWinFrame->setHorizontallyStretchable(true);

    //message bar
    mesgWinFrame->setResizeEnabled(true);
    mesgWinFrame->setMovingEnabled(true);
    mesgWinFrame->setCloseMode(QToolBar::Always);
    mesgWinFrame->setVerticallyStretchable(true);
    mesgWinFrame->setHorizontallyStretchable(true);

    promBox = new QListBox(promWinFrame);
    //promBox->setPaletteBackgroundColor(QColor(220,220,220));
    promBox->setScrollBar(true);
    promBox->setBottomScrollBar(true);

    mesgBox = new QListBox(mesgWinFrame);
    //mesgBox->setPaletteBackgroundColor(QColor(220,220,220));
    mesgBox->setScrollBar(true);
    mesgBox->setBottomScrollBar(true);

    int cx = this->width();
    promBox->setMinimumSize(cx, cx/6);
    mesgBox->setMinimumHeight(cx/6);

    promWinFrame->setStretchableWidget(promBox);
    mesgWinFrame->setStretchableWidget(mesgBox);

}

void VirmacApp::initStatusBar()
{
    ///////////////////////////////////////////////////////////////////
    //STATUSBAR
    statusBar()->message(tr("Ready."));
}

#include "images/granite.xpm"
void VirmacApp::initView()
{
    ////////////////////////////////////////////////////////////////////
    // set the main widget here
    QVBox* view_back = new QVBox( this );
    view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    view_back->setLineWidth(5);
    pWorkspace = new QWorkspace( view_back );
    pWorkspace->setScrollBarsEnabled(true);
    pWorkspace->setPaletteBackgroundPixmap (QPixmap(granite_xpm) );  
    setCentralWidget(view_back);
}


void VirmacApp::createClient(VirmacDoc* doc)
{
    QPixmap appIcon = QPixmap(appicontr);
    VirmacView* w = new VirmacView(doc, pWorkspace,0,WDestructiveClose);
    w->setIcon(appIcon);
    w->installEventFilter(this);
    doc->addView(w);
    if ( pWorkspace->windowList().isEmpty() ) // show the very first window in maximized mode
    {
	w->showMaximized();
    }
    else
    {
	w->showMaximized();
	pWorkspace->tile();
    }

}

void VirmacApp::openDocumentFile(const char* file)
{
    statusBar()->message(tr("Opening file..."));
    VirmacDoc* doc;
    // check, if document already open. If yes, set the focus to the first view
    for(doc=pDocList->first(); doc != 0; doc=pDocList->next())
    {
	if(doc->pathName()==file)
	{
	    VirmacView* view=doc->firstView();	
	    view->setFocus();
	    return;
	}
    }
    doc = new VirmacDoc();
    pDocList->append(doc);
    doc->newDocument();
    // Creates an untitled window if file is 0	
    if(!file)
    {
	untitledCount+=1;
	QString fileName=QString(tr("Untitled%1")).arg(untitledCount);
	doc->setPathName(fileName);
	doc->setTitle(fileName);
    }
    // Open the file
    else
    {
	if(!doc->openDocument(file))
	{
	    QMessageBox::critical(this, tr("Error !"),tr("Could not open document !"));
	    delete doc;
	    return;
	}
    }
    // create the window
    createClient(doc);

    statusBar()->message(tr("Ready."));
}

bool VirmacApp::queryExit()
{
    int exit=QMessageBox::question(this, tr("Quit..."),
	    tr("Do your really want to quit?"),
	    QMessageBox::Yes, QMessageBox::No);

    if (exit==QMessageBox::Yes)
    {

    }
    else
    {

    };

    return (exit==QMessageBox::Yes);
}

bool VirmacApp::eventFilter(QObject* object, QEvent* event)
{
    if((event->type() == QEvent::Close)&&((VirmacApp*)object!=this))
    {
	QCloseEvent* e=(QCloseEvent*)event;
	VirmacView* pView=(VirmacView*)object;
	VirmacDoc* pDoc=pView->getDocument();
	if(pDoc->canCloseFrame(pView))
	{
	    pDoc->removeView(pView);
	    if(!pDoc->firstView())
		pDocList->remove(pDoc);

	    e->accept();
	}
	else
	    e->ignore();
    }
    return QWidget::eventFilter( object, event );    // standard event processing
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void VirmacApp::slotFileNew()
{
    statusBar()->message(tr("Creating new file..."));

    openDocumentFile();		

    statusBar()->message(tr("Ready."));
}

void VirmacApp::slotFileOpen()
{
    statusBar()->message(tr("Opening file..."));

    QString fileName = QFileDialog::getOpenFileName(0,0,this);
    if (!fileName.isEmpty())
    {
	openDocumentFile(fileName);		
    }
    statusBar()->message(tr("Ready."));
}


void VirmacApp::slotFileReadGCode()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(!view)
	return;
    view->killTimers();
    VirmacDoc* doc = (VirmacDoc*)view->getDocument();
    if(!doc)
	return;
    QString fileName;
    fileName = QFileDialog::getOpenFileName(0,"All Files (*.*);;G Code files (*.cnc *.iso *.nc)",this,0,"Read G Code File");
    if(doc->readGCode(fileName))
    {
	setMessage(tr("Loaded G Code File ") + fileName + tr(" For Simulation"));
	setPrompt(tr("Press Play To Start Simulation"));
	view->setCaption(fileName);
	slotToolLibrary();
    }
}

void VirmacApp::slotFileWriteGCode()
{

}

void VirmacApp::slotFileReadStL()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = view->getDocument();
	if(doc)
	{
	    QString fileName;
	    fileName = QFileDialog::getOpenFileName(0,"All Files (*.*);;StL files (*.stl *.STL)",this,0,"Read StL File");
	    doc->readStLFile(fileName);
	}
    }

}

void VirmacApp::slotFileClose()
{
    statusBar()->message(tr("Closing file..."));

    VirmacView* m = (VirmacView*)pWorkspace->activeWindow();
    if( m )
    {
	VirmacDoc* doc=m->getDocument();
	doc->closeDocument();
    }

    statusBar()->message(tr("Ready."));
}

void VirmacApp::slotFileQuit()
{ 
    statusBar()->message(tr("Exiting application..."));
    ///////////////////////////////////////////////////////////////////
    // exits the Application
    if(queryExit())
    {
	qApp->quit();
    }
    else
    {
    };
    statusBar()->message(tr("Ready."));
}

void VirmacApp::closeEvent( QCloseEvent* ce )
{
    int exit=QMessageBox::question(this, tr("Quit..."),
	    tr("Do your really want to quit?"),
	    QMessageBox::Yes, QMessageBox::No);
    if (exit==QMessageBox::Yes)
    {
	ce->accept();
	//qApp->quit();
    }
    else
    {
	ce->ignore();
    };
}

#include "GCodeEditor.h"
void VirmacApp::slotEditGCode()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view && !view->editorActive)
    {
	GCodeEditor* e = new GCodeEditor(view, "Editor", Qt::WDestructiveClose);
	e->loadGCode();
	e->show();
	view->editorActive = true;
    }
}

void VirmacApp::slotEditTrimBefore()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = view->getDocument();
	if(!doc->m_bGCodeLoaded)
	    return;
	view->trimBefore = true;
	view->PickBlock();
    }
}

void VirmacApp::slotEditTrimAfter()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = view->getDocument();
	if(!doc->m_bGCodeLoaded)
	    return;
	view->trimAfter = true;
	view->PickBlock();
    }
}

void VirmacApp::slotViewToolBar(bool toggle)
{
    statusBar()->message(tr("Toggle toolbar..."));
    ///////////////////////////////////////////////////////////////////
    // turn Toolbar on or off
    if (toggle== false)
    {
	fileToolbar->hide();
    }
    else
    {
	fileToolbar->show();
    };

    statusBar()->message(tr("Ready."));
}

void VirmacApp::slotViewStatusBar(bool toggle)
{
    statusBar()->message(tr("Toggle statusbar..."));
    ///////////////////////////////////////////////////////////////////
    //turn Statusbar on or off

    if (toggle == false)
    {
	statusBar()->hide();
    }
    else
    {
	statusBar()->show();
    }

    statusBar()->message(tr("Ready."));
}

void VirmacApp::slotViewMesgBar(bool toggle)
{
    statusBar()->message(tr("Toggle MessageBar..."));
    ///////////////////////////////////////////////////////////////////
    // turn Toolbar on or off
    if (toggle== false)
    {
	mesgWinFrame->hide();
    }
    else
    {
	mesgWinFrame->show();
    };

    statusBar()->message(tr("Ready."));
}

void VirmacApp::slotViewPromBar(bool toggle)
{
    statusBar()->message(tr("Toggle PromptBar..."));
    ///////////////////////////////////////////////////////////////////
    //turn Statusbar on or off

    if (toggle == false)
    {
	promWinFrame->hide();
	mesgWinFrame->setNewLine(true);
    }
    else
    {
	promWinFrame->show();
	mesgWinFrame->setNewLine(false);
    }

    statusBar()->message(tr("Ready."));
}

void VirmacApp::slotWindowNewWindow()
{
    statusBar()->message(tr("Opening new document view..."));

    VirmacView* m = (VirmacView*) pWorkspace->activeWindow();
    if ( m ){
	VirmacDoc* doc = m->getDocument();
	createClient(doc);
    }

    statusBar()->message(tr("Ready."));
}

#include <qdatetime.h>
#include "AboutDialog.h"
void VirmacApp::slotHelpAbout()
{
    QDateTime dt = QDateTime::currentDateTime();
    QDate d = dt.date();
    QString cYear = d.toString("yyyy");
    /*
    QMessageBox::about(this,tr("About..."),
	    tr("Virmac\n Version " VERSION
		"\nCopyright  2000- " + cYear + " N. Sharjith"));
		*/
    AboutDialog ab;
    ab.exec();
}

void VirmacApp::slotStatusHelpMsg(const QString &text)
{
    ///////////////////////////////////////////////////////////////////
    // change status message of whole statusbar temporary (text, msec)
    statusBar()->message(text, 2000);
}

void VirmacApp::windowMenuAboutToShow()
{
    pWindowMenu->clear();
    pWindowMenu->insertTearOffHandle();
    windowNewWindow->addTo(pWindowMenu);
    windowCascade->addTo(pWindowMenu);
    windowTile->addTo(pWindowMenu);

    if ( pWorkspace->windowList().isEmpty() )
    {
	windowAction->setEnabled(false);
    }
    else
    {
	windowAction->setEnabled(true);
    }

    pWindowMenu->insertSeparator();

    QWidgetList windows = pWorkspace->windowList();
    for ( int i = 0; i < int(windows.count()); ++i )
    {
	int id = pWindowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT( windowMenuActivated( int ) ) );
	pWindowMenu->setItemParameter( id, i );
	pWindowMenu->setItemChecked( id, pWorkspace->activeWindow() == windows.at(i) );
    }
}

void VirmacApp::windowMenuActivated( int id )
{
    QWidget* w = pWorkspace->windowList().at( id );
    if ( w )
	w->setFocus();
}

#include "images/crosscursor.xpm"
//viewer slots
void VirmacApp::slotTopView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->TopView();
	view->setCursor(QCursor(crosscursor));
    }
}

void VirmacApp::slotBottomView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->BottomView();
	view->setCursor(QCursor(crosscursor));
    }
}

void VirmacApp::slotFrontView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();		
	view->FrontView();
	view->setCursor(QCursor(crosscursor));
    }
}

void VirmacApp::slotBackView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();		
	view->BackView();
	view->setCursor(QCursor(crosscursor));
    }
}

void VirmacApp::slotLeftView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();		
	view->LeftView();
	view->setCursor(QCursor(crosscursor));
    }
}

void VirmacApp::slotRightView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();		
	view->RightView();
	view->setCursor(QCursor(crosscursor));
    }
}

void VirmacApp::slotAxoView()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->AxonView();
	view->setCursor(Qt::ArrowCursor);
    }
}

void VirmacApp::slotMultiFour()
{	
    bSplit = !bSplit;
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	if(bSplit)
	    view->MultiView(true);
	else
	    view->MultiView(false);
    }
}

void VirmacApp::slotProjection()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	GLProjectionType pt = view->GetView()->GetProjectionType();
	if(pt  == GLPERSPECTIVE)
	{
	    view->GetView()->SetProjectionType(GLORTHOGRAPHIC);
	    viewProjection->setMenuText("Perspective");
	    QPixmap perspIcon(persp);
	    viewProjection->setIconSet(perspIcon);
	}
	else
	{
	    view->GetView()->SetProjectionType(GLPERSPECTIVE);
	    viewProjection->setMenuText("Orthographic");
	    QPixmap orthoIcon(ortho);
	    viewProjection->setIconSet(orthoIcon);
	}
    }
}

void VirmacApp::slotFitall()
{

    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->GetView()->FitAll();
    }
}

void VirmacApp::slotZoomwin()
{   
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->winZoom = true;
	view->setCursor(QCursor(zoomwincursor));
	setPrompt(tr("Press And Drag MB_1 To Create Zoom Window"));
    }
}

void VirmacApp::slotReset()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->GetView()->ResetView();
	view->viewLabel.sprintf("Axonometric");
    }
}

void VirmacApp::slotAntial()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	view->setFocus();
	view->antialiased = !view->antialiased;
	if(!view->antialiased)
	    view->GetView()->SetAntiAliasing(false);
	else
	    view->GetView()->SetAntiAliasing(true);
    }
}

void VirmacApp::slotSnapshot()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	QString fn = QFileDialog::getSaveFileName();
	if ( !fn.isEmpty() )
	{
	    view->GetView()->SnapShot(fn);
	}
    }
}

void VirmacApp::slotBgColor()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {

	QColor col(0,0,0);
	col = QColorDialog::getColor ( col, this, 0 );
	if(!col.isValid())
	    return;
	view->GetView()->SetBackgroundColor((GLfloat)col.red()/255, (GLfloat)col.green()/255, (GLfloat)col.blue()/255);
    }
}

//animation
void VirmacApp::slotPlay()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->m_bGCodeLoaded)
	    {
		QMessageBox::critical(this, "Virmac Error", "No G Code File Loaded For Simulation",
			QMessageBox::Ok, 0, 0);
		return;
	    }
	    if(!view->animActive)
	    {
		timerID = view->startTimer(1);
		doc->HidePath(true);
		view->animActive = true;
	    }
	}
    }
}

void VirmacApp::slotPickPlay()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(!doc->m_bGCodeLoaded)
	{
	    QMessageBox::critical(this, "Virmac Error", "No G Code File Loaded For Simulation",
		    QMessageBox::Ok, 0, 0);
	    return;
	}
	if(view->animActive)
	{
	    view->killTimers();
	    view->animActive = false;
	    doc->HidePath(false);
	}
	view->pickMode = true;
	view->pickNplay = true;
	view->pickNplayActive = true;
	view->PickBlock();
    }
}
void VirmacApp::slotPlayStep()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->m_bGCodeLoaded)
	    {
		QMessageBox::critical(this, "Virmac Error", "No G Code File Loaded For Simulation",
			QMessageBox::Ok, 0, 0);
		return;
	    }
	    view->killTimers();
	    view->animActive = false;
	    doc->HidePath(true);
	    CNCBlock aBloc;
	    if(doc->myPathIter.More())
	    {
		aBloc = doc->myPathIter.Current();
		doc->myPathIter.Next();
		CPoint3D P(aBloc.X, aBloc.Y, aBloc.Z);
		doc->myCutter->MoveTo(P);
		doc->myCutter->Rotate(aBloc.A, aBloc.B, aBloc.C);
		doc->myTrail->AddBlock(aBloc);
		doc->blocString.sprintf("N%uld  G%.2d  X%0.3f  Y%0.3f  Z%0.3f  A%0.3f  B%0.3f  C%0.3f",
			(unsigned)aBloc.N,(int)aBloc.G,aBloc.X,aBloc.Y,aBloc.Z,aBloc.A,aBloc.B, aBloc.C);
		view->GetView()->Refresh();
	    }
	}
    }
}
void VirmacApp::slotRewind()
{	
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->m_bGCodeLoaded)
	    {
		QMessageBox::critical(this, "Virmac Error", "Nothing To Rewind!", 
			QMessageBox::Ok | QMessageBox::Default, 0, 0);
		return;
	    }

	    int res = QMessageBox::question( this, "Virmac Confirmation",
		    "Do You Really Want To Rewind The Simulation?",
		    QMessageBox::Yes | QMessageBox::Default,
		    QMessageBox::No  | QMessageBox::Escape, 0);
	    if(res == QMessageBox::No)
	    {
		return;
	    }

	    view->killTimers();
	    view->animActive = false;
	    doc->myCutter->MoveTo(CPoint3D(0,0,0));
	    doc->myCutter->Rotate(0, 0, 0);
	    doc->myTrail->Nullify();
	    doc->HidePath(false);
	    doc->myPathIter.Init();
	    if(view->pickNplayActive)
		view->pickNplayActive = false;
	    view->GetView()->Refresh();
	}
    }
}
void VirmacApp::slotPause()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	view->killTimers();
	view->animActive = false;
    }
}
void VirmacApp::slotStop()
{
    slotPause();
    slotRewind();
    setMessage(tr("Simulation Stopped By User!"));		
}

void VirmacApp::slotDeleteSirus()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = view->getDocument();
	if(doc)
	{
	    if(!doc->m_bGCodeLoaded)
		return;
	    doc->myPath->Nullify();
	    doc->myTrail->Nullify();
	    doc->m_bGCodeLoaded = false;
	    doc->GetDisplayContext()->RefreshVolume();
	}
    }
}

void VirmacApp::slotDeleteModel()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = view->getDocument();
	if(doc)
	{
	    if(!doc->m_bModelLoaded)
		return;
	    doc->myStLObj->Nullify();
	    doc->m_bModelLoaded = false;
	    doc->GetDisplayContext()->RefreshVolume();
	}
    }
}

#include "SimulSettings.h"
void VirmacApp::slotSimulSettings()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	SimulSettings *dlg = new SimulSettings(view);
	int res = dlg->exec();
	if(res == QDialog::Accepted)
	{
	    VirmacDoc* pDoc = view->getDocument();
	    if(pDoc)
	    {
		//pDoc->myPath->EnableSyntaxChecking(dlg->checkBox1->isChecked());
		//pDoc->StopAtIntersections(dlg->checkBox2->isChecked())
		switch(dlg->slider1->value())
		{
		    case 0:
			pDoc->myCutter->SetResolution(MODEL_LOW);
			break;
		    case 1:
			pDoc->myCutter->SetResolution(MODEL_MEDIUM);
			break;
		    case 2:
			pDoc->myCutter->SetResolution(MODEL_HIGH);
			break;
		    case 3:
			pDoc->myCutter->SetResolution(MODEL_VERYHIGH);
			break;
		    default:
			break;
		}
		pDoc->myPointsPerFrame = dlg->SpinBoxPPF->value();
		pSlider->setValue(pDoc->myPointsPerFrame);
	    }
	}
	delete dlg;
    }
}

#include "ToolLibrary.h"
#include <qtable.h>
void VirmacApp::slotToolLibrary()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	ToolLibrary *lib = new ToolLibrary(view, "", false, Qt::WDestructiveClose);
	lib->exec();                                                 
    }
}

#include "RotAxisSetting.h"
void VirmacApp::slotRotaryAxis()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(doc)
	{
	    RotAxisSetting *dlg = new RotAxisSetting(this);
	    int res = dlg->exec();
	    if(res == QDialog::Accepted)
	    {
		if(dlg->radioButton1->isOn())
		{
		    doc->myCutter->Rotate(dlg->lineEditA->text().toDouble(), dlg->lineEditB->text().toDouble(), dlg->lineEditC->text().toDouble());
		}
	    }
	}
    }	
}

#include "BlockEntry.h"
void VirmacApp::slotStartBlock()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(doc)
	{
	    if(!doc->m_bGCodeLoaded)
	    {
		QMessageBox::critical(this, "Error", "No G Code File Loaded", QMessageBox::Ok, 0, 0);
		return;
	    }
	    BlockEntry *dlg = new BlockEntry(this);
	    int res = dlg->exec();
	    if(res == QDialog::Accepted)
	    {
		for(doc->myPathIter.Init(); doc->myPathIter.More(); doc->myPathIter.Next())
		{
		    if(doc->myPathIter.Current().N == (unsigned) dlg->SpinBoxBlock->value())
			break;
		}
		QString str; str.sprintf("Start Block Number Is %d", dlg->SpinBoxBlock->value());
		setMessage(str);
	    }
	}
    }	
}

void VirmacApp::slotPointsPerFrame(int value)
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(doc)
	{
	    doc->myPointsPerFrame = value;
	    QString str;
	    str.sprintf("Set Points Per Frame %d", value);
	    setMessage(str);
	}
    }
}


//preferences
void VirmacApp::toolMenuATS()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(!doc->myCutter)
	    return;
	if(doc->myCutter->GetDisplayMode() == GLWIREFRAME)
	{
	    pToolMenu->changeItem(pToolMenu->idAt(1), "Solid");
	}
	else
	{
	    pToolMenu->changeItem(pToolMenu->idAt(1), "Transparent");
	}
	if(doc->myCutter->IsHidden())
	{
	    pToolMenu->changeItem(pToolMenu->idAt(2), "Show");
	}
	else
	{
	    pToolMenu->changeItem(pToolMenu->idAt(2), "Hide");
	}
    }
}
void VirmacApp::slotToolDisplayMode()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(!doc->myCutter)
	    return;
	if(doc->myCutter->GetDisplayMode() == GLWIREFRAME)
	{
	    doc->myCutter->SetDisplayMode(GLSHADED);
	    pToolDisplayMode->setMenuText("Transparent");
	}
	else
	{
	    doc->myCutter->SetDisplayMode(GLWIREFRAME);
	    pToolDisplayMode->setMenuText("Solid");
	}
    }
}

void VirmacApp::slotToolHide()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(!doc->myCutter)
	    return;
	if(doc->myCutter->IsHidden())
	{
	    doc->HideTool(false);
	    pToolHide->setMenuText(tr("Hide"));
	}
	else
	{
	    doc->HideTool(true);
	    pToolHide->setMenuText(tr("Show"));
	}
    }
}

void VirmacApp::slotToolColor()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myCutter)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myCutter->SetColor(col.red(), col.green(), col.blue());
	}
    }
}

void VirmacApp::trailMenuATS()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myTrail)
		return;
	    if(doc->myTrail->IsHidden())
	    {
		pToggleTrail->setMenuText("Show");
	    }
	    else
	    {
		pToggleTrail->setMenuText("Hide");
	    }
	}
    }
}

void VirmacApp::slotToggleTrail()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myTrail)
		return;
	    if(doc->myTrail->IsHidden())
	    {
		doc->myTrail->MakeHidden(false);
		pToggleTrail->setMenuText("Off");
	    }
	    else
	    {
		doc->myTrail->MakeHidden(true);
		pToggleTrail->setMenuText("On");
	    }
	    view->GetView()->Refresh();
	}
    }
}

void VirmacApp::modelMenuATS()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myStLObj)
		return;
	    if(doc->myStLObj->GetDisplayMode() == GLWIREFRAME)
	    {
		pModelHide->setMenuText("Shaded");
	    }
	    else
	    {
		pModelHide->setMenuText("Wireframe");
	    }
	    if(doc->myStLObj->IsHidden())
	    {
		pModelHide->setMenuText("Show");
	    }
	    else
	    {
		pModelHide->setMenuText("Hide");
	    }
	}
    }
}

void VirmacApp::slotModelDisplayMode()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(!doc->myStLObj)
	    return;
	if(doc && doc->myStLObj->GetDisplayMode() == GLWIREFRAME)
	{
	    doc->myStLObj->SetDisplayMode(GLSHADED);
	    pModelDisplayMode->setMenuText("Wireframe");
	}
	else
	{
	    doc->myStLObj->SetDisplayMode(GLWIREFRAME);
	    pModelDisplayMode->setMenuText("Shaded");
	}
    }
}

void VirmacApp::slotModelHide()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(!doc->myStLObj)
	    return;
	if(doc && doc->myStLObj->IsHidden())
	{
	    doc->HideModel(false);
	    pModelHide->setMenuText(tr("Hide"));
	}
	else
	{
	    doc->HideModel(true);
	    pModelHide->setMenuText(tr("Show"));
	}
    }
}

void VirmacApp::slotModelColor()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myStLObj)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myStLObj->SetColor(col.red(), col.green(), col.blue());
	}
    }
}

#include "MatlEditor.h"
void VirmacApp::slotModelMaterial()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myStLObj)
		return;
	    MatlEditor e;
	    if(e.exec() == QDialog::Rejected)
		return;
	    doc->myStLObj->SetMaterial(e.GetMaterial());
	}
    }
}


void VirmacApp::slotModelNormals()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myStLObj)
		return;
	    doc->myStLObj->ShowNormals(!doc->myStLObj->IsNormalShown());
	}
    }
}

void VirmacApp::slotToggleSirus()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myPath)
		return;
	    if(doc->myPath->IsHidden())
	    {
		doc->myPath->MakeHidden(false);
		pToggleSirus->setMenuText("Hide Sirus");
	    }
	    else
	    {
		doc->myPath->MakeHidden(true);
		pToggleSirus->setMenuText("Show Sirus");
	    }
	    view->GetView()->Refresh();
	}
    }
}

void VirmacApp::slotColToolPath()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myPath)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myPath->SetColor(col.red(), col.green(), col.blue());
	}
    }

}

void VirmacApp::slotColFeedTrail()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myTrail)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myTrail->SetG01Color(col.red(), col.green(), col.blue());
	}
    }
}

void VirmacApp::slotColRapidTrail()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myTrail)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myTrail->SetG00Color(col.red(), col.green(), col.blue());
	}
    }
}

void VirmacApp::slotColG02Trail()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myTrail)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myTrail->SetG02Color(col.red(), col.green(), col.blue());
	}
    }
}

void VirmacApp::slotColG03Trail()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();
	if(doc)
	{
	    if(!doc->myTrail)
		return;
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    doc->myTrail->SetG03Color(col.red(), col.green(), col.blue());
	}
    }
}

void VirmacApp::gridMenuATS()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {

	if(view->GetView()->IsGridOn())
	{
	    pToggleGrid->setMenuText("Off");
	}
	else
	{
	    pToggleGrid->setMenuText("On");
	}
    }
}

#include "GridSettings.h"
void VirmacApp::slotGridSettings()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	GridSettings dlg(this);
	if(dlg.exec() ==  QDialog::Accepted)
	{
	    CGLGrid* pGrid = view->GetView()->GetGrid();
	    view->GetView()->SetGridPara(pGrid->GetPlane(), dlg.SpinBoxSize->value(), dlg.SpinBoxStep->value());					
	}
    }
}

void VirmacApp::slotColGrid()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {		
	QColor col(0,0,0);
	col = QColorDialog::getColor ( col, this, 0 );
	if(!col.isValid())
	    return;
	view->GetView()->SetGridColor(col.red(), col.green(), col.blue());
    }
}

void VirmacApp::slotToggleGrid()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	if(view->GetView()->IsGridOn())
	{
	    view->GetView()->SetGridOn(false);
	    pToggleGrid->setMenuText(tr("On"));
	}
	else
	{
	    view->GetView()->SetGridOn(true);
	    pToggleGrid->setMenuText(tr("Off"));
	}
    }
}

void VirmacApp::slotToggleCircInt()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(view)
    {
	VirmacDoc* doc = view->getDocument();
	if(doc)
	{
	    if(doc->myPath)
	    {
		bool b = doc->myPath->IsArcShown();
		if(b)
		{				
		    doc->myPath->SetShowArc(false);
		    doc->myTrail->SetShowArc(false);
		    pToggleCircInt->setMenuText(tr("Show Circular Inter"));
		}
		else
		{
		    doc->myPath->SetShowArc(true);
		    doc->myTrail->SetShowArc(true);
		    pToggleCircInt->setMenuText(tr("Hide Circular Inter"));
		}
	    }
	}
    }
}

#include "Transformations.h"
void VirmacApp::slotTransform()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	Transformations *dlg = new Transformations(this);
	int res = dlg->exec();
	VirmacDoc* doc=view->getDocument();
	if(doc)
	{
	    if(!doc->myPath)
		return;
	    if(res == QDialog::Accepted)
	    {
		doc->DX = dlg->LineEditDX->text().toDouble();
		doc->DY = dlg->LineEditDY->text().toDouble();
		doc->DZ = dlg->LineEditDZ->text().toDouble();
		doc->CX = dlg->LineEditCX->text().toDouble();
		doc->CY = dlg->LineEditCY->text().toDouble();
		doc->CZ = dlg->LineEditCZ->text().toDouble();
		doc->RX = dlg->LineEditRX->text().toDouble();
		doc->RY = dlg->LineEditRY->text().toDouble();
		doc->RZ = dlg->LineEditRZ->text().toDouble();
		doc->XP = dlg->LineEditXP->text().toDouble();
		doc->YP = dlg->LineEditYP->text().toDouble();
		doc->ZP = dlg->LineEditZP->text().toDouble();

		//toolpath translations
		doc->myPath->SetTranslation(doc->DX, doc->DY, doc->DZ);
		doc->myPath->SetRotation(doc->CX, doc->CY, doc->CZ, doc->RX, doc->RY, doc->RZ);
		doc->myPath->SetScale(doc->XP, doc->YP, doc->ZP);

		//tooltrail translations
		doc->myTrail->SetTranslation(doc->DX, doc->DY, doc->DZ);
		doc->myTrail->SetRotation(doc->CX, doc->CY, doc->CZ, doc->RX, doc->RY, doc->RZ);
		doc->myTrail->SetScale(doc->XP, doc->YP, doc->ZP);

		//tool translations
		//donot scale tool with number... only sign
		double scalex, scaley, scalez;
		scalex = (doc->XP >= 0) ? 1 : -1;
		scaley = (doc->YP >= 0) ? 1 : -1;
		scalez = (doc->ZP >= 0) ? 1 : -1;

		doc->myCutter->SetTranslation(doc->DX, doc->DY, doc->DZ);
		doc->myCutter->SetRotation(doc->CX, doc->CY, doc->CZ, doc->RX, doc->RY, doc->RZ);
		doc->myCutter->SetScale(scalex, scaley, scalez);
	    }
	}
    }
}

//analysis
void VirmacApp::slotPPIntersection(bool flag)
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(doc)
	{
	    doc->m_bCheckIntersection = flag;	    
	}
    }
}

void VirmacApp::slotRoughStock()
{
    QMessageBox::information(this, "Sorry", "Not Yet Implemented!", QMessageBox::Ok, 0, 0);
}

void VirmacApp::slotWYSIWYC()
{
    QMessageBox::information(this, "Sorry", "Not Yet Implemented!", QMessageBox::Ok, 0, 0);
}

//utilities
#include "BlockEntry.h"
void VirmacApp::slotSearchBlock()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc=view->getDocument();
	if(doc)
	{
	    if(!doc->myPath)
		return;
	    BlockEntry *dlg = new BlockEntry(this);
	    CNCBlock aBloc;
	    QString str;
	    int totCnt = doc->myPath->GetBlockCount();
	    //fprintf(stderr, "Count %d\n", totCnt);
	    if(dlg->exec() == QDialog::Accepted)
	    {
		unsigned bNum = (unsigned)dlg->SpinBoxBlock->value();
		int cnt = 1;
		for(doc->myPathIter.Init(); doc->myPathIter.More(); doc->myPathIter.Next())
		{
		    aBloc = doc->myPathIter.Current();
		    if(aBloc.N == bNum)
		    {
			str.sprintf("N%uld  G%d  X%0.3f  Y%0.3f  Z%0.3f  A%0.3f  B%0.3f  C%0.3f\nSearch Next Occurance?",(unsigned)aBloc.N,aBloc.G,
				aBloc.X,aBloc.Y,aBloc.Z,aBloc.A,aBloc.B,aBloc.C);
			setMessage(str);
			doc->ShowSelectedBloc(aBloc);
			view->GetView()->Refresh();
			if(QMessageBox::information(this, "NC Block Information", str, QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
			    break;   
			}
		    }
		    else if(totCnt == cnt)
		    {
			str.sprintf("Bloc Not Found");
			setMessage(str);
			QMessageBox::information(this, "NC Block Information", str, QMessageBox::Ok);
		    }
		    ++cnt;
		}
		view->GetView()->Refresh();
		doc->myPathIter.Init();
	    }
	}
    }
}

void VirmacApp::slotPickBlock()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if( view )
    {
	VirmacDoc* doc = view->getDocument();
	if(!doc->myPath)
	    return;
	view->PickBlock();
    }
}

void VirmacApp::slotMeasDist()
{
}

void VirmacApp::slotMeasAng()
{
}

#include "SimulReport.h"
#include "ListException.h"
void VirmacApp::slotSimulRep()
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    VirmacDoc* pDoc = view->getDocument();
    if(!pDoc->m_bGCodeLoaded)
	return;
    QString fName, tData, pLength, cutTime,rapidRate;
    SimulReportDlg dlg;
    CNCBlock G1, G2;
    CPoint3D P1, P2;
    double plen = 0;
    double ctTime = 0;
    double len=0;
    //try
    //{
    for(pDoc->myPathIter.Init();  pDoc->myPathIter.More(); pDoc->myPathIter.Next())
    {
	G1 = pDoc->myPathIter.Current();
	pDoc->myPathIter.Next();
	G2 = pDoc->myPathIter.Current();
	P1.SetParam(G1.X,G1.Y,G1.Z);
	P2.SetParam(G2.X,G2.Y,G2.Z);
	plen+=P1.Distance(P2);
	len = P1.Distance(P2);
	if(len==0||G2.F==0)
	    ctTime+=0;
	else
	    ctTime+=(len/G2.F);
    }
    //}
    //catch(CListException ex)
    //{
    //qWarning("Exception Caught\n %s", ex.ErrorMessage());
    //return;
    //}
    if(ctTime!=0)
	ctTime/=(double)60;

    fName = pDoc->title();

    tData.sprintf("Diameter:  \t%0.4g\nCorner Radius:  \t%0.4g\nLength:  \t%0.4g\n",pDoc->myCutter->GetBodyDia(),
	    pDoc->myCutter->GetBallRad(),pDoc->myCutter->GetShankLen());

    pLength.sprintf("%0.4g units",plen);

    cutTime.sprintf("%0.4g hours",ctTime);

    rapidRate.sprintf("%0.0g units/min",(double)10000/*pDoc->rapidRate*/);

    dlg.textLabelFileName->setText(fName);
    dlg.textLabelToolDia->setText(tData);
    dlg.textLabelPathLen->setText(pLength);
    dlg.textLabelCuttingTime->setText(cutTime);
    dlg.textLabelRapid->setText(rapidRate);
    dlg.exec();
}



void VirmacApp::setMessage(const QString& str)
{
    QString msg;
    int cnt = mesgBox->count();
    ++mesgCnt;
    msg.sprintf("MSG #%d ", mesgCnt);
    msg += str;
    mesgBox->insertItem(msg);
    mesgBox->setCurrentItem(cnt);
    if(cnt > 100)
	mesgBox->removeItem(0);
}

void VirmacApp::setPrompt(const QString& str)
{
    QString prm;
    int cnt = promBox->count();
    ++promCnt;
    prm.sprintf("PRM #%d ", promCnt);
    prm += str;
    promBox->insertItem(prm);
    promBox->setCurrentItem(cnt);
    if(cnt > 100)
	promBox->removeItem(0);
}

void VirmacApp::timerEvent(QTimerEvent* /*e*/)
{
    VirmacView* view = (VirmacView*)pWorkspace->activeWindow();
    if(!view)
	return;
    VirmacDoc* doc = view->getDocument();
    if(!doc)
	return;
    doc->AnimFunction();
    blockLabel->setText(doc->blocString);
    view->GetView()->Refresh();
}

