#include "stdafx.h"
//#include <vector>
#include "Utility.h"
#include  <io.h>



void SplitPathA(CStringA szPath,CStringA& szDrive,CStringA& szDir,CStringA& szFileName,CStringA& szFileExt)
{
	char Drive[MAX_PATH];
	char Dir[MAX_PATH];
	char FileName[MAX_PATH];
	char FileExt[MAX_PATH];

	_splitpath(szPath,Drive,Dir,FileName,FileExt);

	szDrive=Drive;
	szDir=Dir;
	szFileName=FileName;
	szFileExt=FileExt;
}

void SplitPathW(CStringW szPath,CStringW& szDrive,CStringW& szDir,CStringW& szFileName,CStringW& szFileExt)
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t FileExt[MAX_PATH];

	_wsplitpath(szPath,Drive,Dir,FileName,FileExt);

	szDrive=Drive;
	szDir=Dir;
	szFileName=FileName;
	szFileExt=FileExt;
}

CStringA ChangeExtA(CStringA szFileName,CStringA szExt)
{
	CStringA szDrive,szDir,szFile,szFileExt;
	SplitPathA(szFileName,szDrive,szDir,szFile,szFileExt);

	return szDrive+szDir+szFile+szExt;
}

CStringW ChangeExtW(CStringW szFileName,CStringW szExt)
{
	CStringW szDrive,szDir,szFile,szFileExt;
	SplitPathW(szFileName,szDrive,szDir,szFile,szFileExt);

	return szDrive+szDir+szFile+szExt;
}

CStringA ChangeFileNameA(CStringA szFileName,CStringA newFileName){
	CStringA szDrive,szDir,szFile,szFileExt;
	SplitPathA(szFileName,szDrive,szDir,szFile,szFileExt);

	return szDrive+szDir+newFileName;
}

CStringW ChangeFileNameW(CStringW szFileName,CStringW newFileName){
	CStringW szDrive,szDir,szFile,szFileExt;
	SplitPathW(szFileName,szDrive,szDir,szFile,szFileExt);

	return szDrive+szDir+newFileName;
}

CStringA GetAbsolutePathA(CStringA FileName)
{
	char szModulePath[MAX_PATH];

	::GetModuleFileNameA(NULL,szModulePath,MAX_PATH);

	CStringA szDrive,szDir,szFileName,szFileExt;
	SplitPathA(szModulePath,szDrive,szDir,szFileName,szFileExt);

	return szDrive+szDir+FileName;
}

CStringW GetAbsolutePathW(CStringW FileName)
{
	wchar_t szModulePath[MAX_PATH];

	::GetModuleFileNameW(NULL,szModulePath,MAX_PATH);

	CStringW szDrive,szDir,szFileName,szFileExt;
	SplitPathW(szModulePath,szDrive,szDir,szFileName,szFileExt);

	return szDrive+szDir+FileName;
}

CStringW GetDirPathW(CStringW szFileName)
{
	CStringW szDrive,szDir,szFile,szFileExt;
	SplitPathW(szFileName,szDrive,szDir,szFile,szFileExt);

	return szDrive+szDir;
}

CStringA GetDirPathA(CStringA szFileName)
{
	CStringA szDrive,szDir,szFile,szFileExt;
	SplitPathA(szFileName,szDrive,szDir,szFile,szFileExt);

	return szDrive+szDir;
}

/************************************************************************
Obtain information about the system's current usage of both physical and virtual memory
************************************************************************/
CString GetSysMemInfo()
{
	CString temp;
	const DWORD divnum = 1024;
	MEMORYSTATUS stat;
	::GlobalMemoryStatus(&stat);
	CString formatstr="\tMemory loaded: %d%%\n"\
		"\tAvailPhys:%ldK\n"\
		"\tTotalVirtual:%ldK\n"\
		"\tAvailVirtual:%ldK";

	temp.Format( (LPCSTR)formatstr, 
		stat.dwMemoryLoad ,
		stat.dwAvailPhys / divnum,
		stat.dwTotalVirtual / divnum,
		stat.dwAvailVirtual /divnum	);
	return temp;
}
//-------------------------------------------------------------------------------------------------------------------------------

