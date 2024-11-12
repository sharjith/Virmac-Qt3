//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	OpenGL, C++ QT Surface Modeler Demo Application

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// ListNodeOfNCBlock.h: interface for the CListNodeOfNCBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LISTNODEOFNCBlock_H__0206868D_2E9C_40D2_AB3E_AD8990AA5941__INCLUDED_)
#define _LISTNODEOFNCBlock_H__0206868D_2E9C_40D2_AB3E_AD8990AA5941__INCLUDED_

#include "NCBlock.h"

class CNCBlock;
class CListOfNCBlock;

class CListNodeOfNCBlock
{
	friend class CListOfNCBlock;
	friend class CListIteratorOfListOfNCBlock;
public:
	CListNodeOfNCBlock(const CNCBlock&);
	CNCBlock GetData() const;
	virtual ~CListNodeOfNCBlock();
	bool operator==(CListNodeOfNCBlock& rhs);
	bool operator!=(CListNodeOfNCBlock& rhs);
private:
	CNCBlock data;
	CListNodeOfNCBlock* nextPtr;

};

#endif // !defined(_LISTNODEOFNCBlock_H__0206868D_2E9C_40D2_AB3E_AD8990AA5941__INCLUDED_)
