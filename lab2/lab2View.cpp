
// lab2View.cpp : implementation of the Clab2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab2.h"
#endif

#include "lab2Doc.h"
#include "lab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double PI = 3.14159265358979323846;


// Clab2View

IMPLEMENT_DYNCREATE(Clab2View, CView)

BEGIN_MESSAGE_MAP(Clab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab2View construction/destruction

Clab2View::Clab2View() noexcept
{
	this->gridDim = 25;
	this->dimSlike = 500;

	this->tamniDeo = GetEnhMetaFile(CString("cactus_part.emf"));
	this->svetliDeo = GetEnhMetaFile(CString("cactus_part_light.emf"));

	this->prviUgao = 0;
	this->drugiUgao = 0;
}

Clab2View::~Clab2View()
{

}

BOOL Clab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Clab2View::Translate(CDC* pDC, float dx, float dy, bool rightMultiply) {
	XFORM transformation;
	transformation.eM11 = 1;
	transformation.eM12 = 0;
	transformation.eM21 = 0;
	transformation.eM22 = 1;
	transformation.eDx = dx;
	transformation.eDy = dy;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

void Clab2View::Scale(CDC* pDC, float sx, float sy, bool rightMultiply) {
	XFORM transformation;
	transformation.eM11 = sx;
	transformation.eM12 = 0;
	transformation.eM21 = 0;
	transformation.eM22 = sy;
	transformation.eDx = 0;
	transformation.eDy = 0;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

//uvek false tj. biramo leftMultiply
void Clab2View::Rotate(CDC* pDC, float angle, bool rightMultiply) {  //ROTIRA OKO KOORDINATNOG POCETKA
	XFORM transformation;
	transformation.eM11 = cos(angle);
	transformation.eM12 = sin(angle);
	transformation.eM21 = -sin(angle);
	transformation.eM22 = cos(angle);
	transformation.eDx = 0;
	transformation.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY); //odnosno odozdo na dole
}

void Clab2View::CrtajSaksiju(CDC* pDC, CBrush* staraCetka, CPen* staraOlovka)
{

	CPen olovkaSaksija(PS_SOLID, 1, RGB(0, 0, 0));
	staraOlovka = pDC->SelectObject(&olovkaSaksija);

	CBrush cetkaSaksija(RGB(222, 148, 0));
	staraCetka = pDC->SelectObject(&cetkaSaksija);


	POINT temenaDonjegDelaSaksije[4] = { 
		CPoint(8 * this->gridDim, 18*this->gridDim),
		CPoint(8.4 * this->gridDim, 20*this->gridDim),
		CPoint(11.6 * this->gridDim, 20 * this->gridDim),
		CPoint(12 * this->gridDim, 18 * this->gridDim),
	};
	pDC->Polygon(temenaDonjegDelaSaksije, 4);

	pDC->Rectangle(7.5 * this->gridDim, 17.2 * this->gridDim, 
				   12.5 * this->gridDim, 18 * this->gridDim);


	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraCetka);
}

void Clab2View::CrtajFiguru(CDC* pDC)
{
	// FAKTOR UVECANJA - nova vr. / stara.vr

	const CRect baseCactusRect(-30, 0, 30, -75); 
	
	float maliKaktus = 12.0;
	float srednjiKaktus = 20.0;
	float velikiKaktus = 30.0;

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED); // prvo setujemo graficki mod
	XFORM XformOld;
	pDC->GetWorldTransform(&XformOld);

	CRect scaledRect = baseCactusRect;

	/*
	Translate(pDC, 3*this->gridDim, 10 * this->gridDim, false);
	Rotate(pDC, PI/2, false);
	Rotate(pDC, prviUgao, false);
	pDC->PlayMetaFile(this->svetliDeo, scaledRect);
	pDC->Ellipse(-10, -10, 10, 10);
	Rotate(pDC, -PI / 2, false);
	Translate(pDC, 3 * this->gridDim, 0, false);
	Rotate(pDC, PI / 2, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, -PI / 4, false);
	Rotate(pDC, drugiUgao, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->svetliDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, PI / 2 - drugiUgao, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, - 3 * PI / 4, false);
	pDC->Ellipse(-10, -10, 10, 10);
	Translate(pDC, 2 * this->gridDim, 2 * this->gridDim, false);
	Rotate(pDC, PI / 2, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, PI / 4, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, PI / 4, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, -PI, false);
	pDC->Ellipse(-10, -10, 10, 10);
	Translate(pDC, 0, 3 * this->gridDim, false);
	Rotate(pDC, 3* PI / 4, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, PI/2, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, -5 * PI / 4, false);
	pDC->Ellipse(-10, -10, 10, 10);
	Translate(pDC, 3 * this->gridDim, -3 * this->gridDim, false);
	Rotate(pDC, PI / 4, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, PI / 2, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, 3 * PI / 4, false);
	pDC->Ellipse(-10, -10, 10, 10);
	*/

	Translate(pDC, 250, 17 * this->gridDim, false); // pomeranje centra koord. sistema
	Rotate(pDC, prviUgao, false);
	pDC->PlayMetaFile(this->svetliDeo, scaledRect);

	pDC->Ellipse(-10, -10, 10, 10);

	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false); // nova_dim_x / stara_dim_x
	Translate(pDC, 0, -3 * this->gridDim, false);

	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false); // reset dim
	Rotate(pDC, PI / 4, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false); // nova_dim_x / stara_dim_x

	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, -PI / 2, false);
	Scale(pDC, maliKaktus / velikiKaktus, 1.0f, false);

	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	Scale(pDC, velikiKaktus / maliKaktus, 1.0f, false);
	Rotate(pDC, PI / 4, false);

	pDC->Ellipse(-10, -10, 10, 10);

	Translate(pDC, -2 * this->gridDim, -2 * this->gridDim, false);

	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);

	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, 3 * PI / 2, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);

	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, PI / 2, false);  //pun krug
	pDC->Ellipse(-10, -10, 10, 10);

	Translate(pDC, 0, -3 * this->gridDim, false);

	pDC->PlayMetaFile(this->tamniDeo, scaledRect);
	pDC->Ellipse(-10, -10, 10, 10);

	Translate(pDC, 4 * this->gridDim, 3 * this->gridDim, false);

	Rotate(pDC, drugiUgao, false);

	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->svetliDeo, scaledRect);

	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, PI / 2 - drugiUgao, false);  //vracamo pocetni ugao crtanja
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);

	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, -PI / 2, false);

	pDC->Ellipse(-10, -10, 10, 10);

	Translate(pDC, 3 * this->gridDim, 0, false);

	Rotate(pDC, PI / 4, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);

	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);
	Rotate(pDC, PI / 2, false);
	Scale(pDC, srednjiKaktus / velikiKaktus, 1.0f, false);
	pDC->PlayMetaFile(this->tamniDeo, scaledRect);

	Scale(pDC, velikiKaktus / srednjiKaktus, 1.0f, false);

	pDC->Ellipse(-10, -10, 10, 10);

	pDC->SetWorldTransform(&XformOld); // vraca koordinatni pocetak gde je bio pre transformacije
	pDC->SetGraphicsMode(prevMode);
}