void SaveRawFile(CString strFileName,LPBYTE lpData,int byte,int nWidth,int nHeight)
{
	HANDLE hFile = CreateFile(strFileName,GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		AfxMessageBox(_T("Couldn't create the file!"));
	else
	{
		// Attach a CFile object to the handle we have.
		CFile myFile(hFile);
		myFile.Write(lpData, nWidth*nHeight*byte);
		myFile.Close();
	}
}

int CStringToWChar(CString strChar, wchar_t *pwstr)
{
	size_t nSize=0;
#ifndef UNICODE
	nSize = _mbslen((const unsigned char*)(LPCTSTR)strChar);
	if(pwstr!=NULL)
	{
		mbstowcs(pwstr,strChar,nSize);
		pwstr[nSize] = L'\0';
	}
#else
	nSize=strChar.GetLength();
	if(pwstr!=NULL)
	{
		for(int i=0;i<nSize;i++)
			pwstr[i]=strChar[i];	
		pwstr[nSize] = L'\0';
	}
#endif

	return int(nSize+1);
}

int WCharToCString(CString& strChar, wchar_t *pwstr)
{
	size_t nSize=0;
#ifndef UNICODE
	nSize = wcslen(pwstr);
	if(pwstr!=NULL)
	{
		nSize=wcstombs(NULL,pwstr,nSize);
		char* pBuffer=new char[nSize+1];
		wcstombs(pBuffer,pwstr,nSize);
		pBuffer[nSize] = '\0';
		strChar=pBuffer;
		delete[] pBuffer;
	}
#else
	strChar=pwstr;

#endif

	return int(nSize+1);
}

bool isValidDateChar(TCHAR c)
{
	return c==_T('.') || c==_T('-') || _istdigit(c);
}

bool isValidDate(LPCTSTR str)
{
	int count=0;

	size_t len=_tcslen(str);
	for(size_t i=0;i<len;i++)
	{
		if(!isValidDateChar(str[i]))
			return false;

		if(_istdigit(str[i]))
			count++;
	}

	if(count!=8)
		return false;

	return true;
}


COleDateTime ParseDateFromString(LPCTSTR str)
{
	ASSERT(str!=NULL);

	CString temp=str;

	temp.Replace(_T("-"),_T(""));
	temp.Replace(_T("."),_T(""));

	int year=0,month=1,day=1;
	_stscanf(LPCTSTR(temp),_T("%04d%02d%02d"),&year,&month,&day);
	if(year>9999)
		year=9999;
	if(year<100)
		year=100;
	if(month>12)
		month=12;
	if(month<1)
		month=1;
	if(day<1)
		day=1;

	return COleDateTime(year,month,day,0,0,0);
}

//str:HHMMSS
COleDateTime ParseTimeFromString(LPCTSTR str)
{
	ASSERT(str!=NULL);

	int hour=0,minute=1,second=1;
	_stscanf(str,_T("%02d%02d%02d"),&hour,&minute,&second);
	if(hour>23)
		hour=23;
	if(hour<0)
		hour=0;

	if(minute>59)
		minute=59;
	if(minute<0)
		minute=0;

	if(second>59)
		second=59;
	if(second<0)
		second=0;

	COleDateTime temp;
	temp.SetTime(hour,minute,second);

	return temp;
}

//str:YYYYMMDDHHMMSS
COleDateTime ParseDateTimeFromString(LPCSTR str)
{
	ASSERT(str!=NULL);

	int year=0,month=1,day=1;
	int hour=0,minute=0,second=0;
	_stscanf(str,_T("%04d%02d%02d%02d%02d%02d"),&year,&month,&day,&hour,&minute,&second);
	if(year>9999)
		year=9999;
	if(year<100)
		year=100;

	if(month>12)
		month=12;
	if(month<1)
		month=1;

	if(day<1)
		day=1;	

	if(hour>23)
		hour=23;
	if(hour<0)
		hour=0;

	if(minute>59)
		minute=59;
	if(minute<0)
		minute=0;

	if(second>59)
		second=59;
	if(second<0)
		second=0;

	return COleDateTime(year,month,day,hour,minute,second);
}

COleDateTime ParseDateTime(CString szDate,CString szTime)
{
	COleDateTime time;
	CString element;

	int year=0;
	int month=0;
	int day=0;
	int hour=0;
	int minute=0;
	int second=0;
	if(!szDate.IsEmpty())
	{
		element=szDate.Left(4);
		year=_ttoi(element);

		element=szDate.Mid(4,2);
		month=_ttoi(element);

		element=szDate.Right(2);
		day=_ttoi(element);
	}

	if(!szTime.IsEmpty())
	{
		element=szTime.Left(2);
		hour=_ttoi(element);

		element=szTime.Mid(2,2);
		minute=_ttoi(element);

		element=szTime.Right(2);
		second=_ttoi(element);
	}

	time.SetDateTime(year,month,day,hour,minute,second);

	return time;
}

CNullableType<long> ToNullableLong(CNullableType<unsigned long> m)
{
	CNullableType<long> ret;

	if(m.IsNull())
		ret=null;
	else
		ret=(long)(unsigned long)m;

	return ret;
}

void ShowAddressSpace()
{
	SYSTEM_INFO SysInfo;
	::GetSystemInfo(&SysInfo);

	CString s;
	CString strOut;

	strOut.Format("起始地址\t长度\t类型\t状态\n");
	//GET_LANG_RESOURECE(strOut,IDS_STRING_04304);

	LPBYTE lpBase=(LPBYTE)SysInfo.lpMinimumApplicationAddress;

	while(reinterpret_cast<size_t>(lpBase)<reinterpret_cast<size_t>(SysInfo.lpMaximumApplicationAddress))
	{
		MEMORY_BASIC_INFORMATION MemInfo;
		size_t dwBytes=::VirtualQuery(lpBase,&MemInfo,sizeof(MEMORY_BASIC_INFORMATION));

		CString str;
		switch(MemInfo.State)
		{
		case MEM_COMMIT:
			str="COMMIT";
			break;
		case MEM_FREE:
			str="FREE";
			break;
		case MEM_RESERVE:
			str="RESERVE";
			break;
		default:
			str="UNKNOWN";
			break;
		}

		if(MemInfo.State==MEM_FREE && MemInfo.RegionSize>=10*1024*1024)
		{
			s.Format("%x\t%gM\t%d\t%s\n",(LPBYTE)MemInfo.BaseAddress,1.0*MemInfo.RegionSize/1024/1024,MemInfo.Type,(LPCTSTR)str);
			strOut+=s;
		}

		lpBase+=MemInfo.RegionSize;
	}

	CString strTotal;
	double x1=(double)(size_t)(LPBYTE)SysInfo.lpMaximumApplicationAddress;
	double x2=(double)(size_t)(LPBYTE)SysInfo.lpMinimumApplicationAddress;

	strTotal.Format("%gG",(x1-x2)/1024/1024/1024);

	AfxMessageBox(strTotal);

	AfxMessageBox(strOut);
}

double InchToMM(double inch)
{
	return inch*25.4;
}

double MMToInch(double mm)
{
	return mm/25.4;
}

double FrenchToMM(double french)
{
	return french*0.335;
}

double MMToFrench(double mm)
{
	return mm/0.335;
}

double ChangeUnitToMM(double value,CString unit)
{
	unit.Trim();
	if(unit.CompareNoCase(_T("mm"))==0)
		return value;
	if(unit.CompareNoCase(_T("French"))==0)
		return FrenchToMM(value);
	if(unit.CompareNoCase(_T("inch"))==0)
		return InchToMM(value);
	return value;
}

CString CreateUID(CString strPrefix,CString strModality,CString strClientID)
{
	CString strUID;

	if(strPrefix.IsEmpty() || strModality.IsEmpty() || strClientID.IsEmpty())
		return strUID;

	CString szIP=GetHostIP();
	if(szIP.IsEmpty())
		return strUID;

	static int nID=1;
	strUID=strPrefix+_T(".")+strModality+_T(".")+strClientID+_T(".");
	CString strDate=COleDateTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	strUID+=strDate;
	strUID+=_T(".");

	CString strID;
	strID.Format("%d",nID);
	strUID+=strID;
	strUID+=_T(".");
	strUID+=szIP;
	nID++;

	return strUID;
}

CString GetHostIP()
{
	CString strIP;
	CString strF;

	WSADATA wsaData;
	char name[255];
	memset(name,0,sizeof(name));

	PHOSTENT hostinfo; 
	if(WSAStartup(MAKEWORD(2,0),&wsaData)==0)
	{  
		if(gethostname(name,sizeof(name))==0)
		{ 
			if((hostinfo=gethostbyname(name))!=NULL)	
			{//这些就是获得IP的函数
				strIP=inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
				strIP.Empty();
				strF.Format("%u",(*(struct in_addr *)*hostinfo->h_addr_list).S_un.S_un_b.s_b1);
				strIP+=strF;
				strF.Format("%u",(*(struct in_addr *)*hostinfo->h_addr_list).S_un.S_un_b.s_b2);
				strIP+=strF;
				strF.Format("%u",(*(struct in_addr *)*hostinfo->h_addr_list).S_un.S_un_b.s_b3);
				strIP+=strF;
				strF.Format("%u",(*(struct in_addr *)*hostinfo->h_addr_list).S_un.S_un_b.s_b4);
				strIP+=strF;
			}
		}
	}
	return strIP;
}

bool DeleteWholeDirectory(const char * dirname)
{
	WIN32_FIND_DATA fileinfo;
	HANDLE rtn;

	CString tempstr,nowpath;

	nowpath=dirname;
	if(nowpath.Right(1)!=_T("\\"))	
		nowpath+=_T("\\");

	if(_access(nowpath,0)) 
		return true;

	rtn=FindFirstFile(nowpath+_T("*.*"), &fileinfo);
	while(rtn!=INVALID_HANDLE_VALUE)
	{
		tempstr=nowpath+fileinfo.cFileName;
		if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if((_tcscmp(fileinfo.cFileName,_T("."))!=0) && (_tcscmp(fileinfo.cFileName,_T(".."))!=0))
			{
				if(!DeleteWholeDirectory(tempstr)) 
					return false;

				//改变文件夹的只读属性
				if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				{
					DWORD tempd=fileinfo.dwFileAttributes & (!FILE_ATTRIBUTE_READONLY);
					SetFileAttributes(tempstr,tempd);
				}
				//删除空文件夹:
				if(!RemoveDirectory(tempstr)) 
					return false;
			}
		}
		else
		{
			//改变文件的只读属性:
			if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY )
			{
				DWORD tempd = fileinfo.dwFileAttributes & (! FILE_ATTRIBUTE_READONLY);
				SetFileAttributes(tempstr,tempd);
			}
			if(_tunlink(tempstr))//_unlink函数:删除文件
				return false;
		}
		if(!FindNextFile(rtn,&fileinfo))
		{
			FindClose(rtn);
			break;
		}
	}
	//FindClose(rtn);
	return true;
}

