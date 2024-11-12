// StLReader.cpp: implementation of the CStLReader class.
//
//////////////////////////////////////////////////////////////////////

#include "Virmac.h"
#include "StLReader.h"

#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStLReader::CStLReader(const char* fName)
{
    fileIsValid = false;
    fileName = new char[strlen(fName)];
    strcpy(fileName, fName);
    pointList = new CListOfCPoint3D;
    if(Read())
	fileIsValid = true;
}

CStLReader::~CStLReader()
{
    pointList->Clear();
    delete pointList;
    delete [] fileName;
}

CStLReader* CStLReader::Copy()
{
    CStLReader* R = new CStLReader();
    R->fileName = new char[strlen(fileName)];
    strcpy(R->fileName, fileName);
    R->fileIsValid = fileIsValid;
    CListIteratorOfListOfCPoint3D it(pointList);
    for(it.Init(); it.More(); it.Next())
	R->pointList->Append(it.Current());
    return R;
}

bool CStLReader::Read()
{
    FILE* stlfp;
    int i=0,j=0,cnt=0 ,pCnt=4;
    char a[100];
    char str[40];
    double x=0,y=0,z=0;
    CPoint3D tPoint;
    int bCnt = 0;
    int totBlk = 0;
    QString pCent;
    QStatusBar* pBar = ((VirmacApp*)qApp->mainWidget())->statusBar();
    QProgressBar* pProg = ((VirmacApp*)qApp->mainWidget())->progBar;

    //for progress bar step
    FILE* infile = fopen(fileName, "r");
    while(!feof(infile))
    {
	fgets(a ,100, infile);
	totBlk++;
    }
    pProg->setTotalSteps(totBlk);
    fclose(infile);


    stlfp = fopen(fileName,"r");
    if(!stlfp)
	return false;
    else
    {
	fscanf(stlfp,"%s",a);
	if (strcmp(a,"solid") != 0)
	{
	    if(QMessageBox::critical(0, "Expected \"solid\" keyword", "StL Format Error", QMessageBox::Ok, 0, 0) == QMessageBox::Ok)
		return false;
	}
	do
	{
	    bCnt++;
	    i=0;
	    cnt=0;
	    fgets(a,100,stlfp);
	    while(a[i]!='\0')
	    {
		if(!islower((int)a[i]) && !isupper((int)a[i]) && a[i]!=' ')
		    break;
		cnt++;
		i++;
	    }
	    while(a[cnt]!='\0')
	    {
		str[j]=a[cnt];
		cnt++;
		j++;
	    }
	    str[j]='\0';
	    j=0;
	    if(sscanf(str,"%lf%lf%lf",&x,&y,&z)==3)
	    {
		tPoint.SetParam(x,y,z);
		pointList->Append(tPoint);
		pCnt++;
	    }
	    if(bCnt%100 == 0)				
		pProg->setProgress(bCnt, totBlk);
	}while(!feof(stlfp));
	fclose(stlfp);
	pCent.sprintf("%d Triangles Processed", (pCnt/4));
	pBar->message(pCent);
	pProg->setProgress(bCnt, totBlk);
	pProg->reset();
	return true;
    }
}

