﻿/*
	Copyright (C) by Franksoft 2010.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file yref.hpp
\ingroup Adaptor
\brief 用于提供指针和引用访问的间接访问类模块。
\version 0.2922;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2010-03-21 23:09:06 + 08:00;
\par 修改时间:
	2010-12-23 11:28 + 08:00;
\par 字符集:
	UTF-8;
\par 模块名称:
	YSLib::Adaptor::YReference;
*/


#ifndef INCLUDED_YREF_HPP_
#define INCLUDED_YREF_HPP_

#include "yadaptor.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Design)

YSL_BEGIN_NAMESPACE(Policies)

/*!
\brief 智能指针所有权策略：简单所有权策略。
\note 类似内建指针。无所有权语义支持。无数据成员，可以减小指针体积，
	可能使用 reinterpret_cast 储存至 32 位整数对象。
*/
template<class P>
struct RawOwnership
{
	enum
	{
		destructiveCopy = false
	};

	/*!
	\brief 构造。
	*/
	inline
	RawOwnership()
	{}

	/*!
	\brief 复制构造。
	*/
	template<class P1>
	inline
	RawOwnership(const RawOwnership<P1>& rhs)
	{}

	/*!
	\brief 复制。
	*/
	inline static P
	Clone(const P& val)
	{
		return val;
	}

	/*!
	\brief 释放。
	*/
	inline static bool
	Release(const P&)
	{
		return false;
	}

	/*!
	\brief 交换。
	*/
	inline static void
	Swap(RawOwnership&)
	{}

	/*!
	\brief 合并。
	*/
	template<class P1>
	inline static bool
	Merge(RawOwnership<P1>&)
	{
		return false;
	}
};


/*!
\brief 使用 ystdex::general_cast 的引用计数所有权策略。
*/
template <class P>
class GeneralCastRefCounted
{
public:
	enum
	{
		destructiveCopy = false
	};

private:
	uintptr_t* pCount_;

public:
	GeneralCastRefCounted()
		: pCount_(static_cast<uintptr_t*>(
		SmallObject<>::operator new(sizeof(uintptr_t))))
	{
		assert(pCount_);
		*pCount_ = 1;
	}
	GeneralCastRefCounted(const GeneralCastRefCounted& rhs)
		: pCount_(rhs.pCount_)
	{}
	template <typename P1>
	GeneralCastRefCounted(const GeneralCastRefCounted<P1>& rhs)
		: pCount_(reinterpret_cast<const GeneralCastRefCounted&>(rhs).pCount_)
	{}

	P
	Clone(const P& val)
	{
		++*pCount_;
		return val;
	}

	template <typename P1>
	P
	Clone(const P1& val)
	{
		P r(ystdex::general_cast<P>(val));

		++*pCount_;
		return r;
	}

	bool
	Release(const P&)
	{
		if(!--*pCount_)
		{
			SmallObject<>::operator delete(pCount_, sizeof(uintptr_t));
			pCount_ = NULL;
			return true;
		}
		return false;
	}

	void
	Swap(GeneralCastRefCounted& rhs)
	{
		std::swap(pCount_, rhs.pCount_);
	}
};

YSL_END_NAMESPACE(Policies)

YSL_END_NAMESPACE(Design)


//! \brief 资源指针。
template<typename T,
	class OP = Policies::TwoRefCounts,
	class CP = Policies::DisallowConversion,
	template<class> class KP = Policies::AssertCheck,
	template<class> class RP = Policies::CantResetWithStrong,
	template<class> class DP = Policies::DeleteSingle,
	typename SPType = StrongPtr<T, true, OP, CP, KP, RP, DP> >
class GHStrong : public SPType
{
public:
	/*!
	\brief 构造：使用内建指针。
	*/
	GHStrong(T* p = NULL)
		: SPType(p)
	{};
	/*!
	\brief 构造：使用对象引用。
	*/
	GHStrong(T& rhs)
		: SPType(rhs)
	{}
	/*!
	\brief 构造：使用智能指针引用。
	*/
	GHStrong(RefToValue<GHStrong> rhs)
		: SPType(rhs)
	{}

	/*!
	\brief 取内建指针。
	*/
	T*
	operator->() const
	{
		return GHStrong<T>::GetPointer();
	}

	/*!
	\brief 释放指针。
	\note Policies::CantResetWithStrong 策略决定
		无法释放所有指向同一对象的强指针；这里仅影响单独的 GHStrong 对象实例。
	*/
	T*
	Release()
	{
		T* p(this->operator->());

		*this = NULL;
		return p;
	}

	/*!
	\brief 复位指针。
	\note Policies::CantResetWithStrong 策略决定
		无法复位所有指向同一对象的强指针；这里仅影响单独的 GHStrong 对象实例。
	*/
	bool
	Reset(T* p = NULL)
	{
		if(this->operator->() != p)
		{
			GHStrong hTemp(NULL);

			this->Swap(hTemp);
			return true;
		}
		return false;
	}
};


//! \brief 资源引用指针。
template<typename T,
	class OP = Policies::TwoRefCounts,
	class CP = Policies::DisallowConversion,
	template<class> class KP = Policies::AssertCheck,
	template<class> class RP = Policies::CantResetWithStrong,
	template<class> class DP = Policies::DeleteSingle,
	typename SPType = StrongPtr<T, false, OP, CP, KP, RP, DP> >
class GHWeak : public SPType
{
public:
	/*!
	\brief 构造：使用内建指针。
	*/
	GHWeak(T* p = NULL)
		: SPType(p)
	{}
	/*!
	\brief 构造：使用对象引用。
	*/
	GHWeak(T& rhs)
		: SPType(rhs)
	{}
	/*!
	\brief 构造：使用强指针。
	*/
	GHWeak(GHStrong<T> p)
		: SPType(p)
	{}
	/*!
	\brief 构造：使用强指针引用。
	*/
	GHWeak(RefToValue<GHStrong<T> > rhs)
		: SPType(rhs)
	{}
	/*!
	\brief 构造：使用弱指针引用。
	*/
	GHWeak(RefToValue<GHWeak> rhs)
		: SPType(rhs)
	{}