bool DirectoryExists(const char * dirname)
{
	CString strDirName=dirname;
	if(!PathFileExists(strDirName))
		if(!CreateDirectory(strDirName,0))
			return false;

	return true;
}

bool CopyWholeDirectory(const CString &source, const CString &destination)
{
	WIN32_FIND_DATA fileinfo;
	HANDLE rtn;
	CString tempstr,nowpath,destdir,tempstr1,csTemp;

	nowpath = source;
	if (nowpath.Right(1) != "\\")
		nowpath+= "\\";
	if(_access(nowpath,0))
		return false;

	destdir=destination;
	if (destdir.Right(1) != "\\")
		destdir += "\\";

	if(_access(destdir,0))
	{
		if(!CreateWholeDirectory(destdir))  
			return false;
	}

	rtn=FindFirstFile(nowpath+_T("*.*"), &fileinfo);
	while (rtn != INVALID_HANDLE_VALUE)
	{
		tempstr=nowpath+fileinfo.cFileName;
		tempstr1=destdir+fileinfo.cFileName;
		if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if((strcmp(fileinfo.cFileName,".") != 0) && (strcmp(fileinfo.cFileName,"..") != 0))
			{
				if(!CopyWholeDirectory(tempstr,tempstr1))
					return false;
			}
		}
		else
		{
			if(!CopyFile(tempstr,tempstr1,FALSE))
			{
				DWORD DD=GetLastError();
				int n = 0;
			}
		}
		if(!FindNextFile(rtn,&fileinfo))
		{
			FindClose(rtn);
			break;
		}
	}
	return true;
}

