#pragma once
#include "Utility.h"

#define UI_LangFile_Directory 		GetAbsolutePath(_T("Lang.ini"));\
	CString UI_LanguageFile_Directory__cstrFilePath=GetAbsolutePath(_T("setting.ini"));\
	CString UI_LanguageFile_Directory__cstrDrive(_T(""));\
	CString UI_LanguageFile_Directory__cstrPath(_T(""));\
	CString UI_LanguageFile_Directory__cstrFile(_T(""));\
	CString UI_LanguageFile_Directory__cstrExt(_T(""));\
	SplitPath(UI_LanguageFile_Directory__cstrFilePath,UI_LanguageFile_Directory__cstrDrive,UI_LanguageFile_Directory__cstrPath,UI_LanguageFile_Directory__cstrFile,UI_LanguageFile_Directory__cstrExt);\
	UI_LanguageFile_Directory__cstrFile =  UI_LanguageFile_Directory__cstrDrive + UI_LanguageFile_Directory__cstrPath;\
	if(!PathIsDirectory(UI_LanguageFile_Directory__cstrFile)){\
	CreateDirectory(UI_LanguageFile_Directory__cstrFile, NULL);\
	}
#define MAX_CHAR_LENGTH	256

CString InitLangString(CString szID,CString szSection = _T("String"));
bool IsExitLangString(CString szID,CString szSection = _T("String"));
void SetLangString(CString szID,CString szValue,CString szSection = _T("String"));
void InitLangDialog(CDialog *pDlg,UINT uDlgID);
void InitLangMenu(CMenu* menu,CString = _T("Default"));
BOOL IsChineseLangSystem();
BOOL ISChineseName();
#define GET_LANG_RESOURECE(strLang,a)   {CString strLang##a;\
	strLang##a.Format("%d",a);\
	strLang = InitLangString(strLang##a);\
	if(_tcscmp(strLang,_T(""))  == 0&&(IsExitLangString(strLang##a)==false)){\
     strLang.LoadString(a);\
	 SetLangString(strLang##a,strLang);}\
    }