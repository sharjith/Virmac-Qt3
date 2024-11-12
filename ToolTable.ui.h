/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you want to add, delete, or rename functions or slots, use
 ** Qt Designer to update this file, preserving your code.
 **
 ** You should not define a constructor or destructor in this file.
 ** Instead, write your code in functions called init() and destroy().
 ** These will automatically be called by the form's constructor and
 ** destructor.
 *****************************************************************************/



void ToolTable::init()
{

}


void ToolTable::slotPageChanged( QWidget* page )
{
    int index = tabWidget2->indexOf(page);

    switch (index)
    {
	case 0:    
	    taperDiaA->setText("25");
	    bodyDia->setText("25");
	    bodyLen->setText("25");
	    ballRad->setText("12.5");
	    ballRad->show();
	    textLabel8->show();
	    break;
	case 1:
	    taperDiaA->setText("20");
	    bodyDia->setText("20");
	    bodyLen->setText("20");
	    ballRad->setText("12.5");
	    ballRad->show();
	    textLabel8->show();
	    break;
	case 2:
	    taperDiaA->setText("25");
	    bodyDia->setText("25");
	    bodyLen->setText("25");
	    ballRad->setText("12.5");
	    ballRad->show();
	    textLabel8->show();
	    break;
	case 3:
	    taperDiaA->setText("25");
	    bodyDia->setText("25");
	    bodyLen->setText("25");
	    ballRad->setText("0");
	    ballRad->hide();
	    textLabel8->hide();
	    break;
	default:
	    break;
    }
}


void ToolTable::slotHelp()
{
	qWarning("Not.....");
}
