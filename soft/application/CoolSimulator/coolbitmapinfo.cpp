// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "coolbitmapinfo.h"

// Dispatch interfaces referenced by this interface
#include "coolbitmapinfoheader.h"
#include "coolrgbquad.h"


/////////////////////////////////////////////////////////////////////////////
// CCoolBitmapInfo properties

CCoolBitmapInfoHeader CCoolBitmapInfo::GetBmiHeader()
{
	LPDISPATCH pDispatch;
	GetProperty(0x1, VT_DISPATCH, (void*)&pDispatch);
	return CCoolBitmapInfoHeader(pDispatch);
}

void CCoolBitmapInfo::SetBmiHeader(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CCoolRGBQuad CCoolBitmapInfo::GetBmiColors()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2, VT_DISPATCH, (void*)&pDispatch);
	return CCoolRGBQuad(pDispatch);
}

void CCoolBitmapInfo::SetBmiColors(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CCoolBitmapInfo operations