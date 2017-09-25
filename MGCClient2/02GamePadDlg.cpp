#include "StdAfx.h"
#include "02GamePadDlg.h"
#include "CategoryUI.h"
#include "GalleryUI.h"
#include "Gallery2UI.h"

#include "02FloatWnd.h"


C02GamePadDlg::C02GamePadDlg(void)
	: m_nLoginState( 0 )
	, m_category( nullptr )
	//, m_gallery( nullptr )
	, m_gallery2( nullptr )
{
}


C02GamePadDlg::~C02GamePadDlg(void)
{
}

CControlUI* C02GamePadDlg::CreateControl( LPCTSTR pstrClass )
{
	if( EQUSTR( pstrClass, L"Category" ) )
		return new CCategoryUI;
	else if( EQUSTR( pstrClass, L"Gallery" ) )
		//return new CGalleryUI;
	{
		m_gallery2 = CHostableWnd::CreateUI<CGallery2UI>( m_hWnd );
		return m_gallery2->GetHostUI();
	}
	else if( EQUSTR(pstrClass, L"VScroll") )
		return new CVScrollUI;

	return NULL;
}

LRESULT C02GamePadDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//EndModal(1);
	ShowWindow( false );
	return 0;
}

void C02GamePadDlg::Init()
{
	m_category	= FindCtrl<CCategoryUI>( L"category" );
	//m_gallery	= FindCtrl<CGalleryUI>( L"gallery" );

	m_vs = FindCtrl<CVScrollUI>( L"vs" );
	m_vs->SetHost( this );

	m_pBnLogin	= FindCtrl<CButtonUI>( L"bnLogin" );
	m_pBnLogin->OnEvent += MakeDelegate(this, &C02GamePadDlg::OnButtonLoginEvent );

	::ShowWindow( m_hWnd, SW_MAXIMIZE );


	//todo
	// sample

	mgcGameFamilyInfo agfi[] = 
	{ 
		{"1", "1", "推荐游戏", "111"},
		{"2", "1", "射击", "222"},
		{"3", "1", "动作", "333"},
		{"4", "1", "双人对战", "444"},
		{"5", "1", "解密", "444"},
		{"6", "1", "像素风", "444"},
		{"7", "1", "动作冒险", "444"},
		{"8", "1", "推塔", "444"}
	};

	int n = sizeof(agfi)/sizeof(mgcGameFamilyInfo);
	vector<mgcGameFamilyInfo>	fi;
	fi.resize( n );
	copy( agfi, agfi+n, fi.begin() );
	m_category->setData( fi );

}

