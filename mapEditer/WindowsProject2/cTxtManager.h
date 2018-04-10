#pragma once
#include "singleton.h"

enum TXT_RESULT
{
	TXT_FIXED	= 1,
	TXT_SUCCESS	= 0,
	TXT_FAIL	= -1
};

enum MYERROR
{
	MYERROR_READ_FILEPTR,
	MYERROR_READ_FILEEND
};

class cTxtManager : public singleton<cTxtManager>
{
private:
	FILE* fp;

private:
	void Release();
public:
	cTxtManager();
	~cTxtManager();

	void Close();
	//-1(파일이 없음) 0(정상적으로 열림) 1(파일의 조작이 감지됨)
	int ReadOpen(const tstring& filePath, bool check = false);

	void WriteOpen(const tstring& filePath, bool check = false);
	void SubseqWriteOpen(const tstring& filePath);

	void WriteLine(const tstring& str);
	void WriteLine(double value);
	void WriteLine(int value);
	void WriteLine(__int64 value);
	void WriteLine(size_t value);
	void WriteLine(const D3DXVECTOR2& vec);

	void Write(const tstring& str);

	TCHAR ReadChar();
	tstring Read();
	const FILE* GetFilePtr() { return fp; }

	tstring ReadLine();
	template <typename T1>
	T1 ReadLine();

	//pair<tstring, tstring> ReadKeyAndValue();
	//template <typename T1>
	//pair<tstring, T1> ReadKeyAndValue();

	//void WriteKeyAndValue(const tstring& str, const tstring& value);
	//void WriteKeyAndValue(const tstring& str, double value);
	//void WriteKeyAndValue(const tstring& str, __int64 value);
	//void WriteKeyAndValue(const tstring& str, int value);

	struct _stat GetFileInfo(const tstring& path);
};

#define TXTMANAGER cTxtManager::GetInstance()