bool CreateWholeDirectory(CString csFilePath)
{
	CString csTemp,csTemp1;

	char filePath[128];
	memset(filePath,'\0',sizeof(filePath));
	char *token;

	csTemp1=csFilePath;
	strcpy(filePath, csFilePath);//本地路径

	token=strtok(filePath, "\\");
	if(token!=NULL)
	{
		if(strlen(token) && token[strlen(token)-1]!=':')
			if(access(token,0) && !CreateDirectory(token, NULL))
				return false;
		csTemp=token ;

		while(token!=NULL)
		{
			token=strtok(NULL,"\\");
			if(token==NULL)
				break;
			csTemp += "\\";
			csTemp += token ;

			if(access(csTemp,0) && !CreateDirectory(csTemp, NULL))
				return false;
		}
	}
	else
	{
		if(!CreateDirectory(filePath, NULL))
			return false;
	}
	return true;
}

bool CalculateDirSize(const char * dirname, __int64& i64Size)
{
	WIN32_FIND_DATA fileinfo;
	HANDLE hret;         
	CString tempstr,filename,strDir;
	strDir=dirname;

	hret=FindFirstFile(strDir+_T("\\*.*"),&fileinfo);
	while(hret!=INVALID_HANDLE_VALUE)
	{
		filename=fileinfo.cFileName;
		if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if((filename!=_T(".")) && (filename!=_T("..")))
			{
				tempstr = dirname;
				if(tempstr.Right(1) != _T("\\")) 
					tempstr+=_T("\\");

				tempstr+=filename; 
				if(!CalculateDirSize(tempstr, i64Size)) 
					return false;
			}				 
		}
		else
		{
			i64Size+=(__int64)fileinfo.nFileSizeHigh*((__int64)MAXDWORD+1)+fileinfo.nFileSizeLow;
		}
		if(!FindNextFile(hret,&fileinfo))
		{
			FindClose(hret);
			break;
		}
	}

	return true;
}

