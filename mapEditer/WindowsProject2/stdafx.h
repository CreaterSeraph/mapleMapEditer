// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define _CRT_SECURE_NO_WARNINGS
// Windows ��� ����:
//#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

//�̰����� ���� ������ ���� VS��ġ�ÿ� MFC�� �����ؼ� ��ġ�ؾ���.(vs2019�̻����)
//https://developercommunity.visualstudio.com/content/problem/536782/vs-2019-does-not-find.html
#include <afxwin.h>//CDC�� ����
//#include <afxext.h>
//#include <afxcoll.h>
//#include <atlimage.h>//CImage�� ����
//�� ������ �����־���� �������̶�ä����Ƥä� ���������Ϥ����������̤�����

// ���̷�ƮX��뿡 �ʿ��� ���̺귯���� ��������� ��ũ�մϴ�.
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"

#ifdef _x64
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3d9.lib" )
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx9.lib" )
#else
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3d9.lib" )
#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" )
#endif

// ���߿� �ʿ��� windows������ϵ� �Դϴ�.
#include <io.h>
#include <time.h>
#include <shellapi.h>
//#include <direct.h>//mkdir�Լ�

// ���߿� �ʿ��� std������ϵ� �Դϴ�
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

// ����� ��������� �ڷ������Դϴ�

using namespace std;

#pragma warning( disable : 4244 )//double���� float�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� ����
#pragma warning( disable : 4996 )//�Լ� �̸��ڿ� _s�� ������ �ʾ� ������ ���޼���

//#pragma warning( disable : 0018) 
// ����� ���������� ������ �߰��մϴ�
#define CHECK_FPS
//#define DEBUGLOG_FPS

//#define DEBUGLOG_TORELEASE

// �����ڵ�� �ƽ�Ű�ڵ带 ȣȯ��ŵ�ϴ�
#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif

#define T(str) TEXT(str)

// ����� ��ũ�� �Լ������Դϴ�
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

// ����� ������ �⺻������ �߰��մϴ�
#define TITLE_NAME T("title")
#define WINDOW_NAME T("window")
#ifndef _DEBUG
#define NDEBUG
#endif

// ����� namespace���� �߰��մϴ�
#include "MyDevice.h"
#include "MyVector2.h"
//#include "MyRand.h"
#include "MyFunc.h"

// ����� ��ũ�� ����� typedef�� ����Դϴ�
#define EPSION 0.00001
enum LOAD_KIND
{
	LK_IMAGE, LK_SOUND, LK_END
};

enum SOUND_KIND
{
	SK_BGM, SK_EFFECT, SK_END
};

// ����� ���������� ����Դϴ�
extern HWND					hWnd;
extern LPDIRECT3DDEVICE9	Device;
extern POINT				ptMouse;

// ����� �Ŵ��� Ŭ�������� ����Դϴ�
#include "cTxtManager.h"
#include "cTimer.h"
//#include "cSoundManager.h"
#include "cSceneManager.h"
#include "cImageManager.h"
#include "cLoadHelper.h"
#include "cKeyManager.h"
#include "mainWindow.h"