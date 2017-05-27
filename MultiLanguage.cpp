#include "StdAfx.h"
#include ".\multilanguage.h"
/**��ǰ�Ƿ�������ϵͳ
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
* ��������:InitLangString
* ˵��:	���ݱ�ʶ szKey��Lang.ini�����ļ��м����ַ���
* ����:	ljj
*********************************************************************/
CString InitLangString(CString szKey,CString szSection)
{
	CString szDefault = _T("*_w");
	//CString szSection = _T("String");
	TCHAR szValue[MAX_CHAR_LENGTH];
	CString strReturn;
	CString strPath = UI_LangFile_Directory;
	//szValue.Replace("\\n","\n");	//�滻�ػ��з���
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
* ��������:SetLangString
* ˵��:	����szKey��szValue��Lang.ini�����ļ���
* ����:	ljj
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
* ��������:IsExitLangString
* ˵��:	���ݱ�ʶ szKey�ж�Lang.ini�����ļ����Ƿ����szKey��
* ����:	ljj
*********************************************************************/
bool IsExitLangString(CString szKey,CString szSection)
{
	CString szDefault = _T("*_w");
	//CString szSection = _T("String");
	TCHAR szValue[MAX_CHAR_LENGTH];
	CString strReturn;
	CString strPath = UI_LangFile_Directory;
	//szValue.Replace("\\n","\n");	//�滻�ػ��з���
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
* ��������:InitLangDialog(CDialog *pDlg,UINT uDlgID)
* ˵��:	 �Ի�������ʱ��Lang.ini��ȡ�����пɵõ����ַ���������޷�����ַ����Ա����ַ���Ĭ�ϲ����浽Lang.ini�����ļ��У�ÿ���ؼ��öԻ���ID�Ϳؼ�ID��ΪΨһ��ʶ

* ��ڲ���:
* CDialog *pDlg -- �Ի����ָ��
*  UINT uDlgID -- �öԻ����ID
* ����: ljj 
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
	//���Ի������
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
	//д������ӿؼ��ı�������
	CWnd* pWnd = pDlg->GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		CRuntimeClass* prt=pWnd->GetRuntimeClass();
		// ����CComboBox��CEdit �����б������ֿ���
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
	//�ͷ��ڴ�
	free(pData);

}
/*********************************************************************
* ��������:InitLangMenu(CMenu* menu)
* ˵��:	���˵�����ʱ��ȡ��Lang.ini�����ļ��пɵõ����ַ���������������򱣴浽Lang.ini�����ļ���
* ��ڲ���:
* CMenu* menu -- �˵���ָ��
* ����: ljj 
*********************************************************************/
void InitLangMenu(CMenu* menu,CString Name)
{
	if(menu==NULL)
	{
		return;
	}
	int nCount = menu->GetMenuItemCount();//��ȡ�ò�˵���
	for(int i=0;i<nCount;i++)//�������˵���
	{
		int  id = menu->GetMenuItemID(i);//�鿴�˵�����id=0Ϊ�ָ���-1Ϊpopup�˵�
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
				menu->GetMenuString (i,strMenuText,MF_BYPOSITION);//��ȡ�˵�����
				::WritePrivateProfileString(strText,szKey,strMenuText,strPath);
			}
			else
			{
				menu->ModifyMenu(i,MF_STRING|MF_BYPOSITION,id,str);
			}
			if(id==-1)
			{
				CMenu* pSub = menu->GetSubMenu(i);
				InitLangMenu(pSub);//�ݹ����popup�˵�
			}
		}

	}
}