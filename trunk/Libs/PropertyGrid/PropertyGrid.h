#pragma once

#define PROPERTY_GRID_VERSION	"1.0.0.0"

#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#include "afxcmn.h"
#include "afxtempl.h"

#include "..\Utils\Utils.h"

//#include "resource.h"
#include "Inputer.h"

#include "PropertyConfigDialog.h"


// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CPropertyGrid 包装类
#define WM_PROPERTY_GRID_EDIT_START		WM_USER+452
#define WM_PROPERTY_GRID_EDIT_END		WM_USER+453
#define WM_PROPERTY_GRID_VALUE_CHANGED	WM_USER+454


namespace PropertyGrid
{
enum OLEDRAGCONSTANTS
{
    flexOLEDragManual = 0,
    flexOLEDragAutomatic = 1
};
enum OLEDROPCONSTANTS
{
    flexOLEDropNone = 0,
    flexOLEDropManual = 1
};
enum DRAGOVERCONSTANTS
{
    flexEnter = 0,
    flexLeave = 1,
    flexOver = 2
};
enum CLIPBOARDCONSTANTS
{
    flexCFText = 1,
    flexCFBitmap = 2,
    flexCFMetafile = 3,
    flexCFDIB = 8,
    flexCFPalette = 9,
    flexCFEMetafile = 14,
    flexCFFiles = 15,
    flexCFRTF = 0xffffbf01
};
enum OLEDROPEFFECTCONSTANTS
{
    flexOLEDropEffectNone = 0,
    flexOLEDropEffectCopy = 1,
    flexOLEDropEffectMove = 2,
    flexOLEDropEffectScroll = 0x80000000
};
enum ERRORCONSTANTS
{
    flexerrIllegaFunctionCall = 5,
    flexerrObjIllegalUse = 425,
    flexerrClipInvalidFormat = 461,
    flexerrDataObjectLocked = 672,
    flexerrExpectedAnArgument = 673,
    flexerrRecursiveOLEDrag = 674,
    flexerrUserFormatNotBinArray = 675,
    flexerrDataNotSetForFormat = 676,
    flexerrUnknownError = 600,
    flexerrSubscript = 381,
    flexerrBadValue = 380,
    flexerrGetNotSupported = 394,
    flexerrSetNotPermitted = 387,
    flexerrOutOfMemory = 7,
    flexerrVB30000 = 30000,
    flexerrVB30001 = 30001,
    flexerrVB30002 = 30002,
    flexerrVB30004 = 30004,
    flexerrVB30005 = 30005,
    flexerrVB30006 = 30006,
    flexerrVB30008 = 30008,
    flexerrVB30009 = 30009,
    flexerrVB30010 = 30010,
    flexerrVB30011 = 30011,
    flexerrVB30013 = 30013,
    flexerrVB30014 = 30014,
    flexerrVB30015 = 30015,
    flexerrVB30016 = 30016,
    flexerrVB30017 = 30017
};
enum APPEARANCESETTINGS
{
    flexFlat = 0,
    flex3D = 1
};
enum BORDERSTYLESETTINGS
{
    flexBorderNone = 0,
    flexBorderSingle = 1
};
enum FOCUSRECTSETTINGS
{
    flexFocusNone = 0,
    flexFocusLight = 1,
    flexFocusHeavy = 2
};
enum HIGHLIGHTSETTINGS
{
    flexHighlightNever = 0,
    flexHighlightAlways = 1,
    flexHighlightWithFocus = 2
};
enum SCROLLBARSSETTINGS
{
    flexScrollBarNone = 0,
    flexScrollBarHorizontal = 1,
    flexScrollBarVertical = 2,
    flexScrollBarBoth = 3
};
enum TEXTSTYLESETTINGS
{
    flexTextFlat = 0,
    flexTextRaised = 1,
    flexTextInset = 2,
    flexTextRaisedLight = 3,
    flexTextInsetLight = 4
};
enum FILLSTYLESETTINGS
{
    flexFillSingle = 0,
    flexFillRepeat = 1
};
enum GRIDLINESETTINGS
{
    flexGridNone = 0,
    flexGridFlat = 1,
    flexGridInset = 2,
    flexGridRaised = 3
};
enum SELECTIONMODESETTINGS
{
    flexSelectionFree = 0,
    flexSelectionByRow = 1,
    flexSelectionByColumn = 2
};
enum MERGECELLSSETTINGS
{
    flexMergeNever = 0,
    flexMergeFree = 1,
    flexMergeRestrictRows = 2,
    flexMergeRestrictColumns = 3,
    flexMergeRestrictAll = 4
};
enum PICTURETYPESETTINGS
{
    flexPictureColor = 0,
    flexPictureMonochrome = 1
};
enum ALLOWUSERRESIZESETTINGS
{
    flexResizeNone = 0,
    flexResizeColumns = 1,
    flexResizeRows = 2,
    flexResizeBoth = 3
};
enum MOUSEPOINTERSETTINGS
{
    flexDefault = 0,
    flexArrow = 1,
    flexCross = 2,
    flexIBeam = 3,
    flexIcon = 4,
    flexSize = 5,
    flexSizeNESW = 6,
    flexSizeNS = 7,
    flexSizeNWSE = 8,
    flexSizeEW = 9,
    flexUpArrow = 10,
    flexHourglass = 11,
    flexNoDrop = 12,
    flexArrowHourGlass = 13,
    flexArrowQuestion = 14,
    flexSizeAll = 15,
    flexCustom = 99
};
enum SORTSETTINGS
{
    flexSortNone = 0,
    flexSortGenericAscending = 1,
    flexSortGenericDescending = 2,
    flexSortNumericAscending = 3,
    flexSortNumericDescending = 4,
    flexSortStringNoCaseAscending = 5,
    flexSortStringNoCaseDescending = 6,
    flexSortStringAscending = 7,
    flexSortStringDescending = 8
};
enum ALIGNMENTSETTINGS
{
    flexAlignLeftTop = 0,
    flexAlignLeftCenter = 1,
    flexAlignLeftBottom = 2,
    flexAlignCenterTop = 3,
    flexAlignCenterCenter = 4,
    flexAlignCenterBottom = 5,
    flexAlignRightTop = 6,
    flexAlignRightCenter = 7,
    flexAlignRightBottom = 8,
    flexAlignGeneral = 9
};

enum PROPERTY_TYPE
{	
	PROT_NONE,
	PROT_EDIT,
	PROT_NUMBER,
	PROT_SPIN_NUMBER,	
	PROT_COMBO,
	PROT_BOOL,
	PROT_COLOR,
	PROT_FONT,
	PROT_COLOREX,
	PROT_DIRECT,
	PROT_PATH,
	PROT_CUSTOM,
};
class CPropertyValue
{
protected:
	CString		m_Value;
public:
	CPropertyValue(){}
	CPropertyValue(CPropertyValue& val)
	{
		m_Value=val.m_Value;
	}
	CPropertyValue(CString& val)
	{
		m_Value=val;
	}
	CPropertyValue(LPCTSTR val)
	{
		m_Value=val;
	}
	CPropertyValue(int val)
	{
		m_Value.Format("%d",val);
	}
	CPropertyValue(UINT val)
	{
		m_Value.Format("%d",val);
	}
	CPropertyValue(long val)
	{
		m_Value.Format("%d",val);
	}
	CPropertyValue(double val)
	{
		m_Value.Format("%g",val);
	}
	CPropertyValue(float val)
	{
		m_Value.Format("%g",val);
	}
	CPropertyValue(bool val)
	{
		val?m_Value="是":m_Value="否";
	}

