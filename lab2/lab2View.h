
// lab2View.h : interface of the Clab2View class
//

#pragma once


class Clab2View : public CView
{
protected: // create from serialization only
	Clab2View() noexcept;
	DECLARE_DYNCREATE(Clab2View)

// Attributes
public:
	Clab2Doc* GetDocument() const;

	HENHMETAFILE svetliDeo;
	HENHMETAFILE tamniDeo;

	float prviUgao;
	float drugiUgao;

	int dimSlike;
	int gridDim;

// Operations
public:

// Overrides
public:
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Scale(CDC* pDC, float sx, float sy, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void CrtajSaksiju(CDC* pDC, CBrush* staraCetka, CPen* staraOlovka);
	void CrtajFiguru(CDC* pDC);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Clab2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in lab2View.cpp
inline Clab2Doc* Clab2View::GetDocument() const
   { return reinterpret_cast<Clab2Doc*>(m_pDocument); }
#endif

