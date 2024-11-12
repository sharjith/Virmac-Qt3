/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void GCodeEditor::slotOpen()
{
	pathName = QFileDialog::getOpenFileName(
                    "/home",
                    "G Code Files (*.iso *.cnc) ;; Text Files (*.txt) ;; All Files (*.*)",
                    this,
                    "Open file dialog",
                    "Choose a file to open" );
	QFile file( pathName); 
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		textEdit1->setText( stream.read() );
	}
}


void GCodeEditor::slotSave()
{
	QFile file( pathName ); 
	if ( file.open( IO_WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << textEdit1->text();
		textEdit1->setModified( FALSE );
	}
}


void GCodeEditor::slotReload()
{
	VirmacView* view = (VirmacView*)parentWidget();
	VirmacDoc* doc = view->getDocument();
	VirmacApp* app = (VirmacApp*)qApp->mainWidget();
	if(doc->readGCode(pathName))
	{
		app->setMessage(tr("Loaded G Code File ") + pathName + tr(" For Simulation"));
		app->setPrompt(tr("Press Play To Start Simulation"));
		view->setCaption(pathName);
		view->GetView()->Refresh();
	}
}


void GCodeEditor::slotHelp()
{

}


void GCodeEditor::slotSaveAs()
{
	pathName = QFileDialog::getSaveFileName(
                    "/home",
                    "G Code Files (*.iso *.cnc) ;; Text Files (*.txt) ;; All Files (*.*)",
                    this,
                    "Save file dialog"
                    "Choose a filename to save under" );
	QFile file( pathName ); 
	if ( file.open( IO_WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << textEdit1->text();
		textEdit1->setModified( FALSE );
	}
}




void GCodeEditor::loadGCode()
{
	VirmacDoc* doc = ((VirmacView*)parentWidget())->getDocument();
	pathName = doc->pathName();
	QFile file( pathName); 
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		textEdit1->setText( stream.read() );
	}
}


void GCodeEditor::init()
{
	loadGCode();
	setFixedHeight(parentWidget()->height());
}




void GCodeEditor::slotClose()
{
	VirmacView* view = (VirmacView*)parentWidget();
	view->editorActive = false;
	close();
}
