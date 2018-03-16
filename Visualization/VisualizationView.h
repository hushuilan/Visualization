
// VisualizationView.h : interface of the CVisualizationView class
//

#pragma once

class CMetaballsDraw;
class CVisualizationView : public CView
{
protected: // create from serialization only
	CVisualizationView();
	DECLARE_DYNCREATE(CVisualizationView)

private:
	HGLRC m_hRC;    //RC 绘图上下文
	CDC* m_pDC;        //DC 设备上下文
	CMetaballsDraw * m_pDrawer;
	BOOL InitializeOpenGL();    //初始化 OpenGL
	BOOL SetupPixelFormat();    //为 DC 建立像素格式

	bool DrawMainFunc();

// Attributes
public:
	CVisualizationDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CVisualizationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in VisualizationView.cpp
inline CVisualizationDoc* CVisualizationView::GetDocument() const
   { return reinterpret_cast<CVisualizationDoc*>(m_pDocument); }
#endif

