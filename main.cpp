/***************************************************************************
  main.cpp  -  description
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

#include <unistd.h>
#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>

#include "Virmac.h"
#include "Splash.h"


int main(int argc, char *argv[])
{
    //QApplication::setStyle(QStyleFactory::create("Motif"));
    QApplication a(argc, argv);
    //QPalette pal(QColor( 65, 93, 93 ), QColor(204, 204, 204));
    /*QPalette pal( QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Active, QColorGroup::Background, QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Active, QColorGroup::Base, QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Active, QColorGroup::Highlight, QColor(204, 204, 204) );
    pal.setColor( QPalette::Active, QColorGroup::HighlightedText, QColor( 22, 22, 22 ) );
    pal.setColor( QPalette::Active, QColorGroup::Foreground, QColor(204, 204, 204) );
    pal.setColor( QPalette::Active, QColorGroup::Text, QColor(204, 204, 204) );
    pal.setColor( QPalette::Active, QColorGroup::ButtonText, QColor(204, 204, 204) );

    pal.setColor( QPalette::Inactive, QColorGroup::Background, QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Inactive, QColorGroup::Base, QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Inactive, QColorGroup::Highlight, QColor(204, 204, 204) );
    pal.setColor( QPalette::Inactive, QColorGroup::HighlightedText, QColor( 22, 22, 22 ) );
    pal.setColor( QPalette::Inactive, QColorGroup::Foreground, QColor(204, 204, 204) );
    pal.setColor( QPalette::Inactive, QColorGroup::Text, QColor(204, 204, 204) );
    pal.setColor( QPalette::Inactive, QColorGroup::ButtonText, QColor(204, 204, 204) );
    
    pal.setColor( QPalette::Disabled, QColorGroup::Background, QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Disabled, QColorGroup::Base, QColor( 65, 93, 93 ) );
    pal.setColor( QPalette::Disabled, QColorGroup::Highlight, QColor(204, 204, 204) );
    pal.setColor( QPalette::Disabled, QColorGroup::HighlightedText, QColor( 22, 22, 22 ) );
    pal.setColor( QPalette::Disabled, QColorGroup::Foreground, Qt::gray );
    pal.setColor( QPalette::Disabled, QColorGroup::Text, Qt::gray );
    pal.setColor( QPalette::Disabled, QColorGroup::ButtonText, Qt::gray );*/
    
    //qApp->setPalette( pal, TRUE );
    QTranslator tor( 0 );
    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.
    tor.load( QString("virmac.") + QTextCodec::locale(), "." );
    a.installTranslator( &tor );

    VirmacApp *virmac=new VirmacApp();
    a.setMainWidget(virmac);

    Splash *sp = new Splash(0, 0, Qt::WX11BypassWM | Qt::WDestructiveClose | Qt::WStyle_StaysOnTop | Qt::WStyle_NoBorder);
    QDesktopWidget *d = QApplication::desktop();
    QRect dRect = d->rect();
    QPoint p = dRect.center();
    int w = sp->width();
    int h = sp->height();
    sp->setGeometry(p.x()-w/2, p.y()-h/2, w, h);
    sp->show();
    qApp->processEvents();
    sleep(2);

    virmac->showMaximized();

    if(argc>1)
	virmac->openDocumentFile(argv[1]);
    else
	virmac->openDocumentFile();

    return a.exec();
}
