// NullableType.h: interface for the CNullableType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NULLABLETYPE_H__6190647D_1F24_4519_B72D_2BD4ACAC9F9F__INCLUDED_)
#define AFX_NULLABLETYPE_H__6190647D_1F24_4519_B72D_2BD4ACAC9F9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afx.h"
 
#include <exception>
#include <iostream>
using namespace std;

/**
 * C++�쳣��
 * 
 * @author Qiu
 * @version 1.0
 * @see CNullableType
 * @since 2005-04-11
 */
class EValueIsNull : public std::exception
{
};

/**
 * ��ʾ���ݿ��г��õĿ�ֵ��NULL����
 * 
 * @author Qiu
 * @version 1.0
 * @since 2005-04-11
 */
class CNull
{
public:
    /**
     * ȱʡ���캯����
     * @since 2005-04-11
     */
	CNull(){}
};

/**
 * ����Ϊ��ֵ���÷���NULL���ơ�
 * @since 2005-04-11
 */
#define null CNull()

/**
 * ��Ϊ��ֵ���������͡�
 * 
 * @author Qiu
 * @version 1.0
 * @since 2005-04-11
 */
template <typename T>
class CNullableType  
{
public:
    /**
     * ��ֵ���������͡�
     */
	typedef T ValueType;
	
public:
    /**
     * ȱʡ���캯����
     * 
     * ��ʼ������Ϊ��ֵ��
     * 
     * @example CNullableType<double> d;
     * @since 2005-04-11
     */
	CNullableType()
	{	
		Value=T();
		mIsNull=TRUE;
	}
	
    /**
     * ���캯����
     * 
     * �������ʼ��Ϊ��ֵ��
     * 
     * @param n      CNull���ͣ���ʾ��������Ϊ��ֵ��
     * 
     * @example CNullableType<int> i(null);
     * @since 2005-04-11
     */
	CNullableType(const CNull& n)
	{	
		Value=T();
		mIsNull=TRUE;
	}

    /**
     * ���캯����
     * 
     * �������ʼ��Ϊָ�����͵ķǿ�ֵ��
     * 
     * @param a      ָ���������͵ķǿ�ֵ��
     * 
     * @example CNullableType<char*> str(NULL);
     * @since 2005-04-11
     */
	CNullableType(const T& a)
	{
		mIsNull=FALSE;	
		Value=a;
	}
	
    /**
     * ��ֵ������
     * 
     * �����������ֵ��������ǰ����
     * 
     * @param AValue �������Ķ���
     * 
     * @return ��ǰ��������á�
     * @example CNullableType<int> a(100);
     * CNullableType<int> b;
     * b=a;
     * @since 2005-04-11
     */
	CNullableType& operator=(const CNullableType& AValue)
	{
		if(AValue.mIsNull)
			mIsNull=TRUE;
		else
		{
			mIsNull=FALSE;
			Value=AValue.Value;
		}
		
		return *this;
	};
	
    /**
     * ���õ�ǰ����Ϊ��ֵ��
     * 
     * @param ANull  CNull���ͣ���ʾ��������Ϊ��ֵ��
     * 
     * @return ��ǰ��������á�
     * @example CNullableType<int> a(100);
     * a=null;
     * @since 2005-04-11
     */
	CNullableType& operator=(const CNull& ANull)
	{
		mIsNull=TRUE;
		return *this;
	};
	
    /**
     * ���õ�ǰ����Ϊָ�����͵ķǿ�ֵ��
     * 
     * @param t      ָ���������͵ķǿ�ֵ��
     * 
     * @return ��ǰ��������á�
     * @example CNullableType<int> a(100);
     * a=200;
     * @since 2005-04-11
     */
	CNullableType& operator=(const T& t)
	{
		mIsNull=FALSE;
		Value=t;		
		return *this;
	}

	friend ostream & operator<<(ostream &mo,const CNullableType& cc)
	{
		mo<<cc.mIsNull;
		mo<<cc.Value;
		return mo;
	};
    /**
     * ����ǰ������ʽת��Ϊָ���ķǿ�ֵ���͡�
     * 
     * �����ǰ����Ϊ��ֵ�����׳�EValueIsNull�쳣��
     * 
     * @return ָ���ķǿ�ֵ����
     * @example CNullableType<int> a(100);
     * int b;
     * b=a;
     * @since 2005-04-11
     */
	operator T() const
	{
		T s=T();
		//if(mIsNull)
		//	throw EValueIsNull();
		//else
		if(!mIsNull)		s=Value;		
		return s;
	};

	BOOL IsNull() const{
		return mIsNull;
	};

	/**
	* �жϵ�ǰ�����Ƿ�Ϊ��ֵ��
	* 
	* @param T
	* @param n
	* @param t
	* 
	* @return 
	* @example CNullableType<int> a(100);
	* if(null==a)
	* {}
	* @since 2005-04-11
	*/
	friend BOOL operator== (const CNull& n,const CNullableType& t)
	{
		if(t.IsNull())
			return TRUE;
		else
			return FALSE;
	};

	/**
	* �жϵ�ǰ�����Ƿ�Ϊ��ֵ��
	* 
	* @param T
	* @param t
	* @param n
	* 
	* @return 
	* @example CNullableType<int> a(100);
	* if(a==null)
	* {}
	* @since 2005-04-11
	*/
	friend BOOL operator== (const CNullableType& t,const CNull& n)
	{
		if(t.IsNull())
			return TRUE;
		else
			return FALSE;
	};

	friend CArchive& operator>>(CArchive& ar,CNullableType& item)
	{
		item.Serialize(ar);

		return ar;
	}

	friend CArchive& operator<<(CArchive& ar,CNullableType& item)
	{
		item.Serialize(ar);

		return ar;
	}

	void Serialize(CArchive& ar)
	{
		if(ar.IsStoring())
		{
			ar<<mIsNull;
			ar<<Value;
		}
		else
		{
			ar>>mIsNull;
			ar>>Value;
		}
	}

protected:
	BOOL mIsNull;
	T  Value;
};

#endif // !defined(AFX_NULLABLETYPE_H__6190647D_1F24_4519_B72D_2BD4ACAC9F9F__INCLUDED_)
