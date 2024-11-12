// ListNodeOfNCBlock.cpp: implementation of the CListNodeOfNCBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "ListNodeOfNCBlock.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListNodeOfNCBlock::CListNodeOfNCBlock( const CNCBlock& P) : data(P), nextPtr(0)
{

}

CListNodeOfNCBlock::~CListNodeOfNCBlock()
{

}

CNCBlock CListNodeOfNCBlock::GetData() const
{
	return data;
}

bool CListNodeOfNCBlock::operator==(CListNodeOfNCBlock& rhs)
{
	if(*this==rhs)
		return true;
	return false;
}

bool CListNodeOfNCBlock::operator!=(CListNodeOfNCBlock& rhs)
{
	return !(*this==rhs);
}