bool CalculateFileSize(const char * filepath, __int64& i64Size)
{
	WIN32_FIND_DATA fileinfo;
	HANDLE hret;

	hret=FindFirstFile(filepath,&fileinfo);
	if(hret!=INVALID_HANDLE_VALUE)
	{
		i64Size+=(__int64)fileinfo.nFileSizeHigh*((__int64)MAXDWORD+1)+fileinfo.nFileSizeLow;
		FindClose(hret);
		return true;
	}
	else
		return false;
}

bool IsDirectoryEmpty(LPCTSTR lpszPath)
{
	CString szPath=lpszPath;
	szPath+=_T("\\*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(szPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
		return true;
	else 
	{
		bool ret=true;
		while(FindNextFile(hFind,&FindFileData))
		{
			if((strcmp(FindFileData.cFileName,".")==0) || (strcmp(FindFileData.cFileName,"..")==0))
				continue;
			else
			{
				ret=false;
				break;
			}
		}

		FindClose(hFind);
		return ret;
	}
}
/*
CString FromBirthdateToAge(CString sBirthDate)
{
CString sAge;

CTime tm = CTime::GetCurrentTime();
CString sToday	= tm.Format("%Y%m%d");
CString sYear   = tm.Format("%Y");
CString sMonth  = tm.Format("%m");
CString sDay    = tm.Format("%d");

int nYear, nMonth, nDay;
if (sBirthDate.GetLength() >= 8)
{
nYear  = atoi(sYear) - atoi(sBirthDate.Left(4));
nMonth = atoi(sMonth) - atoi(sBirthDate.Mid(4,2));
nDay   = atoi(sDay) - atoi(sBirthDate.Right(2));

if (nYear > 120 || nYear < 0)
{
sAge = _T("");
}
else if (nYear > 0)
{
sAge.Format("%03dY", nYear);
}
else if (nMonth > 0)
{
sAge.Format("%03dM", nMonth);
}
else if (nDay >= 0)
{
sAge.Format("%03dD", nDay);
}
else
{
sAge = _T("");
}
}

return sAge;
}*/

/*CString FromBirthdateToAge(CString sBirthDate, CString sStudyDate)
{
CString sAge;

CString sYear_S   = sStudyDate.Left(4);
CString sMonth_S  = sStudyDate.Mid(4,2);
CString sDay_S    = sStudyDate.Right(2);

int nYear_S  = atoi(sYear_S);
int nMonth_S = atoi(sMonth_S);
int nDay_S   = atoi(sDay_S);

CString sYear_B   = sBirthDate.Left(4);
CString sMonth_B  = sBirthDate.Mid(4,2);
CString sDay_B    = sBirthDate.Right(2);

int nYear_B  = atoi(sYear_B);
int nMonth_B = atoi(sMonth_B);
int nDay_B   = atoi(sDay_B);

if (sYear_S.IsEmpty() || sMonth_S.IsEmpty() || sDay_S.IsEmpty()
|| sYear_B.IsEmpty() || sMonth_B.IsEmpty() || sDay_B.IsEmpty())
{
return sAge;
}

COleDateTime dt1(nYear_B, nMonth_B, nDay_B, 0, 0, 0);

COleDateTime dt2(nYear_S, nMonth_S, nDay_S, 0, 0, 0);

COleDateTimeSpan span = dt2 - dt1;

int nDays = (int)(span.GetTotalDays());

int nYear = nYear_S - nYear_B;// nDays/365;
int nMonth = nDays/30;
int nDay   = nDays;

if (nYear > 120 || nYear < 0)
{
sAge = _T("");
}
else if (nYear > 0)
{
sAge.Format("%03dY", nYear);

if (nMonth_B > nMonth_S)
{
sAge.Format("%03dY", nYear-1);
}
else if (nMonth_B == nMonth_S)
{
if (nDay_B > nDay_S)
{
sAge.Format("%03dY", nYear-1);
}
}
}
else if (nMonth > 0)
{
sAge.Format("%03dM", nMonth);
}
else if (nDay >= 0)
{
sAge.Format("%03dD", nDay);
}
else
{
sAge = _T("");
}

return sAge;
}*/

char IsLeapYear(int year)
{
	if (((0 == year%4)&&(0 != year%100)) || (0 ==year%400))
		return 1;
	else
		return 0;
}


int Getdays(int year,int month)
{
	int days;

	if (0 == month)
	{
		month = 12;
	}

	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		days = 31;
		break;

	case 4:
	case 6:
	case 9:
	case 11:
		days = 30;
		break;

	case 2:
		days = 28 + IsLeapYear(year);
		break;
	default:
		days = -1;
		break;
	}

	return days;
}

