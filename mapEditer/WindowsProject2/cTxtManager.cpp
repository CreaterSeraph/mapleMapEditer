#include "stdafx.h"
#include "cTxtManager.h"

cTxtManager::cTxtManager()
	:fp(nullptr)
{
}


cTxtManager::~cTxtManager()
{
	Release();
}

void cTxtManager::Release()
{
	Close();
}

void cTxtManager::Close()
{
	if (fp)
		fclose(fp);
}

int cTxtManager::ReadOpen(const tstring& filePath, bool check)
{
	_tfopen_s(&fp, filePath.c_str(), TEXT("r+"));
	if (fp == nullptr)
	{
		return TXT_FAIL;
	}
	if (check)
	{
		auto st = GetFileInfo(filePath);
		if (st.st_mtime != _ttoi(ReadLine().c_str()))
		{
			return TXT_FIXED;
		}
	}
	return TXT_SUCCESS;
}

void cTxtManager::WriteOpen(const tstring& filePath, bool check)
{
	_tfopen_s(&fp, filePath.c_str(), TEXT("w+"));
	if (check)
	{
		TCHAR str[128];
		time_t nowtime;
		time(&nowtime);
		_itot(nowtime, str, 10);
		_ftprintf(fp, TEXT("%s\n"), str);
	}
}

void cTxtManager::SubseqWriteOpen(const tstring& filePath)
{
	_tfopen_s(&fp, filePath.c_str(), TEXT("a+"));
}

void cTxtManager::WriteLine(const tstring& str)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%s\n"), str.c_str());
	}
}

void cTxtManager::WriteLine(double value)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%f\n"), value);
	}
}

void cTxtManager::WriteLine(int value)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%d\n"), value);
	}
}

void cTxtManager::WriteLine(__int64 value)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%I64d\n"), value);
	}
}

void cTxtManager::WriteLine(size_t value)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%zd\n"), value);
	}
}

void cTxtManager::WriteLine(const D3DXVECTOR2& vec)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%f,%f\n"), vec.x, vec.y);
	}
}

void cTxtManager::Write(const tstring& str)
{
	if (fp)
	{
		_ftprintf(fp, TEXT("%s"), str.c_str());
	}
}

TCHAR cTxtManager::ReadChar()
{
	if (fp)
	{
		return _fgettc(fp);
	}
	return 0;
}

tstring cTxtManager::Read()
{
	//쉬발
	return tstring();
}

tstring cTxtManager::ReadLine()
{
	if (!fp)
		throw MYERROR_READ_FILEPTR;
	if (feof(fp))
	{
		DEBUG_LOG("이미 파일의 끝까지 읽어졌습니다");
		//throw MYERROR_READ_FILEEND;
	}
	TCHAR str[128];
	ZeroMemory(str, 128);
	if (fp)
		_fgetts(str, 128, fp);
	tstring result = str;
	if (result.empty())
	{
		DEBUG_LOG("읽을수 없습니다");
		//throw MYERROR_READ_FILEEND;
		return result;
	}

	if (result.back() == '\n')
		result.pop_back();
	return result;
}

template <>
int cTxtManager::ReadLine()
{
	return _ttoi(ReadLine().c_str());
}

template <>
size_t cTxtManager::ReadLine()
{
	return _ttoi(ReadLine().c_str());
}

template <>
double cTxtManager::ReadLine()
{
	return _ttof(ReadLine().c_str());
}

template <>
__int64 cTxtManager::ReadLine()
{
	return _ttoi64(ReadLine().c_str());
}

template <>
D3DXVECTOR2 cTxtManager::ReadLine()
{
	tstring readData;
	try
	{
		readData = ReadLine();
	}
	catch (const MYERROR& error)
	{
		switch (error)
		{
		case MYERROR_READ_FILEPTR:
			MessageBox(hWnd, T("MYERROR_READ_FILEPTR"), T("ERROR"), MB_OK);
			throw MYERROR_READ_FILEPTR;
		case MYERROR_READ_FILEEND:
			MessageBox(hWnd, T("MYERROR_READ_FILEEND"), T("ERROR"), MB_OK);
			throw MYERROR_READ_FILEEND;
		}
	}
	
	size_t cut = readData.find(T(','));
	if (cut == string::npos)
		return D3DXVECTOR2();
	D3DXVECTOR2 result;
	result.x = _ttof(readData.substr(0, cut).c_str());
	result.y = _ttof(readData.substr(cut + 1).c_str());

	return result;
}

struct _stat cTxtManager::GetFileInfo(const tstring& path)
{
	struct _stat info;

	_tstat(path.c_str(), &info);

	if (errno == 0)
		return info;

	switch (errno)
	{
	case ENOENT:
		DEBUG_LOG_W(L"파일경로가 존재하지 않습니다");
		break;
	case EACCES:
		DEBUG_LOG_W(L"읽기 권한이 없습니다");
		break;
	case ENOTDIR:
		DEBUG_LOG_W(L"파일 디렉토리가 아닙니다");
		break;
	default:
		return info;
	}
	ZeroMemory(&info, sizeof(info));
	return info;
}

//void cTxtManager::WriteKeyAndValue(const tstring& str, const tstring& value)
//{
//}
//
//void cTxtManager::WriteKeyAndValue(const tstring& str, double value)
//{
//}
//
//void cTxtManager::WriteKeyAndValue(const tstring& str, __int64 value)
//{
//}
//
//void cTxtManager::WriteKeyAndValue(const tstring& str, int value)
//{
//}