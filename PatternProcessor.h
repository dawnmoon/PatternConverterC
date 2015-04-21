#pragma once
#include <vector>
using namespace std;

struct TPattern
{
	int width;
	int height;
	CString strBase64;
};
class CPatternProcessor
{
public:
	CPatternProcessor(void);
	~CPatternProcessor(void);

	BOOL ReadPatternXML(CString strXMLPath);
	void Draw(CDC& dc);
	void Save(CDC& dc, CString strImgPath);
	int GetFillWidth() const;
	int GetTotalHeight()const;
	size_t GetPatternCount() const;

private:
	HBITMAP CreatePattern(CString strBase64, int width, int height);
	void DrawPattern(CDC& dc, const TPattern& pat, const CRect& rtPos);	
	int DecodeBase64(CString strBase64, BYTE** pOutBits);
	vector<TPattern> m_vPatterns;
	int m_iBaseW;
	int m_iBaseH;
	int m_iSpace;

};

