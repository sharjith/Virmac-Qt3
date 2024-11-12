TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on debug opengl

LIBS	+= -L$(HOME)/lib64 -lVKGeom -lVKGraphic -lVKCollection

DEFINES	+= 'VERSION="5.5"' QT_CLEAN_NAMESPACE

INCLUDEPATH	+= $(HOME)/include

HEADERS	+= BlockEntry.ui.h \
	CircEntryPara.ui.h \
	GCodeEditor.ui.h \
	GCodeException.h \
	GLDisplayContext.h \
	GridSettings.ui.h \
	ListIteratorOfListOfNCBlock.h \
	ListIteratorOfListOfCTriFacet.h \
	ListNodeOfNCBlock.h \
	ListNodeOfCTriFacet.h \
	ListOfNCBlock.h \
	ListOfCTriFacet.h \
	MatlEditor.ui.h \
	MDIWindow.h \
	MillCutter.h \
	NCBlock.h \
	QGLView.h \
	RetractPara.ui.h \
	RotAxisSetting.ui.h \
	SimulSettings.ui.h \
	Splash.ui.h \
	StLObject.h \
	StLReader.h \
	ToolLibrary.ui.h \
	ToolPath.h \
	ToolTable.ui.h \
	ToolTrail.h \
	ToolViewer.h \
	Transformations.ui.h \
	TriFacet.h \
	Virmac.h \
	VirmacDoc.h \
	VirmacView.h

SOURCES	+= GLDisplayContext.cpp \
	ListIteratorOfListOfNCBlock.cpp \
	ListIteratorOfListOfCTriFacet.cpp \
	ListNodeOfNCBlock.cpp \
	ListNodeOfCTriFacet.cpp \
	ListOfNCBlock.cpp \
	ListOfCTriFacet.cpp \
	MDIWindow.cpp \
	MillCutter.cpp \
	NCBlock.cpp \
	QGLView.cpp \
	StLObject.cpp \
	StLReader.cpp \
	ToolPath.cpp \
	ToolTrail.cpp \
	ToolViewer.cpp \
	Virmac.cpp \
	VirmacDoc.cpp \
	VirmacView.cpp \
	main.cpp

FORMS	= BlockEntry.ui \
	CircEntryPara.ui \
	GCodeEditor.ui \
	GridSettings.ui \
	MatlEditor.ui \
	RetractPara.ui \
	RotAxisSetting.ui \
	SimulReport.ui \
	SimulSettings.ui \
	Splash.ui \
	ToolLibrary.ui \
	ToolTable.ui \
	Transformations.ui \
	AboutDialog.ui

IMAGES	= images/antial.xpm \
	images/appicon.xpm \
	images/appicontr.xpm \
	images/axo.xpm \
	images/back.xpm \
	images/ballend.png \
	images/ballend_cutter.bmp \
	images/ballend_cutter.xpm \
	images/bg.xpm \
	images/blackpl.png \
	images/blackru.png \
	images/bottom.xpm \
	images/brass.png \
	images/bronze.png \
	images/cancel.xpm \
	images/cancelold.xpm \
	images/chrome.png \
	images/clear.xpm \
	images/close.xpm \
	images/copper.png \
	images/copy.xpm \
	images/cross.xpm \
	images/cut.xpm \
	images/cylindrical_cutter.bmp \
	images/cylindrical_cutter.xpm \
	images/edit.png \
	images/edit.xpm \
	images/emerald.png \
	images/filegcode.xpm \
	images/filenew.xpm \
	images/fileopen.xpm \
	images/filesave.xpm \
	images/filesaveas.xpm \
	images/filestl.xpm \
	images/filewritegcode.xpm \
	images/fitall.xpm \
	images/front.xpm \
	images/gold.png \
	images/granite.xpm \
	images/help.xpm \
	images/jade.png \
	images/left.xpm \
	images/logo.png \
	images/multi.xpm \
	images/newsplash.xpm \
	images/obsidian.png \
	images/ok.xpm \
	images/okcur.xpm \
	images/okold.xpm \
	images/ortho.xpm \
	images/pancursor.xpm \
	images/paste.xpm \
	images/pause.xpm \
	images/pearl.png \
	images/persp.xpm \
	images/pickbloc.xpm \
	images/pickcursor.xpm \
	images/pickplay.xpm \
	images/play.xpm \
	images/playstep.xpm \
	images/quest.xpm \
	images/quit.xpm \
	images/reload.xpm \
	images/resetview.xpm \
	images/rewind.xpm \
	images/right.xpm \
	images/rotatecursor.xpm \
	images/rotax.xpm \
	images/ruby.png \
	images/searchbloc.xpm \
	images/silver.png \
	images/snapshot.xpm \
	images/snapshot1.png \
	images/spherical_cutter.bmp \
	images/spherical_cutter.xpm \
	images/splash.xpm \
	images/stl.xpm \
	images/stop.xpm \
	images/tmp.xpm \
	images/top.xpm \
	images/torical_cutter.bmp \
	images/torical_cutter.xpm \
	images/trimafter.xpm \
	images/trimbefore.xpm \
	images/turquoise.png \
	images/view.xpm \
	images/zoomcursor.xpm \
	images/zoomwin.xpm \
	images/zoomwincursor.xpm \
	images/splash_small.png

TARGET	= virmac
TRANSLATIONS += Virmac_de.ts Virmac_fr.ts
unix:{
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
win32:{
    UI_DIR = ui
    MOC_DIR = moc
    OBJECTS_DIR = obj
}

