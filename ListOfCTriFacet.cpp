// ListOfCTriFacet.cpp: implementation of the CListOfCTriFacet class.
//
//////////////////////////////////////////////////////////////////////

#include "ListOfCTriFacet.h"
#include "ListException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOfCTriFacet::CListOfCTriFacet() : firstPtr(0), lastPtr(0)
{
}


CListOfCTriFacet::~CListOfCTriFacet()
{


	CListNodeOfCTriFacet* curPtr = firstPtr, *tmp;
	if(!IsEmpty())
	{
		while(curPtr != 0)
		{
			tmp = curPtr;
			curPtr = curPtr->nextPtr;
			delete tmp;
		}
	}
}


void CListOfCTriFacet::Append(const CTriFacet& data)
{
	CListNodeOfCTriFacet* newPtr = new CListNodeOfCTriFacet(data);
	if(IsEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		lastPtr->nextPtr = newPtr;
		lastPtr = newPtr;
	}
}


void CListOfCTriFacet::Prepend(const CTriFacet& data)
{
	CListNodeOfCTriFacet* newPtr = new CListNodeOfCTriFacet(data);
	if(IsEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		newPtr->nextPtr = firstPtr;
		firstPtr = newPtr;
	}
}

CTriFacet CListOfCTriFacet::First() const
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return firstPtr->GetData();
}

CTriFacet CListOfCTriFacet::Last() const
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return lastPtr->GetData();
}

bool CListOfCTriFacet::IsEmpty() const
{
	return (firstPtr==0);
}		


void CListOfCTriFacet::Clear()
{
	CListNodeOfCTriFacet* curPtr = firstPtr, *tmp;
	if(!IsEmpty())
	{
		while(curPtr != 0)
		{
			tmp = curPtr;
			curPtr = curPtr->nextPtr;
			delete tmp;
		}
		
		firstPtr = lastPtr = 0;
	}
}

CListNodeOfCTriFacet* CListOfCTriFacet::NewNode(const CTriFacet& P)
{
	CListNodeOfCTriFacet* newPtr= new CListNodeOfCTriFacet(P);
	if(!newPtr)
		throw CListException(LIST_OUT_OF_MEMORY);
	newPtr->nextPtr = 0;
	return newPtr;
}
