#include "StdAfx.h"
#include "PatternProcessor.h"
#include "Markup.h"
#include <atlimage.h>
//#import "msxml3.dll"
//using namespace MSXML2;

CPatternProcessor::CPatternProcessor(void)
{
	m_vPatterns.reserve(10000);
	m_iBaseW = 100;
	m_iBaseH = 100;
	m_iSpace = 2;
}

CPatternProcessor::~CPatternProcessor(void)
{
	m_vPatterns.clear();
}

BOOL CPatternProcessor::ReadPatternXML(CString strXMLPath)
{
	if (strXMLPath.IsEmpty())
	{
		return FALSE;
	}
	/*MSXML2::IXMLDOMDocumentPtr xmlDocPtr = NULL;   
	MSXML2::IXMLDOMElementPtr xmlRootPtr = NULL;  
	MSXML2::IXMLDOMElementPtr xmlElementPtr = NULL;  
	MSXML2::IXMLDOMNodeListPtr xmlNodesPtr = NULL;  
	MSXML2::IXMLDOMNamedNodeMapPtr xmlNameAttr = NULL;  
	MSXML2::IXMLDOMNodePtr xmlNode = NULL;  
	HRESULT HR = xmlDocPtr.CreateInstance(_uuidof(MSXML2::DOMDocument30));  
	if(!SUCCEEDED(HR))  
	{  
		return FALSE;  
	}  
	xmlDocPtr->load(strXMLPath.AllocSysString());  
	xmlNodesPtr = xmlDocPtr->selectNodes(_T("//lg:PatternTable/lg:Pattern"));
	TPattern pattern = {0};

	for (int i=0; i<xmlNodesPtr->Getlength(); i++)
	{
		xmlNode = xmlNodesPtr->Getitem(i);
		CString strNodeName = xmlNode->GetnodeName();
		MSXML2::IXMLDOMNodeListPtr xmlChildNodesPtr = xmlNode->GetchildNodes(); 
		if (xmlChildNodesPtr!=NULL)
		{
			for (int j=0; j<xmlChildNodesPtr->Getlength();j++)
			{
				MSXML2::IXMLDOMNodePtr xmlChildNode = xmlChildNodesPtr->Getitem(j);
				CString xmlChildNodeName = xmlChildNode->GetnodeName();

				if (xmlChildNodeName==_T("lg:Height"))
				{
					CString xmlHeightValue = xmlChildNode->Gettext();
					pattern.height = _ttoi(xmlHeightValue);
				}

				if (xmlChildNodeName==_T("lg:Width"))
				{
					CString xmlWidthValue = xmlChildNode->Gettext();
					pattern.width = _ttoi(xmlWidthValue);
				}

				if (xmlChildNodeName==_T("lg:BitPattern"))
				{
					CString xmlPatternValue = xmlChildNode->Gettext();
					pattern.strBase64 = xmlPatternValue;
				}
			}
			if (pattern.height>0&&pattern.width>0&&!pattern.strBase64.IsEmpty())
			{
				m_vPatterns.push_back(pattern);
			}
		}
	}*/

	CMarkup xmlPattern;   
	BOOL rs = xmlPattern.Load(strXMLPath);  
	if(!rs)
		return FALSE;

	BOOL bFind = true;   
	xmlPattern.ResetMainPos();   
	xmlPattern.FindElem();
	xmlPattern.IntoElem();  

	TPattern pattern = {0};
	while (xmlPattern.FindElem(_T("lg:Pattern")))   
	{   
		if(xmlPattern.FindChildElem(_T("lg:Height")))
		{
			pattern.height = _ttoi(xmlPattern.GetChildData());
			xmlPattern.ResetChildPos();
		}

		if(xmlPattern.FindChildElem(_T("lg:Width")))
		{
			pattern.width = _ttoi(xmlPattern.GetChildData());
			xmlPattern.ResetChildPos();
		}
		if(xmlPattern.FindChildElem(_T("lg:BitPattern")))
		{
			pattern.strBase64 = xmlPattern.GetChildData();
			xmlPattern.ResetChildPos();
		}

		if (pattern.height>0&&pattern.width>0&&!pattern.strBase64.IsEmpty())
		{
			m_vPatterns.push_back(pattern);
		}
	}    

	return TRUE;
}

void CPatternProcessor::Draw(CDC& dc)
{
	int iTotalHeight = m_vPatterns.size()*(m_iBaseH + m_iSpace);
	CRect rt(0,0,m_iBaseW,m_iBaseH);
	for (size_t i=0; i<m_vPatterns.size();i++)
	{
		rt.left = 0;
		rt.top = i*(m_iBaseH+m_iSpace);
		rt.right = rt.left+m_iBaseW;
		rt.bottom = rt.top + m_iBaseH;
		DrawPattern(dc,m_vPatterns[147],rt);
		CString strIdx;
		strIdx.Format(_T("%03d"),i);
		dc.TextOut(rt.right + 10,rt.top, strIdx);
	}
}

