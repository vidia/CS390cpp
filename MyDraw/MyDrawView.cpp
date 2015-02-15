// MyDrawView.cpp : implementation of the CMyDrawView class
//

#include "stdafx.h"
#include "MyDraw.h"

#include "MyDrawDoc.h"
#include "MyDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyDrawView

IMPLEMENT_DYNCREATE(CMyDrawView, CView)

BEGIN_MESSAGE_MAP(CMyDrawView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FIGURE_LINE, &CMyDrawView::OnFigureLine)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FIGURE_RECTANGLE, &CMyDrawView::OnFigureRectangle)
	ON_COMMAND(ID_FIGURE_OVAL, &CMyDrawView::OnFigureOval)
	ON_COMMAND(ID_EDIT_DELETE, &CMyDrawView::OnEditDelete)
END_MESSAGE_MAP()

// CMyDrawView construction/destruction

CMyDrawView::CMyDrawView()
{
	// TODO: add construction code here

}

CMyDrawView::~CMyDrawView()
{
}

BOOL CMyDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMyDrawView drawing

void CMyDrawView::OnDraw(CDC* pDC)
{
	CMyDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	pDoc->drawing.draw(pDC);
}


// CMyDrawView printing

BOOL CMyDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CMyDrawView diagnostics

#ifdef _DEBUG
void CMyDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CMyDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyDrawDoc* CMyDrawView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDrawDoc)));
	return (CMyDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyDrawView message handlers

void CMyDrawView::OnFigureLine()
{
	// TODO: Add your command handler code here
	CMyDrawDoc* pDoc = GetDocument();
	pDoc->drawing.setEditMode(Drawing::NewLineMode);
}

afx_msg void CMyDrawView::OnMouseMove(UINT nFlags,CPoint point)
{
	CMyDrawDoc* pDoc = GetDocument();
	pDoc->drawing.OnMouse(this, nFlags, point);
}

afx_msg void CMyDrawView::OnLButtonUp(UINT nFlags,CPoint point)
{
	CMyDrawDoc* pDoc = GetDocument();
	pDoc->drawing.OnMouse(this, nFlags, point);
}

afx_msg void CMyDrawView::OnLButtonDown(UINT nFlags,CPoint point)
{
	CMyDrawDoc* pDoc = GetDocument();

	pDoc->drawing.OnMouse(this, nFlags, point);
}


void CMyDrawView::OnFigureRectangle()
{
	CMyDrawDoc* pDoc = GetDocument();
	pDoc->drawing.setEditMode(Drawing::NewRectangleMode);
}


void CMyDrawView::OnFigureOval()
{
	CMyDrawDoc* pDoc = GetDocument();
	pDoc->drawing.setEditMode(Drawing::NewOvalMode);
}


void CMyDrawView::OnEditDelete()
{
	CMyDrawDoc* pDoc = GetDocument();
	pDoc->drawing.deleteSelected(this); 
	// TODO: Add your command handler code here
}
