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

#include <qmessagebox.h>

void ToolLibrary::init()
{   
    myViewer = new ToolViewer(frame3, "", (QGLWidget*)parent());
    if(!myViewer)
    {
	qWarning("Cannot Create Viewer!");
	return;
    }
    // Put the GL widget inside the frame
    QHBoxLayout* flayout = new QHBoxLayout( frame3, 2, 2, "flayout");
    flayout->addWidget( myViewer, 1 );

    //load tools
    int toolNum = 0; 
    unsigned i;
    double HolderDia, HolderLen, ShankDia, ShankLen, TaperDiaA,TaperDiaB, TaperLen, BodyDia, BodyLen, BallRad;
    char typ[20];
    char env[200];
    strcpy(env, getenv("VRMTLIB"));
    QString pth(env);
    pth += "/cutter.dat";
    FILE* fp = fopen(pth, "r");
    if(fp)
    {
	fscanf(fp, "%d", &toolCount);
	for(i = 0; i < toolCount; ++i)
	{
	    fscanf(fp, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",  typ, &BodyDia,  &BallRad,  &BodyLen, &HolderDia, &HolderLen, &ShankDia, &ShankLen, &TaperDiaA, &TaperDiaB, &TaperLen);
	    table1->setText(toolNum, 0, typ);
	    table1->setText(toolNum, 1, QString("%1").arg(BodyDia));
	    table1->setText(toolNum, 2, QString("%1").arg(BallRad));
	    table1->setText(toolNum, 3, QString("%1").arg(BodyLen));
	    table1->setText(toolNum, 4, QString("%1").arg(HolderDia));
	    table1->setText(toolNum, 5, QString("%1").arg(HolderLen));
	    table1->setText(toolNum, 6, QString("%1").arg(ShankDia));
	    table1->setText(toolNum, 7, QString("%1").arg(ShankLen));
	    table1->setText(toolNum, 8, QString("%1").arg(TaperDiaA));
	    table1->setText(toolNum, 9, QString("%1").arg(TaperDiaB));
	    table1->setText(toolNum, 10, QString("%1").arg(TaperLen));
	    toolNum++;
	}
	fclose(fp);
    }
    else
    {
	toolCount = 1;
	QMessageBox::critical(0, tr("LoadTool"), tr("File cutter.dat not found"), QMessageBox::Ok, 0, 0);
    }
}

void ToolLibrary::loadTool()
{
    int row = table1->currentRow();

    VirmacView* view = (VirmacView*)parent();
    if( view )
    {
	view->setFocus();
	VirmacDoc* doc = (VirmacDoc*)view->getDocument();

	QString typ = table1->text(row, 0);
	if(typ == "BALLNOSE")
	    doc->myCutter->SetCutterType(MILL_BALL_NOSE);
	if(typ == "SPHERICAL")
	    doc->myCutter->SetCutterType(MILL_SPHERICAL);
	if(typ == "TORICAL")
	    doc->myCutter->SetCutterType(MILL_TOROIDAL);
	if(typ == "CYLINDRICAL")
	    doc->myCutter->SetCutterType(MILL_CYLINDRICAL);

	doc->myCutter->SetBodyDia(table1->text(row, 1).toDouble());
	doc->myCutter->SetBallRad(table1->text(row, 2).toDouble());
	doc->myCutter->SetBodyLen(table1->text(row, 3).toDouble());
	doc->myCutter->SetHolderDia (table1->text(row, 4).toDouble());
	doc->myCutter->SetHolderLen(table1->text(row, 5).toDouble());
	doc->myCutter->SetShankDia(table1->text(row, 6).toDouble());
	doc->myCutter->SetShankLen(table1->text(row, 7).toDouble());
	doc->myCutter->SetTaperDiaA(table1->text(row, 8).toDouble());
	doc->myCutter->SetTaperDiaB(table1->text(row, 9).toDouble());
	doc->myCutter->SetTaperLen(table1->text(row, 10).toDouble());
	doc->myCutter->RebuildDisplayLists();
	view->GetView()->Refresh();

	//QMessageBox::information(0, tr("LoadTool"), tr("Ball Rad %1").arg(doc->myCutter->GetBallRad()), QMessageBox::Ok, 0, 0);

    }
}


void ToolLibrary::slotSelectionChanged(int row, int /*col*/)
{
    // QMessageBox::information(0, tr("LoadTool"), tr("Row Number %1").arg(row), QMessageBox::Ok, 0, 0);
    QString typ = table1->text(row, 0);
    if(typ == "BALLNOSE")
	myViewer->itsType = BALLNOSE;
    if(typ == "SPHERICAL")
	myViewer->itsType = SPHERICAL;
    if(typ == "TORICAL")
	myViewer->itsType = TORICAL;
    if(typ == "CYLINDRICAL")
	myViewer->itsType = CYLINDRICAL;
    myViewer->itsBodyDia = (double) table1->text(row, 1).toDouble();
    myViewer->itsBallRad = (double) table1->text(row, 2).toDouble();
    myViewer->itsBodyLen = (double) table1->text(row, 3).toDouble();
    myViewer->itsHolderDia = (double) table1->text(row, 4).toDouble();
    myViewer->itsHolderLen = (double) table1->text(row, 5).toDouble();
    myViewer->itsShankDia = (double) table1->text(row, 6).toDouble();
    myViewer->itsShankLen = (double) table1->text(row, 7).toDouble();
    myViewer->itsTaperDiaA = (double) table1->text(row, 8).toDouble();
    myViewer->itsTaperDiaB = (double) table1->text(row, 9).toDouble();
    myViewer->itsTaperLen = (double) table1->text(row, 10).toDouble();
    loadTool();
    myViewer->updateView();

    //QMessageBox::information(0, tr("LoadTool"), tr("Tool Type %1").arg(table1->text(row, 0)), QMessageBox::Ok, 0, 0);
}


void ToolLibrary::destroy()
{
    if(myViewer)
	delete myViewer;
}


void ToolLibrary::slotAddNew()
{   
    ++toolCount;
    ToolTable tbl(this);
    tbl.toolNumber->setValue(toolCount);
    int res = tbl.exec();
    if(res == QDialog::Accepted)
    {
	int index = tbl.tabWidget2->currentPageIndex();
	myViewer->itsType = (CutterType)(index+1);
	myViewer->itsHolderDia = (double) tbl.holderDia->text().toDouble();
	myViewer->itsHolderLen = (double) tbl.holderLen->text().toDouble();
	myViewer->itsShankDia = (double) tbl.shankDia->text().toDouble();
	myViewer->itsShankLen = (double) tbl.shankLen->text().toDouble();
	myViewer->itsTaperDiaA = (double) tbl.taperDiaA->text().toDouble();
	myViewer->itsTaperDiaB = (double) tbl.taperDiaB->text().toDouble();
	myViewer->itsTaperLen = (double) tbl.taperLen->text().toDouble();
	myViewer->itsBodyDia = (double) tbl.bodyDia->text().toDouble();
	myViewer->itsBodyLen = (double) tbl.bodyLen->text().toDouble();
	myViewer->itsBallRad = (double) tbl.ballRad->text().toDouble();
	int toolNum = tbl.toolNumber->value() - 1; 
	table1->setText(toolNum, 1, tbl.bodyDia->text());
	table1->setText(toolNum, 2, tbl.ballRad->text());
	table1->setText(toolNum, 3, tbl.bodyLen->text());
	table1->setText(toolNum, 4, tbl.holderDia->text());
	table1->setText(toolNum, 5, tbl.holderLen->text());
	table1->setText(toolNum, 6, tbl.shankDia->text());
	table1->setText(toolNum, 7, tbl.shankLen->text());
	table1->setText(toolNum, 8, tbl.taperDiaA->text());
	table1->setText(toolNum, 9, tbl.taperDiaB->text());
	table1->setText(toolNum, 10, tbl.taperLen->text());
	switch (index)
	{
	    case 0:
		table1->setText(toolNum, 0,  "BALLNOSE");
		break;
	    case 1:
		table1->setText(toolNum, 0,  "SPHERICAL");
		break;
	    case 2:
		table1->setText(toolNum, 0,  "TORICAL");
		break;
	    case 3:
		table1->setText(toolNum, 0,  "CYLINDRICAL");
		break;
	    default:
		table1->setText(toolNum, 0,  "UNKNOWN");
		break;
	}
	table1->selectRow(toolNum);
    }
    else
	--toolCount;
    char env[200];
    strcpy(env, getenv("VRMTLIB"));
    QString pth(env);
    pth += "/cutter.dat";
    FILE* fp = fopen(pth, "w");
    if(fp)
    {
	unsigned i;
	fprintf(fp, "%d", toolCount);
	for(i = 0; i < toolCount; i++)
	{
	    QString str = table1->text(i, 0);
	    if(!str.isEmpty())
	    {
		fprintf(fp, "\n%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", (const char*)table1->text(i, 0),
			(double) table1->text(i, 1).toDouble(),
			(double) table1->text(i, 2).toDouble(),
			(double) table1->text(i, 3).toDouble(),
			(double) table1->text(i, 4).toDouble(),
			(double) table1->text(i, 5).toDouble(),
			(double) table1->text(i, 6).toDouble(),
			(double) table1->text(i, 7).toDouble(),
			(double) table1->text(i, 8).toDouble(),
			(double) table1->text(i, 9).toDouble(),
			(double) table1->text(i, 10).toDouble());
	    }
	}
	fclose(fp);
    }
    else
    {
	QMessageBox::critical(0, tr("LoadTool"), tr("File cutter.dat not found\nPlease set the environment variable VRMTLIB to correct location"), QMessageBox::Ok, 0, 0);
    }
}


void ToolLibrary::slotClose()
{
    QDialog::reject();
}


void ToolLibrary::slotHelp()
{
    qWarning("Not.....");
}


void ToolLibrary::show()
{ 
    QDialog::show();  
    table1->selectRow(1);
    table1->selectRow(0);
}