// Clab2View drawing

void Clab2View::OnDraw(CDC* pDC)
{
	Clab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect(0, 0, 500, 500);
	AdjustWindowRectEx(&clientRect, GetStyle(), FALSE, GetExStyle());
	SetWindowPos(nullptr, 0, 0, clientRect.Width(), clientRect.Height(), SWP_NOMOVE | SWP_NOZORDER);


	// OLOVKE

	CPen pozadinskaOlovka(PS_SOLID, 0, RGB(221, 221, 221));
	CPen kaktusOlovka(PS_SOLID, 1, RGB(0, 0, 0));

	// CETKE

	CBrush pozadinskaCetka(RGB(135, 206, 235));
	CBrush kaktusCetka(RGB(0, 204, 0));


	CPen* staraOlovka = pDC->SelectObject(&pozadinskaOlovka);
	CBrush* staraCetka = pDC->SelectObject(&pozadinskaCetka);
	pDC->Rectangle(0, 0, this->dimSlike, this->dimSlike);

	// CRTANJE KAKTUSA

	pDC->SelectObject(kaktusOlovka);
	pDC->SelectObject(kaktusCetka);

	CrtajFiguru(pDC);

	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraCetka);

	// CRTANJE SAKSIJE

	CrtajSaksiju(pDC, staraCetka, staraOlovka);

}


// Clab2View printing

BOOL Clab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab2View diagnostics

#ifdef _DEBUG
void Clab2View::AssertValid() const
{
	CView::AssertValid();
}

void Clab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab2Doc* Clab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab2Doc)));
	return (Clab2Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab2View message handlers


void Clab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	bool static kliknutSpace = false;
	CDC* pDC = GetDC();

	if (nChar == VK_SPACE && !kliknutSpace)
	{
		CPen* gridOlovka = new CPen(PS_SOLID, 2, RGB(230, 230, 230));
		pDC->SelectObject(gridOlovka);

		for (int i = 0; i < this->dimSlike; i += this->gridDim)
		{
			for (int j = 0; j < this->dimSlike; j += this->gridDim)
			{
				pDC->MoveTo(0, i);
				pDC->LineTo(this->dimSlike, i);

				pDC->MoveTo(j, 0);
				pDC->LineTo(j, this->dimSlike);
			}
		}
		kliknutSpace = true;
		delete gridOlovka;
	}
	else if (nChar == VK_SPACE && kliknutSpace)
	{
		kliknutSpace = false;
		Invalidate();
	}

	// ROTIRANJE KAKTUSA

	if (nChar == 'Q')
	{
		this->prviUgao -= PI / 12;

		Invalidate();
	}

	if (nChar == 'W')
	{
		this->prviUgao += PI / 12;

		Invalidate();
	}

	if (nChar == 'E')
	{
		this->drugiUgao -= PI / 12;

		Invalidate();
	}

	if (nChar == 'R')
	{
		this->drugiUgao += PI / 12;

		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
