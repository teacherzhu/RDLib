#include "StdAfx.h"
#include "WindowWnd.h"

#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib" )


//#include "AppData.h"


void __Trace(LPCTSTR pstrFormat, ...)
{
#ifdef _DEBUG
	TCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	_tcscat(szBuffer, _T("\n"));
	va_end(args);
	::OutputDebugString(szBuffer);
#endif
}

LPCTSTR __TraceMsg(UINT uMsg)
{
#define MSGDEF(x) if(uMsg==x) return _T(#x)
	MSGDEF(WM_SETCURSOR);
	MSGDEF(WM_NCHITTEST);
	MSGDEF(WM_NCPAINT);
	MSGDEF(WM_PAINT);
	MSGDEF(WM_ERASEBKGND);
	MSGDEF(WM_NCMOUSEMOVE);  
	MSGDEF(WM_MOUSEMOVE);
	MSGDEF(WM_MOUSELEAVE);
	MSGDEF(WM_MOUSEHOVER);   
	MSGDEF(WM_NOTIFY);
	MSGDEF(WM_COMMAND);
	MSGDEF(WM_MEASUREITEM);
	MSGDEF(WM_DRAWITEM);   
	MSGDEF(WM_LBUTTONDOWN);
	MSGDEF(WM_LBUTTONUP);
	MSGDEF(WM_LBUTTONDBLCLK);
	MSGDEF(WM_RBUTTONDOWN);
	MSGDEF(WM_RBUTTONUP);
	MSGDEF(WM_RBUTTONDBLCLK);
	MSGDEF(WM_SETFOCUS);
	MSGDEF(WM_KILLFOCUS);  
	MSGDEF(WM_MOVE);
	MSGDEF(WM_SIZE);
	MSGDEF(WM_SIZING);
	MSGDEF(WM_MOVING);
	MSGDEF(WM_GETMINMAXINFO);
	MSGDEF(WM_CAPTURECHANGED);
	MSGDEF(WM_WINDOWPOSCHANGED);
	MSGDEF(WM_WINDOWPOSCHANGING);   
	MSGDEF(WM_NCCALCSIZE);
	MSGDEF(WM_NCCREATE);
	MSGDEF(WM_NCDESTROY);
	MSGDEF(WM_TIMER);
	MSGDEF(WM_KEYDOWN);
	MSGDEF(WM_KEYUP);
	MSGDEF(WM_CHAR);
	MSGDEF(WM_SYSKEYDOWN);
	MSGDEF(WM_SYSKEYUP);
	MSGDEF(WM_SYSCOMMAND);
	MSGDEF(WM_SYSCHAR);
	MSGDEF(WM_VSCROLL);
	MSGDEF(WM_HSCROLL);
	MSGDEF(WM_CHAR);
	MSGDEF(WM_SHOWWINDOW);
	MSGDEF(WM_PARENTNOTIFY);
	MSGDEF(WM_CREATE);
	MSGDEF(WM_NCACTIVATE);
	MSGDEF(WM_ACTIVATE);
	MSGDEF(WM_ACTIVATEAPP);   
	MSGDEF(WM_CLOSE);
	MSGDEF(WM_DESTROY);
	MSGDEF(WM_GETICON);   
	MSGDEF(WM_GETTEXT);
	MSGDEF(WM_GETTEXTLENGTH);   
	static TCHAR szMsg[10];
	::wsprintf(szMsg, _T("0x%04X"), uMsg);
	return szMsg;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//
/*

CStdPtrArray::CStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
{
	ASSERT(iPreallocSize>=0);
	if( iPreallocSize > 0 ) m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
}

CStdPtrArray::CStdPtrArray(const CStdPtrArray& src) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(0)
{
	for(int i=0; i<src.GetSize(); i++)
		Add(src.GetAt(i));
}

CStdPtrArray::~CStdPtrArray()
{
	if( m_ppVoid != NULL ) free(m_ppVoid);
}

void CStdPtrArray::Empty()
{
	if( m_ppVoid != NULL ) free(m_ppVoid);
	m_ppVoid = NULL;
	m_nCount = m_nAllocated = 0;
}

void CStdPtrArray::Resize(int iSize)
{
	Empty();
	m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
	::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
	m_nAllocated = iSize;
	m_nCount = iSize;
}

bool CStdPtrArray::IsEmpty() const
{
	return m_nCount == 0;
}

bool CStdPtrArray::Add(LPVOID pData)
{
	if( ++m_nCount >= m_nAllocated) {
		int nAllocated = m_nAllocated * 2;
		if( nAllocated == 0 ) nAllocated = 11;
		LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
		if( ppVoid != NULL ) {
			m_nAllocated = nAllocated;
			m_ppVoid = ppVoid;
		}
		else {
			--m_nCount;
			return false;
		}
	}
	m_ppVoid[m_nCount - 1] = pData;
	return true;
}

bool CStdPtrArray::InsertAt(int iIndex, LPVOID pData)
{
	if( iIndex == m_nCount ) return Add(pData);
	if( iIndex < 0 || iIndex > m_nCount ) return false;
	if( ++m_nCount >= m_nAllocated) {
		int nAllocated = m_nAllocated * 2;
		if( nAllocated == 0 ) nAllocated = 11;
		LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
		if( ppVoid != NULL ) {
			m_nAllocated = nAllocated;
			m_ppVoid = ppVoid;
		}
		else {
			--m_nCount;
			return false;
		}
	}
	memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
	m_ppVoid[iIndex] = pData;
	return true;
}

bool CStdPtrArray::SetAt(int iIndex, LPVOID pData)
{
	if( iIndex < 0 || iIndex >= m_nCount ) return false;
	m_ppVoid[iIndex] = pData;
	return true;
}

bool CStdPtrArray::Remove(int iIndex)
{
	if( iIndex < 0 || iIndex >= m_nCount ) return false;
	if( iIndex < --m_nCount ) ::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));
	return true;
}

int CStdPtrArray::Find(LPVOID pData) const
{
	for( int i = 0; i < m_nCount; i++ ) if( m_ppVoid[i] == pData ) return i;
	return -1;
}

int CStdPtrArray::GetSize() const
{
	return m_nCount;
}

LPVOID* CStdPtrArray::GetData()
{
	return m_ppVoid;
}

LPVOID CStdPtrArray::GetAt(int iIndex) const
{
	if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
	return m_ppVoid[iIndex];
}

LPVOID CStdPtrArray::operator[] (int iIndex) const
{
	ASSERT(iIndex>=0 && iIndex<m_nCount);
	return m_ppVoid[iIndex];
}

//*/


///////////////////////////////////////////////////////////////////////////////////////
////
////
//
//CStdValArray::CStdValArray(int iElementSize, int iPreallocSize /*= 0*/) : 
//m_pVoid(NULL), 
//	m_nCount(0), 
//	m_iElementSize(iElementSize), 
//	m_nAllocated(iPreallocSize)
//{
//	ASSERT(iElementSize>0);
//	ASSERT(iPreallocSize>=0);
//	if( iPreallocSize > 0 ) m_pVoid = static_cast<LPBYTE>(malloc(iPreallocSize * m_iElementSize));
//}
//
//CStdValArray::~CStdValArray()
//{
//	if( m_pVoid != NULL ) free(m_pVoid);
//}
//
//void CStdValArray::Empty()
//{   
//	m_nCount = 0;  // NOTE: We keep the memory in place
//}
//
//bool CStdValArray::IsEmpty() const
//{
//	return m_nCount == 0;
//}
//
//bool CStdValArray::Add(LPCVOID pData)
//{
//	if( ++m_nCount >= m_nAllocated) {
//		int nAllocated = m_nAllocated * 2;
//		if( nAllocated == 0 ) nAllocated = 11;
//		LPBYTE pVoid = static_cast<LPBYTE>(realloc(m_pVoid, nAllocated * m_iElementSize));
//		if( pVoid != NULL ) {
//			m_nAllocated = nAllocated;
//			m_pVoid = pVoid;
//		}
//		else {
//			--m_nCount;
//			return false;
//		}
//	}
//	::CopyMemory(m_pVoid + ((m_nCount - 1) * m_iElementSize), pData, m_iElementSize);
//	return true;
//}
//
//bool CStdValArray::Remove(int iIndex)
//{
//	if( iIndex < 0 || iIndex >= m_nCount ) return false;
//	if( iIndex < --m_nCount ) ::CopyMemory(m_pVoid + (iIndex * m_iElementSize), m_pVoid + ((iIndex + 1) * m_iElementSize), (m_nCount - iIndex) * m_iElementSize);
//	return true;
//}
//
//int CStdValArray::GetSize() const
//{
//	return m_nCount;
//}
//
//LPVOID CStdValArray::GetData()
//{
//	return static_cast<LPVOID>(m_pVoid);
//}
//
//LPVOID CStdValArray::GetAt(int iIndex) const
//{
//	if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
//	return m_pVoid + (iIndex * m_iElementSize);
//}
//
//LPVOID CStdValArray::operator[] (int iIndex) const
//{
//	ASSERT(iIndex>=0 && iIndex<m_nCount);
//	return m_pVoid + (iIndex * m_iElementSize);
//}



CWindowWnd::CWindowWnd() : m_hWnd(NULL), m_OldWndProc(::DefWindowProc), m_bSubclassed(false)
{
}

//HWND CWindowWnd::GetHWND() const 
//{ 
//	return m_hWnd; 
//}

UINT CWindowWnd::GetClassStyle() const
{
	return 0;
}

LPCTSTR CWindowWnd::GetSuperClassName() const
{
	return NULL;
}

//CWindowWnd::operator HWND() const
//{
//	return m_hWnd;
//}

HWND CWindowWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu)
{
	return Create(hwndParent, pstrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND CWindowWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	LPCTSTR	strSuperClassName = GetSuperClassName();
	if( strSuperClassName == NULL )
	{
		if( !RegisterWindowClass() )
			return NULL;
	}
	else
	{
		if( !RegisterSuperclass() )
			return NULL;
	}

	//if( GetSuperClassName() != NULL && !RegisterSuperclass() ) return NULL;
	//if( GetSuperClassName() == NULL && !RegisterWindowClass() ) return NULL;
	m_hWnd = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, 
		x, y, cx, cy, hwndParent, hMenu, CAppData::GetInstance(), this);
	DWORD dwError=GetLastError();

	ASSERT(m_hWnd!=NULL);
	return m_hWnd;
}

