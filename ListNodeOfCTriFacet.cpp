// ListNodeOfCTriFacet.cpp: implementation of the CListNodeOfCTriFacet class.
//
//////////////////////////////////////////////////////////////////////

#include "ListNodeOfCTriFacet.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListNodeOfCTriFacet::CListNodeOfCTriFacet( const CTriFacet& P) : data(P), nextPtr(0)
{

}

CListNodeOfCTriFacet::~CListNodeOfCTriFacet()
{

}

CTriFacet CListNodeOfCTriFacet::GetData() const
{
	return data;
}

bool CListNodeOfCTriFacet::operator==(CListNodeOfCTriFacet& rhs)
{
	if(*this==rhs)
		return true;
	return false;
}

bool CListNodeOfCTriFacet::operator!=(CListNodeOfCTriFacet& rhs)
{
	return !(*this==rhs);
}
