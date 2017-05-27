#include "StdAfx.h"
#include ".\multilanguage.h"
/**当前是否是中文系统
*/
int gBChineseLang = -1;
BOOL IsChineseLangSystem()
{
	if(gBChineseLang < 0)
	{
		LCID localID = GetSystemDefaultLCID();
		int PrimaryID = localID & 0x000003FF;
		TCHAR rett[8];
		GetLocaleInfo(localID,LOCALE_IDEFAULTANSICODEPAGE,rett,8);
#ifndef _UNICODE
		int codePage = atoi(rett);
#else
		int codePage = _wtoi(rett);
#endif
		if (PrimaryID == 4 && 936 == codePage)
		{
			gBChineseLang = 1;
		}
		else
		{
			gBChineseLang = 0;
		}
	}
	return gBChineseLang;
}

BOOL ISChineseName()
{
	CString strConfigFile;
	BOOL btemp;
	strConfigFile = GetAbsolutePath(_T("config\\Exploiter.ini"));
	btemp = ::GetPrivateProfileInt(_T("Playback"), _T("ChineseSexName"), 1, strConfigFile);
	return btemp;
}
/*********************************************************************
* 函数名称:InitLangString
* 说明:	根据标识 szKey到Lang.ini语言文件中加载字符串
* 作者:	ljj
*********************************************************************/
CString InitLangString(CString szKey,CString szSection)
{
	CString szDefault = _T("*_w");
	//CString szSection = _T("String");
	TCHAR szValue[MAX_CHAR_LENGTH];
	CString strReturn;
	CString strPath = UI_LangFile_Directory;
	//szValue.Replace("\\n","\n");	//替换回换行符号
	DWORD size =GetPrivateProfileString(szSection,szKey,szDefault,szValue,MAX_CHAR_LENGTH,strPath);
	if( size!= 0 &&(_tcscmp(szValue,_T("*_w"))  != 0))
	{
		
		strReturn= szValue;
		strReturn.Replace("\\n","\n");
		strReturn.Replace("\\r","\r");
		
	}
	else
	{
		strReturn=_T("");
		//WritePrivateProfileString(szSection,szKey,strReturn,strPath);
	}
	return strReturn;

}
/*********************************************************************
* 函数名称:SetLangString
* 说明:	设置szKey的szValue到Lang.ini语言文件中
* 作者:	ljj
*********************************************************************/
void SetLangString(CString szKey,CString szValue,CString szSection )
{
	//CString szSection = _T("String");
	CString strPath = UI_LangFile_Directory;
    CString strTemp(szValue);
	strTemp.Replace("\n","\\n");
	strTemp.Replace("\r","\\r");
	WritePrivateProfileString(szSection,szKey,strTemp,strPath);
}
/*********************************************************************
* 函数名称:IsExitLangString
* 说明:	根据标识 szKey判断Lang.ini语言文件中是否存在szKey行
* 作者:	ljj
*********************************************************************/
bool IsExitLangString(CString szKey,CString szSection)
{
	CString szDefault = _T("*_w");
	//CString szSection = _T("String");
	TCHAR szValue[MAX_CHAR_LENGTH];
	CString strReturn;
	CString strPath = UI_LangFile_Directory;
	//szValue.Replace("\\n","\n");	//替换回换行符号
	DWORD size =GetPrivateProfileString(szSection,szKey,szDefault,szValue,MAX_CHAR_LENGTH,strPath);
	bool suc;
	if( (_tcscmp(szValue,_T("*_w"))  != 0))
	{
		suc=true;;
	}
	else
	{
		suc=false;
	}
	return suc;

}
/*********************************************************************
* 函数名称:InitLangDialog(CDialog *pDlg,UINT uDlgID)
* 说明:	 对话框运行时从Lang.ini获取其所有可得到的字符串，如果无法获得字符串以本身字符串默认并保存到Lang.ini语言文件中，每个控件用对话框ID和控件ID作为唯一标识

* 入口参数:
* CDialog *pDlg -- 对话框的指针
*  UINT uDlgID -- 该对话框的ID
* 作者: ljj 
*********************************************************************/
void InitLangDialog(CDialog *pDlg,UINT uDlgID)
{
	CString szSection = "String";
	CRuntimeClass* prt =pDlg->GetRuntimeClass();
	szSection=prt->m_lpszClassName;
	CString szKey,szText;
	CString szDefault = _T("*_w");
	DWORD dwSize = 1000;
	char* pData = (char*)malloc(dwSize);
	//读对话框标题
	szKey.Format("IDD%d_CAPTION",uDlgID);
	CString strPath = UI_LangFile_Directory;
	DWORD size =GetPrivateProfileString(szSection,szKey,szDefault,pData,dwSize,strPath);
	if( (_tcscmp(pData,_T("*_w"))  != 0))
	{
		pDlg->SetWindowText(pData);
	}
	else
	{
		pDlg->GetWindowText(szText);
		WritePrivateProfileString(szSection,szKey,szText,strPath);
	}
	//写入各个子控件的标题文字
	CWnd* pWnd = pDlg->GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		CRuntimeClass* prt=pWnd->GetRuntimeClass();
		// 对于CComboBox和CEdit 不进行标题文字控制
		if(prt->IsDerivedFrom(RUNTIME_CLASS(CComboBox))|| 
			prt->IsDerivedFrom(RUNTIME_CLASS(CEdit))|| 
			prt->IsDerivedFrom(RUNTIME_CLASS(CIPAddressCtrl))|| 
			prt->IsDerivedFrom(RUNTIME_CLASS(CRichEditCtrl))||
			prt->IsDerivedFrom(RUNTIME_CLASS(CProgressCtrl))
			)
		{
		}
		else
		{
			szKey.Format("IDD%d_%d",uDlgID,pWnd->GetDlgCtrlID());
			size =GetPrivateProfileString(szSection,szKey,szDefault,pData,dwSize,strPath);
			if( (_tcscmp(pData,_T("*_w"))  != 0))
			{
				pWnd->SetWindowText(pData);
			}
			else
			{
				pWnd->GetWindowText(szText);
				WritePrivateProfileString(szSection,szKey,szText,strPath);
			}
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	//释放内存
	free(pData);

}
/*********************************************************************
* 函数名称:InitLangMenu(CMenu* menu)
* 说明:	当菜单运行时获取其Lang.ini语言文件中可得到的字符串，如果不存在则保存到Lang.ini语言文件中
* 入口参数:
* CMenu* menu -- 菜单的指针
* 作者: ljj 
*********************************************************************/
void InitLangMenu(CMenu* menu,CString Name)
{
	if(menu==NULL)
	{
		return;
	}
	int nCount = menu->GetMenuItemCount();//获取该层菜单数
	for(int i=0;i<nCount;i++)//遍历所菜单项
	{
		int  id = menu->GetMenuItemID(i);//查看菜单属性id=0为分割线-1为popup菜单
		if(id == 0)
		{
			continue;
		}
		else 
		{
			CString strPath = UI_LangFile_Directory;
			CString strText;
			//CRuntimeClass* prt = menu->
			CRuntimeClass* prt = menu ->GetRuntimeClass();
			strText  = prt->m_lpszClassName;
			TCHAR str[MAX_CHAR_LENGTH];
			CString szKey;
			if(id!=-1)
			{
			   szKey.Format("IDD%d_MENU_%d",id,i);
			}
			else
			{
               szKey.Format("IDD%d_MENU_%d_%s",id,i,Name);
			}
			DWORD size = ::GetPrivateProfileString(strText,szKey,_T("*_w"),str,MAX_CHAR_LENGTH,strPath);
			if (_tcscmp(str,_T("*_w"))  == 0)
			{
				CString strMenuText;
				menu->GetMenuString (i,strMenuText,MF_BYPOSITION);//获取菜单文字
				::WritePrivateProfileString(strText,szKey,strMenuText,strPath);
			}
			else
			{
				menu->ModifyMenu(i,MF_STRING|MF_BYPOSITION,id,str);
			}
			if(id==-1)
			{
				CMenu* pSub = menu->GetSubMenu(i);
				InitLangMenu(pSub);//递归遍历popup菜单
			}
		}

	}
}