HWND CWindowWnd::Subclass(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(m_hWnd==NULL);
	m_OldWndProc = SubclassWindow(hWnd, __WndProc);
	if( m_OldWndProc == NULL ) return NULL;
	m_bSubclassed = true;
	m_hWnd = hWnd;
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
	return m_hWnd;
}

void CWindowWnd::Unsubclass()
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	if( !m_bSubclassed ) return;
	SubclassWindow(m_hWnd, m_OldWndProc);
	m_OldWndProc = ::DefWindowProc;
	m_bSubclassed = false;
}

void CWindowWnd::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}	

void CWindowWnd::Close()
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	PostMessage(WM_CLOSE, 0, 0L);
}

void CWindowWnd::CenterWindow()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	if( hWndCenter == NULL ) rcCenter = rcArea; else ::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if( xLeft < rcArea.left ) xLeft = rcArea.left;
	else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
	if( yTop < rcArea.top ) yTop = rcArea.top;
	else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
	::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CWindowWnd::SetIcon(UINT nRes)
{
	HICON hIcon = (HICON)::LoadImage(CAppData::GetInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	ASSERT(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
	hIcon = (HICON)::LoadImage(CAppData::GetInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	ASSERT(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

bool CWindowWnd::RegisterWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CWindowWnd::__WndProc;
	wc.hInstance = CAppData::GetInstance();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClass(&wc);
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

bool CWindowWnd::RegisterSuperclass()
{
	LPCTSTR	strSuperClassName = GetSuperClassName();

	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	if( !::GetClassInfoEx(NULL, strSuperClassName, &wc) ) {
		if( !::GetClassInfoEx(CAppData::GetInstance(), strSuperClassName, &wc) ) {
			ASSERT(!"Unable to locate window class");
			return NULL;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = CWindowWnd::__ControlProc;
	wc.hInstance = CAppData::GetInstance();
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClassEx(&wc);
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LRESULT CALLBACK CWindowWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindowWnd* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CWindowWnd*>(lpcs->lpCreateParams);
		pThis->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	} 
	else {
		pThis = reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
			if( pThis->m_bSubclassed ) pThis->Unsubclass();
			pThis->m_hWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}

	//20170106
	if( pThis != NULL && IsWindow( pThis->m_hWnd ) )
	{
		//if( pThis != NULL ) {
		return pThis->HandleMessage(uMsg, wParam, lParam);
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK CWindowWnd::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindowWnd* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CWindowWnd*>(lpcs->lpCreateParams);
		::SetProp(hWnd, _T("WndX"), (HANDLE) pThis);
		pThis->m_hWnd = hWnd;
	} 
	else {
		pThis = reinterpret_cast<CWindowWnd*>(::GetProp(hWnd, _T("WndX")));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			if( pThis->m_bSubclassed ) pThis->Unsubclass();
			::SetProp(hWnd, _T("WndX"), NULL);
			pThis->m_hWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) {
		return pThis->HandleMessage(uMsg, wParam, lParam);
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CWindowWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
} 

LRESULT CWindowWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

bool CWindowWnd::MoveWindow( int x, int y, int width, int height, bool repaint/*=true*/ )
{
	ASSERT(::IsWindow(m_hWnd));
	return (::MoveWindow(m_hWnd, x, y, width, height, repaint?TRUE:FALSE) == TRUE);
}

//void CWindowWnd::ResizeClient(int cx /*= -1*/, int cy /*= -1*/)
//{
//	ASSERT(::IsWindow(m_hWnd));
//	RECT rc = { 0 };
//	if( !::GetClientRect(m_hWnd, &rc) ) return;
//	if( cx != -1 ) rc.right = cx;
//	if( cy != -1 ) rc.bottom = cy;
//	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) return;
//	::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
//}

LRESULT CWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
}

void CWindowWnd::OnFinalMessage(HWND /*hWnd*/)
{
}

//UINT CWindowWnd::ShowModal()
//{
//	ASSERT(::IsWindow(m_hWnd));
//	UINT nRet = 0;
//	HWND hWndParent = GetWindowOwner(m_hWnd);
//	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
//	::EnableWindow(hWndParent, FALSE);
//	MSG msg = { 0 };
//	while( ::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0) ) {
//		if( msg.message == WM_CLOSE && msg.hwnd == m_hWnd ) {
//			nRet = msg.wParam;
//			::EnableWindow(hWndParent, TRUE);
//			::SetFocus(hWndParent);
//		}
//		if( !CPaintManagerUI::TranslateMessage(&msg) ) {
//			::TranslateMessage(&msg);
//			::DispatchMessage(&msg);
//		}
//		if( msg.message == WM_QUIT ) break;
//	}
//	::EnableWindow(hWndParent, TRUE);
//	::SetFocus(hWndParent);
//	if( msg.message == WM_QUIT ) ::PostQuitMessage(msg.wParam);
//	return nRet;
//}



//////////////////////////////////////////////////////////////////////////
CDialogWnd::CDialogWnd():m_ExitModal(false),m_DialogResult(0)
{

}

UINT CDialogWnd::ShowModal()
{
	//如果是模态显示的窗口,则不要在OnFinal中析构自己,否则IsWindow会导致this->m_hWnd访问违例
	if (::IsWindow(m_hWnd))
	{		
		HWND hWndParent = GetWindowOwner(m_hWnd);
		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
		::EnableWindow(hWndParent, FALSE);
		MSG msg = { 0 };
		HWND hWnd = m_hWnd;
		while(::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0) ) 
		{
			if( !CAppData::TranslateMessage(&msg) ) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			if (m_ExitModal){				
				break;
			}
		}
		ASSERT(m_ExitModal);
		if (m_ExitModal)//必须是EndDialog导致的
		{
			//关键是在窗口销毁之前使父窗口可用,否则将导致焦点被其他程序的窗口占用
			::EnableWindow(hWndParent, TRUE);
			::SetActiveWindow(hWndParent);
			::DestroyWindow(m_hWnd);
		}
	}
	return m_DialogResult;
}

bool CDialogWnd::EndModal( UINT ret )
{
	bool result = (::IsWindow(m_hWnd)==TRUE);
	if (result && !m_ExitModal)
	{
		m_ExitModal = true;
		m_DialogResult = ret;
		PostMessage(WM_NULL);
	}
	return result;
}
