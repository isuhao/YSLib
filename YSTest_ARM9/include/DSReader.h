﻿/*
	Copyright (C) by Franksoft 2010 - 2011.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file DSReader.h
\ingroup YReader
\brief 适用于 DS 的双屏阅读器。
\version r2443;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2010-01-05 14:03:47 +0800;
\par 修改时间:
	2011-09-01 01:59 +0800;
\par 字符集:
	UTF-8;
\par 模块名称:
	YReader::DSReader;
*/


#ifndef INCLUDED_DSREADER_H_
#define INCLUDED_DSREADER_H_

#include <YSLib/Core/yapp.h>
#include <YSLib/Service/yftext.h>
#include <YSLib/UI/ydesktop.h>
#include <YSLib/Service/ytext.h>
#include <YSLib/Helper/yglobal.h>
#include <YSLib/UI/textarea.h>
#include <YSLib/Service/textmgr.h>

YSL_BEGIN

YSL_BEGIN_NAMESPACE(DS)

YSL_BEGIN_NAMESPACE(Components)

using Drawing::Color;
using Drawing::PixelType;

class BlockedText
{
public:
	TextFile& File; //!< 文本文件对象。
	Text::TextFileBuffer Blocks; //!< 文本缓存映射。

	explicit
	BlockedText(TextFile&);
};


class MDualScreenReader
{
private:
	BlockedText* pText; //!< 文本资源。
	FontCache& fc; //!< 字体缓存。
	Drawing::Rotation rot; //!< 屏幕指向。
	Text::TextFileBuffer::HText itUp; //!< 字符区域读取文本缓存迭代器。
	Text::TextFileBuffer::HText itDn; //!< 字符区域读取文本缓存迭代器。
	u8 lnHeight; //!< 行高。

public:
	YSLib::Components::TextArea AreaUp; //!< 上屏幕对应字符区域。
	YSLib::Components::TextArea AreaDown; //!< 下屏幕对应字符区域。

	/*!
	\brief 构造。
	\param w 字符区域宽。
	\param h_up 上字符区域高。
	\param h_down 下字符区域高。
	\param fc_ 字体缓存对象。
	*/
	MDualScreenReader(SDst w = MainScreenWidth,
		SDst h_up = MainScreenHeight, SDst h_down = MainScreenHeight,
		FontCache& fc_ = FetchGlobalInstance().GetFontCache());

	bool IsTextTop(); //!< 判断输出位置是否到文本顶端。
	bool IsTextBottom(); //!< 判断输出位置是否到文本底端。

	DefGetter(u8, FontSize, fc.GetFontSize()) //!< 取字符区域的字体大小。
	DefGetter(Color, ColorUp, AreaUp.Color) \
		//!< 取上字符区域的字体颜色。
	DefGetter(Color, ColorDown, AreaDown.Color) \
		//!< 取下字符区域的字体颜色。
	DefGetter(u8, LineGapUp, AreaUp.LineGap) //!< 取上字符区域的行距。
	DefGetter(u8, LineGapDown, AreaDown.LineGap) \
		//!< 取下字符区域的行距。
	DefGetter(Color, Color, GetColorUp()) //!< 取字符区域的字体颜色。
	DefGetter(u8, LineGap, GetLineGapUp()) //!< 取字符区域的行距。

private:
	DefSetterDe(PixelType, ColorUp, AreaUp.Color, 0) \
		//!< 设置上字符区域的字体颜色。
	DefSetterDe(PixelType, ColorDown, AreaDown.Color, 0) \
		//!< 设置下字符区域的字体颜色。
	DefSetterDe(u8, LineGapUp, AreaUp.LineGap, 0) \
		//!< 设置上字符区域的行距。
	DefSetterDe(u8, LineGapDown, AreaDown.LineGap, 0) \
		//!< 设置下字符区域的行距。

public:
	void
	SetColor(Color = Drawing::ColorSpace::Black); //!< 设置字符颜色。
	void
	SetFontSize(Drawing::Font::SizeType = Drawing::Font::DefaultSize); \
		//!< 设置字符区域字体大小。
	void
	SetLineGap(u8 = 0); //!< 设置行距。

	//设置笔的行位置。
	//void
	//SetCurrentTextLineNTo(u8);

	//! \brief 无效化文本区域。
	void
	Invalidate();

	//! \brief 上移一行。
	bool
	LineUp();

	//! \brief 下移一行。
	bool
	LineDown();

	//! \brief 载入文本。
	void
	LoadText(TextFile&);

	//! \brief 绘制上屏文本。
	void
	PrintTextUp(const Drawing::Graphics&);

	//! \brief 绘制下屏文本。
	void
	PrintTextDown(const Drawing::Graphics&);

	//! \brief 复位输出状态。
	void
	Reset();

	//! \brief 上移一屏。
	bool
	ScreenUp();

	//! \brief 下移一屏。
	bool
	ScreenDown();

	/*//!
	\brief 自动滚屏。
	\param pCheck 输入检测函数指针。
	*/
	//void
	//Scroll(Function<void()> pCheck);

	//! \brief 卸载文本。
	void
	UnloadText();

	//! \brief 更新缓冲区文本。
	void
	Update();
};

inline bool
MDualScreenReader::IsTextTop()
{
	return itUp == pText->Blocks.begin();
}
inline bool
MDualScreenReader::IsTextBottom()
{
	return itDn == pText->Blocks.end();
}

inline void
MDualScreenReader::SetColor(Color c)
{
	AreaUp.Color = c;
	AreaDown.Color = c;
}
inline void
MDualScreenReader::SetFontSize(Drawing::Font::SizeType fz)
{
	fc.SetFontSize(fz);
	lnHeight = fc.GetHeight();
}
inline void
MDualScreenReader::SetLineGap(u8 g)
{
	AreaUp.LineGap = g;
	AreaDown.LineGap = g;
}

YSL_END_NAMESPACE(Components)

YSL_END_NAMESPACE(DS)

YSL_END

#endif

