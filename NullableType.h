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
 * C++异常。
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
 * 表示数据库中常用的空值（NULL）。
 * 
 * @author Qiu
 * @version 1.0
 * @since 2005-04-11
 */
class CNull
{
public:
    /**
     * 缺省构造函数。
     * @since 2005-04-11
     */
	CNull(){}
};

/**
 * 定义为空值。用法与NULL类似。
 * @since 2005-04-11
 */
#define null CNull()

/**
 * 可为空值的数据类型。
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
     * 空值的数据类型。
     */
	typedef T ValueType;
	
public:
    /**
     * 缺省构造函数。
     * 
     * 初始化对象为空值。
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
     * 构造函数。
     * 
     * 将对象初始化为空值。
     * 
     * @param n      CNull类型，表示将对象设为空值。
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
     * 构造函数。
     * 
     * 将对象初始化为指定类型的非空值。
     * 
     * @param a      指定数据类型的非空值。
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
     * 赋值函数。
     * 
     * 将其他对象的值拷贝到当前对象。
     * 
     * @param AValue 被拷贝的对象。
     * 
     * @return 当前对象的引用。
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
     * 设置当前对象为空值。
     * 
     * @param ANull  CNull类型，表示将对象设为空值。
     * 
     * @return 当前对象的引用。
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
     * 设置当前对象为指定类型的非空值。
     * 
     * @param t      指定数据类型的非空值。
     * 
     * @return 当前对象的引用。
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
     * 将当前对象隐式转换为指定的非空值类型。
     * 
     * 如果当前对象为空值，将抛出EValueIsNull异常。
     * 
     * @return 指定的非空值对象。
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
	* 判断当前对象是否为空值。
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
	* 判断当前对象是否为空值。
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