	CString& GetString()
	{
		return m_Value;
	}
	operator CString()
	{
		return m_Value;
	}
	operator LPCTSTR()
	{
		return (LPCTSTR)m_Value;
	}
	operator int()
	{
		return atoi((LPCTSTR)m_Value);
	}
	operator UINT()
	{
		return atoi((LPCTSTR)m_Value);
	}
	operator long()
	{
		return atoi((LPCTSTR)m_Value);
	}
	operator __int64()
	{
		return _atoi64((LPCTSTR)m_Value);		
	}
	operator unsigned __int64()
	{
		return _atoi64((LPCTSTR)m_Value);		
	}
	operator double()
	{
		return atof((LPCTSTR)m_Value);
	}
	operator float()
	{
		return (float)atof((LPCTSTR)m_Value);
	}
	operator bool()
	{
		return m_Value=="是";
	}
	CPropertyValue& operator=(const CString& val)
	{
		m_Value=val;
		return *this;
	}
	CPropertyValue& operator=(const LPCTSTR val)
	{
		m_Value=val;
		return *this;
	}
	CPropertyValue& operator=(const int val)
	{
		m_Value.Format("%d",val);
		return *this;
	}
	CPropertyValue& operator=(const UINT val)
	{
		m_Value.Format("%d",val);
		return *this;
	}
	CPropertyValue& operator=(const long val)
	{
		m_Value.Format("%d",val);
		return *this;
	}
	CPropertyValue& operator=(const __int64 val)
	{
		m_Value.Format("%I64d",val);
		return *this;
	}
	CPropertyValue& operator=(const unsigned __int64 val)
	{
		m_Value.Format("%I64u",val);
		return *this;
	}
	CPropertyValue& operator=(const double val)
	{
		m_Value.Format("%g",val);
		return *this;
	}

