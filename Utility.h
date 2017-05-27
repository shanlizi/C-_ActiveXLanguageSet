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



//��������ڴ��������ַ�ʱ�����⣬����ʹ��
//Ӧʹ��ATL�еĺ�������ת����USES_CONVERSION;  A2W()
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
*ɾ��ָ��Ŀ¼�е��ļ����ļ���
*/
bool DeleteWholeDirectory(const char* dirname);

/**
*�ж�ָ��Ŀ¼�Ƿ����,���������򴴽�		
*/
bool DirectoryExists(const char * dirname);

/**
*����Ŀ¼�е��ļ���ָ���ļ���
*/
bool CopyWholeDirectory(const CString &source, const CString & destination);

/**
*����Ŀ¼
*/
bool CreateWholeDirectory(CString csFilePath);

/**
*����Ŀ¼��С
*/
bool CalculateDirSize(const char* dirname, __int64& i64Size);

/**
*�����ļ���С
*/
bool CalculateFileSize(const char* filepath, __int64& i64Size);

/**
*�ӳ������»��������
*/
CString FromBirthdateToAge(CString sBirthDate, CString sStudyDate);
bool VerifyDate(int lMonth,int lDay,int lYear);
int Getdays(int year,int month);
char IsLeapYear(int year);


/**
*CStringת��Ϊfloat
*/
float GetFloatFromString(CString str);
/**
*CStringת��Ϊint
*/
int GetIntFromString(CString str);

void SplitString(const CString& strSource, const CString& strSeparator, CStringList& strList);

#endif