void C02GamePadDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"windowinit")
	{
		//string gameId;
		//string gameName;
		//string gamePath;
		//string gameFamily;
		//string operationMode;
		//string gameCover;
		//string gameVideo;

		mgcGameInfo agi[] = 
		{ 
			{"1", "NBA2018", "", "", "", "game1.png", "1.mp4"},
			{"2", "星露谷物语", "", "", "", "game2.png", "1.mp4"},
			{"3", "极速狂飙", "", "", "", "game3.png", "1.mp4"},
			{"4", "星际争霸II", "", "", "", "game3.png", "1.mp4"},
			{"5", "旅途", "", "", "", "game2.png", "1.mp4"},
			{"6", "愤怒的小鸟", "", "", "", "game1.png", "1.mp4"},
			{"7", "无双大蛇", "", "", "", "game2.png", "1.mp4"},
			{"8", "黏土世界", "", "", "", "game1.png", "1.mp4"},
			{"9", "上古卷轴7", "", "", "", "game3.png", "1.mp4"},
			{"10", "文明6", "", "", "", "game1.png", "1.mp4"}
		};

		int n = sizeof(agi)/sizeof(mgcGameInfo);

		vector<mgcGameInfo>	gi;
		gi.resize( n );
		copy( agi, agi+n, gi.begin() );

		m_gallery2->setData( gi );

		bool bScrollEnable = m_gallery2->resetAllItems();
		m_vs->SetVisible( bScrollEnable );
		if( bScrollEnable )
			m_vs->SetRatio( 0 );

	}
	else if( strType == L"click")
	{
		if( strSenderName == L"bnLogin" )
		{
			m_nLoginState = !m_nLoginState;

			if( m_nLoginState == 1 )
			{
				m_pBnLogin->SetNormalImage( L"endlogin-normal.png" );
				m_pBnLogin->SetHotImage( L"endlogin-hover.png" );
				m_pBnLogin->SetPushedImage( L"endlogin-pushed.png" );
			}
			else
			{
				m_pBnLogin->SetNormalImage( L"startlogin-normal.png" );
				m_pBnLogin->SetHotImage( L"startlogin-hover.png" );
				m_pBnLogin->SetPushedImage( L"startlogin-pushed.png" );

				//// sample
				//m_gallery2->clearAllItems();

				//m_gallery2->addTitleAndImageName( L"NBA2018",	L"game1.png" );
				//m_gallery2->addTitleAndImageName( L"星露谷物语", L"game2.png" );
				//m_gallery2->addTitleAndImageName( L"极速狂飙",	L"game3.png" );
				//m_gallery2->addTitleAndImageName( L"星际争霸II", L"game2.png" );
				//m_gallery2->addTitleAndImageName( L"旅途",		L"game1.png" );
				//m_gallery2->addTitleAndImageName( L"愤怒的小鸟", L"game3.png" );
				//m_gallery2->addTitleAndImageName( L"无双大蛇",	L"game2.png" );
				//m_gallery2->addTitleAndImageName( L"黏土世界",	L"game3.png" );
				//m_gallery2->addTitleAndImageName( L"上古卷轴7",	L"game1.png" );
				//m_gallery2->addTitleAndImageName( L"文明6",		L"game3.png" );

				//bool bScrollEnable = m_gallery2->resetAllItems();
				//m_vs->SetVisible( bScrollEnable );
				//if( bScrollEnable )
				//	m_vs->SetRatio( 0 );
			}

			C02FloatWnd::Hide();
		}
	}
	else if( strType == L"itemClick" )
	{
		int nCatIndex = (int)msg.wParam;
		if( -1 == nCatIndex )
			return;

		MsgBox( m_category->getItemText( nCatIndex ) );

		//// sample
		//m_gallery2->clearAllItems();

		//for( int i = 0; i < nCatIndex+1; ++i )
		//	m_gallery2->addTitleAndImageName( L"111", L"game1.png" );
	
		//
		//bool bScrollEnable = m_gallery2->resetAllItems();
		//m_vs->SetVisible( bScrollEnable );
		//if( bScrollEnable )
		//	m_vs->SetRatio( 0 );
	}
	else if( strType == L"hover-loginbutton" )
	{
		if( !m_nLoginState )
			return;

		CDUIRect	rtItem = m_pBnLogin->GetPos();
		CDUIPoint	ptLT = rtItem.TopLeft();
		ptLT.Offset( 0, -190 );
		MapWindowPoints( m_hWnd, GetDesktopWindow(), &ptLT, 1 );
		C02FloatWnd::Show( m_hWnd, ptLT );
	}
	else if( strType == L"leave-loginbutton" )
	{
		if( !m_nLoginState )
			return;

		C02FloatWnd::Hide();
	}
}

LRESULT C02GamePadDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_GLY_SETVS_RATIO == uMsg )
	{
		float f = wParam/100.f;
		m_vs->SetRatio( f );
	}
	else if( WM_GLY_CLICK_LEFT == uMsg )
	{
		CStringW	str;
		str.Format( L"WM_GLY_CLICK_LEFT %d\n", wParam );
		MsgBox( str );
	}
	else if( WM_GLY_CLICK_RIGHT == uMsg )
	{
		CStringW	str;
		str.Format( L"WM_GLY_CLICK_RIGHT %d\n", wParam );
		MsgBox( str );
	}

	return CDialogBase::HandleMessage(uMsg, wParam, lParam);
}

void C02GamePadDlg::Data2UI()
{
}

void C02GamePadDlg::OnSetRatio( PointF pt )
{
	//ATLTRACE( L"%.2f %.2f\n", pt.X, pt.Y );
	ATLTRACE( L"%.2f\n", pt.Y );
	//m_gallery->SetVSRatio( pt.Y );
	m_gallery2->SetVSRatio( pt.Y );
}

bool C02GamePadDlg::OnButtonLoginEvent( void* paramters )
{
	TEventUI* pEvent = (TEventUI*)paramters;	
	if( pEvent->Type == UIEVENT_MOUSEENTER || pEvent->Type == UIEVENT_MOUSEHOVER )
	{
		m_pBnLogin->GetManager()->SendNotify( m_pBnLogin, L"hover-loginbutton");
	}
	else if( pEvent->Type == UIEVENT_MOUSELEAVE )
	{
		m_pBnLogin->GetManager()->SendNotify( m_pBnLogin, L"leave-loginbutton");
	}

	return true;
}