void CPatternProcessor::Save(CDC& dc, CString strImgPath)
{
	if (strImgPath.IsEmpty())
	{
		::AfxMessageBox(_T("unable to save!"));
		return;
	}

	int iTotalHeight = m_vPatterns.size()*(m_iBaseH + m_iSpace);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(&dc,m_iBaseW, iTotalHeight);
	memDC.SelectObject(&memBmp);
	CRect rt(0,0,m_iBaseW,m_iBaseH);
	for (size_t i=0; i<m_vPatterns.size();i++)
	{
		rt.left = 0;
		rt.top = i*(m_iBaseH+m_iSpace);
		rt.right = rt.left+m_iBaseW;
		rt.bottom = rt.top + m_iBaseH;
		DrawPattern(memDC,m_vPatterns[i],rt);
	}
	dc.BitBlt(0, 0, m_iBaseW, iTotalHeight, &memDC, 0, 0, SRCCOPY);
	
	CImage img;
	img.Attach(memBmp);
	img.Save(strImgPath);
	AfxMessageBox(strImgPath);

	memBmp.DeleteObject();
	memDC.DeleteDC();
}

void CPatternProcessor::DrawPattern(CDC& dc, const TPattern& pat, const CRect& rtPos)
{
	HBITMAP hBmp = CreatePattern(pat.strBase64,pat.width, pat.height);
	if (hBmp)
	{
		CBitmap bmp;
		bmp.Attach(hBmp);
		CBrush bmpBrush(&bmp);
		dc.FillRect(rtPos, &bmpBrush);
	}
	else
	{
		CBrush greenBrush;
		greenBrush.CreateSolidBrush(RGB(0,255,0));
		dc.FillRect(rtPos, &greenBrush);
	}
}

int CPatternProcessor::GetFillWidth() const
{
	return m_iBaseW;
}

int CPatternProcessor::GetTotalHeight() const
{
	return (m_iBaseH + m_iSpace)*m_vPatterns.size();
}

size_t CPatternProcessor::GetPatternCount() const
{
	return m_vPatterns.size();
}

HBITMAP CPatternProcessor::CreatePattern(CString strBase64, int width, int height)
{
	strBase64.Remove(_T('\r'));
	strBase64.Remove(_T('\n'));
	strBase64.Remove(_T(' '));
	
	HBITMAP hBitmap = NULL;
	BYTE* pBits = NULL;
	LONG iOutLen = 0;
	int res = DecodeBase64( strBase64, &pBits);

	if (res==0)
	{
		return NULL;
	}

	hBitmap = ::CreateBitmap( width, height, 1, 1, pBits );
	delete [] pBits;

	return hBitmap;
}

int CPatternProcessor::DecodeBase64(CString strBase64, BYTE** pOutBits)
{
	LONG iInLen = strBase64.GetLength();
	if( iInLen <= 0 )
		return 0;

	// allocate output
	LONG iLen = ( ( iInLen % 4 ) == 0 ) ? iInLen : ( ( iInLen / 4 ) + 1 ) * 4;
	LONG outLength = iLen * 3 / 4;
	*pOutBits = new BYTE[ outLength ];
	if( *pOutBits == NULL )
		return 0;

	// decode
	LONG j, e, d, i;
	ULONG iResult = 0;
	ULONG iMask = 0;
	memset( *pOutBits, 0, outLength);

	WCHAR* sInput = strBase64.AllocSysString();

	for( j = 0, e = 0, d = 2; j < ( iLen / 4 ); ++j, d += 3 )
	{
		iResult = 0;
		for( i = 0; i < 4; ++i, ++e )
		{
			iResult <<= 6;
			if( e >= iInLen )
				continue;
			if( L'A' <= sInput[ e ] && sInput[ e ] <= L'Z' )
				iResult |= ( sInput[ e ] - L'A' );
			else if( L'a' <= sInput[ e ] && sInput[ e ] <= L'z' )
				iResult |= ( sInput[ e ] - L'a' + 26 );
			else if( L'0'  <= sInput[ e ] && sInput[ e ] <= L'9' )
				iResult |= ( sInput[ e ] - L'0' + 52 );
			else if( L'+'  == sInput[ e ] )
				iResult |= 62;
			else if( L'/' == sInput[ e ] )
				iResult |= 63;
		}
		iMask = 0xFF;
		for( i = 0; i < 3; ++i )
		{
			( *pOutBits )[ d - i ] = (BYTE)( iResult & iMask );
			iResult >>= 8;
		}
	}
	return outLength;
}