bool VerifyDate(int lMonth,int lDay,int lYear)
{
	const unsigned char LeapDay[12] =  
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	const unsigned char NotLeapDay[12] = 
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if(lYear < 1900 )
	{
		return false;
	}

	if( (lMonth > 12) || (lMonth <= 0) )
	{
		return false;
	}

	if(IsLeapYear(lYear))
	{
		/*is leap year*/
		if( (lDay > LeapDay[lMonth-1]) || (lDay <= 0) )
		{
			return false;
		}
	}
	else
	{
		/*Is Not leap*/
		if( (lDay > NotLeapDay[lMonth-1]) || (lDay <= 0) )
		{
			return false;
		}
	}

	return true;
}

CString FromBirthdateToAge(CString sBirthDate, CString sStudyDate)  //LiSY
{
	CString sAge = _T("");

	CString sYear_S   = sStudyDate.Left(4);
	CString sMonth_S  = sStudyDate.Mid(4,2);
	CString sDay_S    = sStudyDate.Right(2);

	int nYear_S  = atoi(sYear_S);
	int nMonth_S = atoi(sMonth_S);
	int nDay_S   = atoi(sDay_S);

	CString sYear_B   = sBirthDate.Left(4);
	CString sMonth_B  = sBirthDate.Mid(4,2);
	CString sDay_B    = sBirthDate.Right(2);

	int nYear_B  = atoi(sYear_B);
	int nMonth_B = atoi(sMonth_B);
	int nDay_B   = atoi(sDay_B);

	int nYear;
	int nMonth;
	int nDay;
	int temp;

	if ((!VerifyDate(nMonth_S,nDay_S,nYear_S)) || (!VerifyDate(nMonth_B,nDay_B,nYear_B)))
	{
		return sAge;
	}


	nDay = nDay_S - nDay_B;
	if (nDay<0)
	{
		temp = Getdays(nYear_S,nMonth_S-1);
		if (temp < 0)
		{
			return sAge;
		}
		nDay = nDay + temp;
		nMonth_S = nMonth_S - 1;
	} 

	nMonth = nMonth_S - nMonth_B;
	if (nMonth<0)
	{
		nMonth = nMonth + 12;
		nYear_S = nYear_S - 1;
	}

	nYear = nYear_S - nYear_B;
	if (nYear > 120 || nYear < 0)
	{
		sAge = _T("");
	}
	else if (nYear > 0)
	{
		sAge.Format("%03dY", nYear);
	}
	else if (nMonth > 0)
	{
		sAge.Format("%03dM", nMonth);
	}
	else if (nDay >= 0)
	{
		if (0 == nDay)
		{
			nDay =1; //当天出生也显示为出生一天！ 
		}
		sAge.Format("%03dD", nDay);
	}
	else
	{
		sAge = _T("");
	}

	return sAge;
}

float GetFloatFromString(CString str)
{
	float ret(0);
#ifndef _UNICODE
	char* pChar = (LPSTR)(LPCSTR)(str);
	ret = (float)atof(pChar);
#else
	wchar_t* pChar = LPCTSTR(str);
	ret = _wtof(pChar);
#endif	
	return ret;
}

int GetIntFromString(CString str)
{
	int ret(0);
#ifndef _UNICODE
	char* pChar = (LPSTR)(LPCSTR)(str);
	ret = atoi(pChar);
#else
	wchar_t* pChar = LPCTSTR(str);
	ret = _wtoi(pChar);
#endif	
	return ret;
}

void SplitString(const CString& strSource, const CString& strSeparator, CStringList& strList)
{
	strList.RemoveAll();
	CString strTemp = strSource;
	int nFindStart = 0;
	CString strToken = strTemp.Tokenize(strSeparator, nFindStart);
	while (strToken != _T(""))
	{
		strList.AddTail(strToken);
		strToken = strTemp.Tokenize(strSeparator, nFindStart);
	}
}