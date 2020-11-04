// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS
// Windows 헤더 파일:
//#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

//이곳에서 빌드 에러가 날시 VS설치시에 MFC를 포함해서 설치해야함.(vs2019이상부터)
//https://developercommunity.visualstudio.com/content/problem/536782/vs-2019-does-not-find.html
#include <afxwin.h>//CDC를 포함
//#include <afxext.h>
//#include <afxcoll.h>
//#include <atlimage.h>//CImage를 포함
//넌 무조건 여기있어야해 개샘ㄴ이라ㅓㅁㄴ아ㅓㅍ ㅍㅈㅇㄹ니ㅏ러ㅏㅈㅁ이ㅏㄴㄹ

// 다이렉트X사용에 필요한 라이브러리와 헤더파일을 링크합니다.
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"

#ifdef _x64
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3d9.lib" )
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx9.lib" )
#else
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3d9.lib" )
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" )
#endif

// 개발에 필요한 windows헤더파일들 입니다.
#include <io.h>
#include <time.h>
#include <shellapi.h>
//#include <direct.h>//mkdir함수

// 개발에 필요한 std헤더파일들 입니다
#include <string>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <set>
#include <array>
#include <algorithm>
#include <random>
#include <thread>
#include <mutex>
#include <memory>

// 사용할 사용자정의 자료형들입니다

using namespace std;

#pragma warning( disable : 4244 )//double에서 float로 변환하면서 데이터가 손실될 수 있음
#pragma warning( disable : 4996 )//함수 이름뒤에 _s를 붙이지 않아 나오는 경고메세지

//#pragma warning( disable : 0018) 
// 사용할 분할컴파일 설정을 추가합니다
#define CHECK_FPS
//#define DEBUGLOG_FPS

//#define DEBUGLOG_TORELEASE

// 유니코드와 아스키코드를 호환시킵니다
#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif

#define T(str) TEXT(str)

// 사용할 매크로 함수들목록입니다
#define MAKE_FUNCPTR(func) (std::move(std::bind(func, this)))

#define SAFE_DELETE(p)			{if(p) delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	{if(p) delete[] p; p = nullptr;}
#define SAFE_RELEASE(p)			{if(p) p->Release(); p = nullptr;}

#if defined(_DEBUG) || defined(DEBUGLOG_TORELEASE)
#define DEBUG_LOG(log) cout << log << endl
#define DEBUG_LOG_W(log) wcout << log << endl
#define DEBUG_LINE(line) cout << line
#define DEBUG_LINE_W(line) wcout << line
#else
#define DEBUG_LOG(log)
#define DEBUG_LOG_W(log)
#define DEBUG_LINE(line)
#define DEBUG_LINE_W(line)
#endif

// 사용할 윈도우 기본설정을 추가합니다
#define TITLE_NAME T("title")
#define WINDOW_NAME T("window")
#ifndef _DEBUG
#define NDEBUG
#endif

// 사용할 namespace들을 추가합니다
#include "MyDevice.h"
#include "MyVector2.h"
//#include "MyRand.h"
#include "MyFunc.h"

// 사용할 매크로 상수및 typedef의 목록입니다
#define EPSION 0.00001
enum LOAD_KIND
{
	LK_IMAGE, LK_SOUND, LK_END
};

enum SOUND_KIND
{
	SK_BGM, SK_EFFECT, SK_END
};

// 사용할 전역변수의 목록입니다
extern HWND					hWnd;
extern LPDIRECT3DDEVICE9	Device;
extern POINT				ptMouse;

// 사용할 매니저 클래스들의 목록입니다
#include "cTxtManager.h"
#include "cTimer.h"
//#include "cSoundManager.h"
#include "cSceneManager.h"
#include "cImageManager.h"
#include "cLoadHelper.h"
#include "cKeyManager.h"
#include "mainWindow.h"