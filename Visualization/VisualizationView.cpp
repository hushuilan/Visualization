
// VisualizationView.cpp : implementation of the CVisualizationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Visualization.h"
#endif

#include "VisualizationDoc.h"
#include "VisualizationView.h"
#include "MetaballsDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glaux.h"

//#pragma comment(lib, "glaux.lib")
// CVisualizationView

IMPLEMENT_DYNCREATE(CVisualizationView, CView)

BEGIN_MESSAGE_MAP(CVisualizationView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CVisualizationView construction/destruction

CVisualizationView::CVisualizationView()
{
	// TODO: add construction code here
	m_pDrawer = NULL;
}

CVisualizationView::~CVisualizationView()
{
	if (m_pDrawer != NULL)
	{
		delete m_pDrawer;
	}
}

BOOL CVisualizationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// CVisualizationView drawing

void CVisualizationView::OnDraw(CDC* /*pDC*/)
{
	CVisualizationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	// 清除颜色、深度缓存
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//可以添加渲染函数
	DrawMainFunc();
	// Flush掉渲染流水线
	::glFinish();
	// 交换前后缓存区
	::SwapBuffers(m_pDC->GetSafeHdc());
}


// CVisualizationView diagnostics

#ifdef _DEBUG
void CVisualizationView::AssertValid() const
{
	CView::AssertValid();
}

void CVisualizationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

BOOL CVisualizationView::InitializeOpenGL()
{
	//客户区获得DC
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if (m_pDC == NULL)
	{
		MessageBox(L"Error Obtaining DC");
		return FALSE;
	}
	//为DC建立像素格式
	if (!SetupPixelFormat())
	{
		return FALSE;
	}
	//创建 RC
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//Failure to Create Rendering Context
	if (m_hRC == 0)
	{
		MessageBox(L"Error Creating RC");
		return FALSE;
	}
	//设定OpenGL当前线程的渲染环境。
	//以后这个线程所有的OpenGL调用都是在这个hdc标识的设备上绘制。
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)
	{
		MessageBox(L"Error making RC Current");
		return FALSE;
	}
	//背景颜色
	::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//深度缓存 1最大，让任何都能显示出来
	::glClearDepth(1.0f);
	//如果通过比较后深度值发生变化了，会进行更新深度缓冲区的操作
	::glEnable(GL_DEPTH_TEST);
	return TRUE;
}

BOOL CVisualizationView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,                // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)
	{
		return FALSE;
	}
	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

CVisualizationDoc* CVisualizationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisualizationDoc)));
	return (CVisualizationDoc*)m_pDocument;
}
#endif //_DEBUG


// CVisualizationView message handlers


int CVisualizationView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	InitializeOpenGL();

	return 0;
}


void CVisualizationView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		MessageBox(L"Could not make RC non-current");
	}

	//Delete the rendering context
	if (::wglDeleteContext(m_hRC) == FALSE)
	{
		MessageBox(L"Could not delete RC");
	}
	//Delete the DC
	if (m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
	m_pDC = NULL;
}


void CVisualizationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (0 >= cx || 0 >= cy)
	{
		return;
	}

	if (cy == 0)
	{
		return;
	}

	double minX = 0;
	double maxX = 0;
	double minY = 0;
	double maxY = 0;

	double height = 0;
	double width = 0;
	double ratio = (double)cx / cy;
	if (cx < cy)
	{
		width = 100.0;
		height = width / ratio;
	}
	else
	{
		height = 100.0;
		width = height * ratio;
	}

	minX = -0.5*width;
	maxX = 0.5*width;

	minY = -0.5*height;
	maxY = 0.5*height;

	::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// specify the back of the buffer as clear depth
	::glClearDepth(1.0f);
	// enable depth testing
	::glEnable(GL_DEPTH_TEST);
	::glViewport(0, 0, cx, cy);
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	::glOrtho(minX, maxX, minY, maxY, -10, 10);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	Invalidate();
}


BOOL CVisualizationView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return true;
}


bool CVisualizationView::DrawMainFunc()
{
	float R = 25.0;
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < 100; i++)
	{
		glVertex3f(R*cos(2.0 * M_PI / 100 * i), R*sin(2.0 * M_PI / 100 * i), 0.0f);
	}

	glEnd();
	glFlush();

	if (!m_pDrawer)
	{
		m_pDrawer = new CMetaballsDraw();
	}

	m_pDrawer->DrawFunc();

	return true;
}