	CPropertyValue& operator=(const float val)
	{
		m_Value.Format("%g",val);
		return *this;
	}
	CPropertyValue& operator=(const bool val)
	{
		val?m_Value="是":m_Value="否";
		return *this;
	}
	
};

struct PROPERTY_INFO
{	
	int							PropertyType;	
	CString						Define;
	CPropertyConfigDialog *		pConfigDialog;
	CString						Name;
	int							ID;
	CPropertyValue				Value;
	int							BindRow;
	PROPERTY_INFO()
	{
		PropertyType=0;
		pConfigDialog=NULL;
		ID=0;
		BindRow=0;
	}
	PROPERTY_INFO(const PROPERTY_INFO& Info)
	{
		PropertyType=Info.PropertyType;
		Define=Info.Define;
		pConfigDialog=Info.pConfigDialog;
		Name=Info.Name;
		ID=Info.ID;
		Value=Info.Value;
		BindRow=Info.BindRow;
	}
};

struct PROPERTY_CHANGE_INFO
{
	CPropertyValue	OldValue;
	CPropertyValue	NewValue;
	PROPERTY_INFO * pInfo;
};



class CPropertyGrid : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPropertyGrid)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6262D3A0, 0x531B, 0x11CF, { 0x91, 0xF6, 0xC2, 0x86, 0x3C, 0x38, 0x5E, 0x30 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		if(!CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID))
			return false;
		Init();
		return true;
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		if(!CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
			pPersist, bStorage, bstrLicKey))
			return false;
		Init();
		return true;
	}

	void Init();

// 属性
public:


// 操作
public:

// IMSFlexGrid

