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

// ListOfNCBlock.h: interface for the CListOfNCBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LISTOFNCBlock_H__64FE1368_B1AF_44CF_8FFF_9382221176C2__INCLUDED_)
#define _LISTOFNCBlock_H__64FE1368_B1AF_44CF_8FFF_9382221176C2__INCLUDED_

#include "ListNodeOfNCBlock.h"

class CNCBlock;
class CListIteratorOfListOfNCBlock;

class CListOfNCBlock
{
	friend class CListIteratorOfListOfNCBlock;
public:
	CListOfNCBlock();
	virtual ~CListOfNCBlock();
	void Append(const CNCBlock& data);
	void Prepend(const CNCBlock& data);
	CNCBlock First() const;
    CNCBlock& First();
	CNCBlock Last() const;
    CNCBlock& Last();
	bool IsEmpty() const;
	void Clear();
private:
	CListNodeOfNCBlock* firstPtr;
	CListNodeOfNCBlock* lastPtr;
private:
	CListNodeOfNCBlock* NewNode(const CNCBlock&);
};

#endif // !defined(_LISTOFNCBlock_H__64FE1368_B1AF_44CF_8FFF_9382221176C2__INCLUDED_)
