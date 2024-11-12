/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void Splash::init()
{
	setWFlags(Qt::WX11BypassWM | Qt::WDestructiveClose | Qt::WStyle_StaysOnTop | Qt::WStyle_NoBorder);
	// signals and slots connections
    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
    timer->start( 4000, TRUE ); // 4 seconds single-shot timer
}


void Splash::timerDone()
{
	killTimers();
	close(true);
}


void Splash::mousePressEvent( QMouseEvent * /*e*/ )
{
	killTimers();
	close(true);
}


void Splash::keyPressEvent( QKeyEvent * /*e*/ )
{
	killTimers();
	close(true);
}
