#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <winuser.h>
#include <atlbase.h>
#include "NullableType.h""

#define BeginCounter(x) LARGE_INTEGER x##freq,x##start,x##end;\
	QueryPerformanceFrequency(&x##freq);\
	QueryPerformanceCounter(&x##start);

#define EndCounter(x,y) QueryPerformanceCounter(&x##end);\
	double x##time=((double)x##end.QuadPart-(double)x##start.QuadPart)*1000/x##freq.QuadPart;\
	CString x##str;\
	x##str.Format("%s:%g\n",y,x##time);\
	TRACE(x##str);

#define StrToEnum(strin,str,enm)  if(strin.CompareNoCase( _T(str))==0)  return enm;
#define EnumToStr(enmin,enm,str) if(enmin==enm) return _T(str);	

/************************************************************************
Obtain information about the system's current usage of both physical and virtual memory
************************************************************************/
CString GetSysMemInfo();




void SplitPathA(CStringA szPath,CStringA& szDrive,CStringA& szDir,CStringA& szFileName,CStringA& szFileExt);
void SplitPathW(CStringW szPath,CStringW& szDrive,CStringW& szDir,CStringW& szFileName,CStringW& szFileExt);

CStringA ChangeExtA(CStringA szFileName,CStringA szExt);
CStringW ChangeExtW(CStringW szFileName,CStringW szExt);

CStringA ChangeFileNameA(CStringA szFileName,CStringA newFileName);
CStringW ChangeFileNameW(CStringW szFileName,CStringW newFileName);

CStringW GetAbsolutePathW(CStringW szFileName);
CStringA GetAbsolutePathA(CStringA szFileName); 

CStringW GetDirPathW(CStringW szFileName);
CStringA GetDirPathA(CStringA szFileName); 



//这个函数在处理中文字符时有问题，不能使用
//应使用ATL中的函数进行转换：USES_CONVERSION;  A2W()
int CStringToWChar(CString strChar, wchar_t *pwstr);

int WCharToCString(CString& strChar, wchar_t *pwstr);

void SaveRawFile(CString strFileName,LPBYTE lpData,int byte,int nWidth,int nHeight);

bool IsDirectoryEmpty(LPCTSTR lpszPath);

bool isValidDate(LPCTSTR str);
bool isValidDateChar(TCHAR c);

//str:YYYYMMDD
COleDateTime ParseDateFromString(LPCTSTR str);
//str:HHMMSS
COleDateTime ParseTimeFromString(LPCTSTR str);
//str:YYYYMMDDHHMMSS
COleDateTime ParseDateTimeFromString(LPCSTR str);

COleDateTime ParseDateTime(CString szDate,CString szTime);

#ifdef UNICODE
#define SplitPath SplitPathW
#define GetAbsolutePath GetAbsolutePathW
#define ChangeExt ChangeExtW
#define ChangeFileName ChangeFileNameW
#define GetDirPath GetDirPathW
#else
#define SplitPath SplitPathA
#define GetAbsolutePath GetAbsolutePathA
#define ChangeExt ChangeExtA
#define ChangeFileName ChangeFileNameA
#define GetDirPath GetDirPathA
#endif


CNullableType<long> ToNullableLong(CNullableType<unsigned long> m);

void ShowAddressSpace();

double InchToMM(double inch);
double MMToInch(double mm);
double FrenchToMM(double french);
double MMToFrench(double mm);

double ChangeUnitToMM(double value,CString unit);



CString CreateUID(CString strPrefix,CString strModality,CString strClientID);
CString GetHostIP();

/**
*删除指定目录中的文件及文件夹
*/
bool DeleteWholeDirectory(const char* dirname);

/**
*判断指定目录是否存在,若不存在则创建		
*/
bool DirectoryExists(const char * dirname);

/**
*拷贝目录中的文件至指定文件夹
*/
bool CopyWholeDirectory(const CString &source, const CString & destination);

/**
*创建目录
*/
bool CreateWholeDirectory(CString csFilePath);

/**
*计算目录大小
*/
bool CalculateDirSize(const char* dirname, __int64& i64Size);

/**
*计算文件大小
*/
bool CalculateFileSize(const char* filepath, __int64& i64Size);

/**
*从出生年月换算出年龄
*/
CString FromBirthdateToAge(CString sBirthDate, CString sStudyDate);
bool VerifyDate(int lMonth,int lDay,int lYear);
int Getdays(int year,int month);
char IsLeapYear(int year);


/**
*CString转换为float
*/
float GetFloatFromString(CString str);
/**
*CString转换为int
*/
int GetIntFromString(CString str);

void SplitString(const CString& strSource, const CString& strSeparator, CStringList& strList);

#endif