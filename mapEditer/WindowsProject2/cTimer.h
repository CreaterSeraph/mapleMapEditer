#pragma once
#include "singleton.h"

#define TIMER_MAX 100

class cTimer : public singleton<cTimer>
{
private:
	double	fixedFrame;

	__int64	currentTime;
	__int64	lastTime;
	__int64	periodFrequency;
	double	timerDelta;

	double	elapsedTime;//���ӽ����� ����ð�
	double  deltaTime;

#ifdef CHECK_FPS
	struct TimerFps
	{
		int		count;//�ʸ��� ����
		int		fps;//����� fps
		double	timeCount;
		TimerFps()
			:count(0), fps(0), timeCount(0)
		{
		}
	} fps;
#endif

	__int64 timer[TIMER_MAX];

	double timeScale;
public:
	cTimer();
	~cTimer();

	void Init();
	double Skip();
	bool Update();

	void SetFixedFrame(double frame) { fixedFrame = frame; }
	void SetTimeScale(double scale) { timeScale = scale; }

	const double& GetFixedFrame() { return fixedFrame; }
	double GetDeltaTime() { return deltaTime * timeScale; }
	const double& GetElapsedTime() { return elapsedTime; }
#ifdef CHECK_FPS
	const int& GetFps() { return fps.fps; }
#endif

	bool StartTimer(OUT int* idx);
	double UpdateTimer(int idx);
	double EndTimer(int idx);
};

#define TIMER cTimer::GetInstance()