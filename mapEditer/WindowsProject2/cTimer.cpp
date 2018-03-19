#include "stdafx.h"
#include "cTimer.h"


cTimer::cTimer()
	:elapsedTime(0), deltaTime(0), timer(), timeScale(1), fps()
{
	//Init();
}


cTimer::~cTimer()
{
}

void cTimer::Init()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency))
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
		timerDelta = 1.0 / periodFrequency;
	}
	else
	{
		MessageBox(hWnd, T("��ǻ�Ͱ� ����Ÿ�̸Ӹ� �������� �ʽ��ϴ�"), T("����"), MB_OK);
		PostQuitMessage(0);
	}
	fixedFrame = 60.f;
}

double cTimer::Skip()
{
	__int64 last = lastTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	return timerDelta * (last - lastTime);
}

bool cTimer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	if (fixedFrame < EPSION || lastTime + (1.0 / fixedFrame) * periodFrequency < currentTime)
		//������������ 0�̰ų� / ��ǥ �������� ����
	{
		deltaTime = timerDelta * (currentTime - lastTime);
		lastTime = currentTime;
		elapsedTime += deltaTime;
#ifdef CHECK_FPS
		fps.timeCount += deltaTime;
		fps.count++;
		if (fps.timeCount > 1)
		{
			fps.fps = fps.count;
			fps.count = 0;
			fps.timeCount -= 1;
#ifdef DEBUGLOG_FPS
			
			DEBUG_LOG(fps.fps);
#endif
		}
#endif
		return true;
	}
	return false;
}

bool cTimer::StartTimer(OUT int* idx)
{
	for (int i = 0; i < TIMER_MAX; i++)
	{
		if (timer[i] == 0)
		{
			*idx = i;
			QueryPerformanceCounter((LARGE_INTEGER*)&timer[i]);
			return true;
		}
	}
	return false;
}

double cTimer::UpdateTimer(int idx)
{
	if (timer[idx] == 0) return 0;

	__int64 temp = timer[idx];
	QueryPerformanceCounter((LARGE_INTEGER*)&timer[idx]);
	return timerDelta * (timer[idx] - temp);
}

double cTimer::EndTimer(int idx)
{
	if (timer[idx] == 0) return 0;

	double result;
	__int64 temp = timer[idx];
	QueryPerformanceCounter((LARGE_INTEGER*)&timer[idx]);
	result = timerDelta * (timer[idx] - temp);
	timer[idx] = 0;
	return result;
}