// Functions
//

	long GetRows()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetRows(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetCols()
	{
		long result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetCols(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetFixedRows()
	{
		long result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetFixedRows(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetFixedCols()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetFixedCols(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short GetVersion()
	{
		short result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	CString GetFormatString()
	{
		CString result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetFormatString(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetTopRow()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetTopRow(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetLeftCol()
	{
		long result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetLeftCol(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetRow()
	{
		long result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetRow(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetCol()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetCol(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetRowSel()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetRowSel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetColSel()
	{
		long result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetColSel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString GetText()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetBackColor()
	{
		unsigned long result;
		InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetBackColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetForeColor()
	{
		unsigned long result;
		InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetForeColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetBackColorFixed()
	{
		unsigned long result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetBackColorFixed(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetForeColorFixed()
	{
		unsigned long result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetForeColorFixed(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetBackColorSel()
	{
		unsigned long result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetBackColorSel(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetForeColorSel()
	{
		unsigned long result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetForeColorSel(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetBackColorBkg()
	{
		unsigned long result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetBackColorBkg(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetWordWrap()
	{
		BOOL result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetWordWrap(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH GetFont()
	{
		LPDISPATCH result;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_Font(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	float GetFontWidth()
	{
		float result;
		InvokeHelper(0x54, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void SetFontWidth(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x54, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString GetCellFontName()
	{
		CString result;
		InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetCellFontName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float GetCellFontSize()
	{
		float result;
		InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void SetCellFontSize(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetCellFontBold()
	{
		BOOL result;
		InvokeHelper(0x4f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetCellFontBold(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetCellFontItalic()
	{
		BOOL result;
		InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetCellFontItalic(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x50, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetCellFontUnderline()
	{
		BOOL result;
		InvokeHelper(0x51, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetCellFontUnderline(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x51, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetCellFontStrikeThrough()
	{
		BOOL result;
		InvokeHelper(0x52, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetCellFontStrikeThrough(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x52, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float GetCellFontWidth()
	{
		float result;
		InvokeHelper(0x53, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void SetCellFontWidth(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x53, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetTextStyle()
	{
		long result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetTextStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetTextStyleFixed()
	{
		long result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetTextStyleFixed(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetScrollTrack()
	{
		BOOL result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetScrollTrack(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetFocusRect()
	{
		long result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetFocusRect(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetHighLight()
	{
		long result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetHighLight(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetRedraw()
	{
		BOOL result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetRedraw(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetScrollBars()
	{
		long result;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetScrollBars(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetMouseRow()
	{
		long result;
		InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetMouseCol()
	{
		long result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetCellLeft()
	{
		long result;
		InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetCellTop()
	{
		long result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetCellWidth()
	{
		long result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetCellHeight()
	{
		long result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetRowHeightMin()
	{
		long result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetRowHeightMin(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetFillStyle()
	{
		long result;
		InvokeHelper(DISPID_FILLSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetFillStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(DISPID_FILLSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetGridLines()
	{
		long result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetGridLines(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetGridLinesFixed()
	{
		long result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetGridLinesFixed(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetGridColor()
	{
		unsigned long result;
		InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetGridColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetGridColorFixed()
	{
		unsigned long result;
		InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetGridColorFixed(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetCellBackColor()
	{
		unsigned long result;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetCellBackColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long GetCellForeColor()
	{
		unsigned long result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetCellForeColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short GetCellAlignment()
	{
		short result;
		InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetCellAlignment(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetCellTextStyle()
	{
		long result;
		InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetCellTextStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short GetCellPictureAlignment()
	{
		short result;
		InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetCellPictureAlignment(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString GetClip()
	{
		CString result;
		InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetClip(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetSort(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetSelectionMode()
	{
		long result;
		InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetSelectionMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetMergeCells()
	{
		long result;
		InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetMergeCells(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetAllowBigSelection()
	{
		BOOL result;
		InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetAllowBigSelection(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x33, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetAllowUserResizing()
	{
		long result;
		InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetAllowUserResizing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetBorderStyle()
	{
		long result;
		InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetBorderStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GethWnd()
	{
		long result;
		InvokeHelper(DISPID_HWND, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL GetEnabled()
	{
		BOOL result;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetEnabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetAppearance()
	{
		long result;
		InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetAppearance(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetMousePointer()
	{
		long result;
		InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetMousePointer(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x35, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH GetMouseIcon()
	{
		LPDISPATCH result;
		InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_MouseIcon(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x36, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	long GetPictureType()
	{
		long result;
		InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetPictureType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH GetPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetCellPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_CellPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x2a, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString GetTextArray(long index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, index);
		return result;
	}
	void SetTextArray(long index, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	short GetColAlignment(long index)
	{
		short result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms, index);
		return result;
	}
	void SetColAlignment(long index, short newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x38, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	long GetColWidth(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, index);
		return result;
	}
	void SetColWidth(long index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	long GetRowHeight(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, index);
		return result;
	}
	void SetRowHeight(long index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	BOOL GetMergeRow(long index)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, index);
		return result;
	}
	void SetMergeRow(long index, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x3b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	BOOL GetMergeCol(long index)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, index);
		return result;
	}
	void SetMergeCol(long index, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	void SetRowPosition(long index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	void SetColPosition(long index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	long GetRowData(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, index);
		return result;
	}
	void SetRowData(long index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	long GetColData(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, index);
		return result;
	}
	void SetColData(long index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	CString GetTextMatrix(long Row, long Col)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Row, Col);
		return result;
	}
	void SetTextMatrix(long Row, long Col, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, Col, newValue);
	}
	void AddItem(LPCTSTR Item, VARIANT index)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Item, &index);
	}
	void RemoveItem(long index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index);
	}
	void Clear()
	{
		m_PropertyList.RemoveAll();
		InvokeHelper(0x44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
		SetRows(2);
		SetCols(2);
		SetFixedRows(1);
		SetFixedCols(1);
		SetRow(0);
		SetCol(0);
		SetText("属性");
		SetCellAlignment(flexAlignCenterCenter);
		SetCol(1);
		SetText("值");
		SetCellAlignment(flexAlignCenterCenter);
		SetRows(1);
	}
	void Refresh()
	{
		InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH GetDataSource()
	{
		LPDISPATCH result;
		InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SetDataSource(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetRowIsVisible(long index)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x55, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, index);
		return result;
	}
	BOOL GetColIsVisible(long index)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x56, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, index);
		return result;
	}
	long GetRowPos(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, index);
		return result;
	}
	long GetColPos(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, index);
		return result;
	}
	short GetGridLineWidth()
	{
		short result;
		InvokeHelper(0x59, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetGridLineWidth(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short GetFixedAlignment(long index)
	{
		short result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms, index);
		return result;
	}
	void SetFixedAlignment(long index, short newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x5a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, index, newValue);
	}
	CString GetFontName()
	{
		CString result;
		InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetFontName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float GetFontSize()
	{
		float result;
		InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void SetFontSize(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetFontBold()
	{
		BOOL result;
		InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetFontBold(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetFontItalic()
	{
		BOOL result;
		InvokeHelper(0x5e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetFontItalic(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetFontStrikethru()
	{
		BOOL result;
		InvokeHelper(0x5f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetFontStrikethru(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetFontUnderline()
	{
		BOOL result;
		InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetFontUnderline(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL GetRightToLeft()
	{
		BOOL result;
		InvokeHelper(0xfffffd9d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetRightToLeft(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xfffffd9d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetOLEDropMode()
	{
		long result;
		InvokeHelper(0x60f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetOLEDropMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void OLEDrag()
	{
		InvokeHelper(0x610, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//
protected:
	CArray<PROPERTY_INFO,PROPERTY_INFO&>	m_PropertyList;
	CInputer								m_Inputer;	
	int										m_CurEditRow;
	int										m_CurEditCol;
public:

	void AddProperty(PROPERTY_INFO * pInfo);
	void SetProperty(PROPERTY_INFO * pInfo,bool AddWhileNotExist=true);
	void SetPropertyValue(int Index,CPropertyValue Value);
	void SetPropertyValueByID(int ID,CPropertyValue Value);
	void SetPropertyValue(LPCTSTR Name,CPropertyValue Value);
	PROPERTY_INFO * GetProperty(int index)
	{
		if(index>=0&&index<m_PropertyList.GetCount())
		{
			return &(m_PropertyList[index]);
		}
		return NULL;
	}
	PROPERTY_INFO * GetPropertyByName(LPCTSTR Name);
	PROPERTY_INFO * GetPropertyByID(int ID);	
	void OnFinishEdit(bool IsChanged);

protected:	

	DECLARE_EVENTSINK_MAP()
	void OnClickMsFlexGrid();	

};
}

#include "PropertyView.h"

using namespace PropertyGrid;