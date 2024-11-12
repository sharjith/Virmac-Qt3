// ListOfNCBlock.cpp: implementation of the CListOfNCBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "ListOfNCBlock.h"
#include "ListException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOfNCBlock::CListOfNCBlock() : firstPtr(0), lastPtr(0)
{
}


CListOfNCBlock::~CListOfNCBlock()
{


	CListNodeOfNCBlock* curPtr = firstPtr, *tmp;
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


void CListOfNCBlock::Append(const CNCBlock& data)
{
	CListNodeOfNCBlock* newPtr = new CListNodeOfNCBlock(data);
	if(IsEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		lastPtr->nextPtr = newPtr;
		lastPtr = newPtr;
	}
}


void CListOfNCBlock::Prepend(const CNCBlock& data)
{
	CListNodeOfNCBlock* newPtr = new CListNodeOfNCBlock(data);
	if(IsEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		newPtr->nextPtr = firstPtr;
		firstPtr = newPtr;
	}
}

CNCBlock CListOfNCBlock::First() const
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return firstPtr->GetData();
}

CNCBlock& CListOfNCBlock::First()
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return firstPtr->data;
}

CNCBlock CListOfNCBlock::Last() const
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return lastPtr->GetData();
}

CNCBlock& CListOfNCBlock::Last()
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return lastPtr->data;
}

bool CListOfNCBlock::IsEmpty() const
{
	return (firstPtr==0);
}


void CListOfNCBlock::Clear()
{
	CListNodeOfNCBlock* curPtr = firstPtr, *tmp;
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

CListNodeOfNCBlock* CListOfNCBlock::NewNode(const CNCBlock& P)
{
	CListNodeOfNCBlock* newPtr= new CListNodeOfNCBlock(P);
	if(!newPtr)
		throw CListException(LIST_OUT_OF_MEMORY);
	newPtr->nextPtr = 0;
	return newPtr;
}
