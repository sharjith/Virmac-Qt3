//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	StereoLithography Data File Reader

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// StLReader.h: interface for the CStLReader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __STLREADER_H__
#define __STLREADER_H__

#include "ListOfCPoint3D.h"
#include "ListIteratorOfListOfCPoint3D.h"

class CStLReader  
{
public:
	CStLReader(){pointList = new CListOfCPoint3D;}
	CStLReader(const char* fName);
	virtual ~CStLReader();
	CStLReader* Copy();
	bool IsFileValid() const { return fileIsValid; }
public:
	CListOfCPoint3D* pointList;
private:
	bool Read();
private:
	char* fileName;
	bool fileIsValid;
};

#endif
