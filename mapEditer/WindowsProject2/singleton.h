#pragma once

template <typename T1>
class singleton
{
private:
	static T1* instance;
public:
	singleton() {}
	virtual ~singleton() {}

	static T1* GetInstance()
	{
		if (!instance)
			instance = new T1;
		return instance;
	}

	static void ReleaseInstace()
	{
		if (instance)
			delete instance;
		instance = nullptr;
	}
};

template <typename T1>
T1* singleton<T1>::instance = nullptr;