// ListIteratorOfListOfNCBlock.cpp: implementation of the CListIteratorOfListOfNCBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "ListIteratorOfListOfNCBlock.h"
#include "ListException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListIteratorOfListOfNCBlock::CListIteratorOfListOfNCBlock(CListOfNCBlock* aList) : theList(aList)
{
}

CListIteratorOfListOfNCBlock::~CListIteratorOfListOfNCBlock()
{
}

void CListIteratorOfListOfNCBlock::SetList(CListOfNCBlock* aList)
{
	theList = aList;
}

void CListIteratorOfListOfNCBlock::Init()
{
	curPtr = theList->firstPtr;
}

bool CListIteratorOfListOfNCBlock::More() const
{
	return (curPtr != 0);
}

void CListIteratorOfListOfNCBlock::Next()
{
	curPtr = (curPtr) ? (curPtr->nextPtr) : 0;
}

CNCBlock CListIteratorOfListOfNCBlock::Current() const
{
	return (curPtr != NULL) ? curPtr->data : CNCBlock();
}

CNCBlock CListIteratorOfListOfNCBlock::ValueAt(const int index)
{
	int cnt =0;
	Init();
	while(curPtr != 0)
	{
		Next();
		cnt++;
	}
	
	if(cnt < index)
		throw CListException(LIST_BOUND_UPPER);
		
	else if(index<1)
		throw CListException(LIST_BOUND_LOWER);
		
	/*else*/ if(index ==1 )
		return theList->firstPtr->data;
	else
	{
		Init();
		for(int i=1; i < index; i++)
		{
			if(curPtr->nextPtr == 0)
				break;
			Next();
		}
		
		return curPtr->data;
	}
		
}