	/*!
	\brief 取内建指针。
	*/
	T*
	operator->() const
	{
		return GHWeak<T>::GetPointer();
	}

	/*!
	\brief 释放指针。
	\note Policies::CantResetWithStrong 策略决定可以释放弱指针；
		会影响所有指向同一对象的 GHStrong 和 GHWeak 的对象实例。
	*/
	T*
	Release()
	{
		T* p(NULL);

		ReleaseAll(static_cast<SPType&>(*this), p);
		return p;
	}

	/*!
	\brief 复位指针。
	\note Policies::CantResetWithStrong 策略决定可以复位弱指针；
		会影响所有指向同一对象的 GHStrong 和 GHWeak 的对象实例。
	*/
	bool
	Reset(T* p = NULL)
	{
		return ResetAll(static_cast<SPType&>(*this), p);
	}
};


//! \brief 句柄类。
template<typename T,
	template<class> class OP = Design::Policies::RawOwnership,
	class CP = Policies::DisallowConversion,
	template<class> class KP = Policies::AssertCheck,
	template<class> class SP = Policies::DefaultSPStorage,
	typename SPType = SmartPtr<T, OP, CP, KP, SP> >
class GHHandle : public SPType
{
public:
	/*!
	\brief 构造：使用内建指针。
	*/
	GHHandle(T* p = NULL)
		: SPType(p)
	{}
	/*!
	\brief 构造：使用对象引用。
	*/
	GHHandle(T& rhs)
		: SPType(rhs)
	{}
	/*!
	\brief 构造：使用句柄引用。
	*/
	GHHandle(RefToValue<GHHandle> rhs)
		: SPType(rhs)
	{}
	/*!
	\brief 构造：使用其它类型句柄引用。
	*/
	template<class C>
	inline explicit
	GHHandle(const GHHandle<C>& h)
		: SPType(h)
	{}

	/*!
	\brief 转换：句柄引用。
	*/
	operator RefToValue<GHHandle>()
	{
		return RefToValue<GHHandle>(*this);
	}

	/*!
	\brief 取内建指针。
	*/
	T*
	operator->() const
	{
		return GHHandle<T>::GetPointer(*this);
	}

	/*!
	\brief 释放指针。
	*/
	T*
	Release()
	{
		T* p(this->operator->());

		*this = NULL;
		return p;
	}

	/*!
	\brief 复位指针。
	*/
	bool
	Reset(T* p = NULL)
	{
		if(p != operator->())
		{
			delete operator->();
			*this = p;
			return true;
		}
		return false;
	}
};


/*!	\defgroup GetPointer GetPointer
\brief 取内建指针。
*/
//@{
template<typename _type>
inline _type*
GetPointer(_type* h)
{
	return h;
}
template<typename _type>
inline _type*
GetPointer(SmartPtr<_type> h)
{
	return SmartPtr<_type>::GetPointer(h);
}
template<class _type>
inline _type*
GetPointer(GHStrong<_type> h)
{
	return h.operator->();
}
template<class _type>
inline _type*
GetPointer(GHWeak<_type> h)
{
	return h.operator->();
}
template<typename _type>
inline _type*
GetPointer(GHHandle<_type> h)
{
	return h.operator->();
}
//@}

/*!
\brief 转换：指定类型句柄为整数。
*/
template<typename _tIntegral, typename _tReference>
inline _tIntegral
handle2int(GHHandle<_tReference> h)
{
	return reinterpret_cast<_tIntegral>(GetPointer(h));
}

/*!
\brief 转换：指定类型整数为句柄。
*/
template<typename _tIntegral, typename _tReference>
inline GHHandle<_tReference>
handle2int(_tIntegral i)
{
	return GHHandle<_tReference>(reinterpret_cast<_tIntegral>(i));
}

/*!
\brief 转换：指定类型句柄。
*/
template<typename _type, typename _tReference>
inline GHHandle<_type>
static_handle_cast(const GHHandle<_tReference>& h)
{
	return GHHandle<_type>(h);
}

/*!
\brief 转换：指定类型句柄。
*/
template<typename _type, typename _tReference>
GHHandle<_type>
dynamic_handle_cast(GHHandle<_tReference> h)
{
	GHHandle<_type> hDst(dynamic_cast<_type*>(GetPointer(h)));

	hDst.Merge(h);
	return hDst;
}


/*!	\defGroup YReset YReset
\brief 安全删除指定引用的句柄指向的对象。
\post 指定引用的句柄值等于 NULL 。
*/
//@{
template<typename _type>
inline bool
YReset(_type*& h) ythrow()
{
	bool b(h);

	delete h;
	h = NULL;
	return b;
}
//! \warning 句柄不会被自动回收，需要手动释放。
template<typename _type>
inline bool
YReset(GHHandle<_type>& h) ythrow()
{
	return h.Reset();
}
template<typename _type>
inline bool
YReset(GHStrong<_type>& h) ythrow()
{
	return h.Reset();
}
template<typename _type>
inline bool
YReset(GHWeak<_type>& h) ythrow()
{
	return h.Reset();
}

#ifdef YSL_USE_MEMORY_DEBUG

template<typename _type>
inline bool
YDelete_Debug(_type*& h) ythrow()
{
	bool b(h);

	ydelete(h);
	h = NULL;
	return b;
}

#else
#	define YDelete_Debug YReset
#endif

//@}

YSL_END

#endif

