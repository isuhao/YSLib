﻿/*
	Copyright (C) by Franksoft 2010.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file yform.cpp
\ingroup Shell
\brief 平台无关的 GUI 窗体实现。
\version 0.1479;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2010-04-30 00:51:36 + 08:00;
\par 修改时间:
	2010-12-21 16:12 + 08:00;
\par 字符集:
	UTF-8;
\par 模块名称:
	YSLib::Shell::YForm;
*/


#include "yform.h"
#include "ydesktop.h" // for YDesktop delete procedure;

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Components)

YSL_BEGIN_NAMESPACE(Forms)

using namespace Controls;

YForm::YForm(const Rect& r, const GHStrong<YImage> i, HWND hWnd)
	: YFrameWindow(r, i, hWnd)//,
//	Client(this, Rect::Empty/*r*/)
{
//	SetSize(Size.Width, Size.Height);
//	DrawBackground();
}
YForm::~YForm() ythrow()
{}
/*
void
YForm::SetSize(SDST w, SDST h)
{
	YFrameWindow::SetSize(w, h);
//	BitmapBuffer::SetSize(w, h);
//	YWidget::SetSize(w, h);
}
void
YForm::SetBounds(const Rect& r)
{
	YFrameWindow::SetBounds(r);
//	Location = r.GetPoint();
//	SetSize(r.Width, r.Height);
}

void
YForm::DrawBackground()
{
	YFrameWindow::DrawBackground();

//	Client.DrawBackground();
}

void
YForm::DrawForeground()
{
	YFrameWindow::DrawForeground();
//	Client.DrawForeground();

}
void
YForm::Draw()
{
	YFrameWindow::Draw();
}
*/
YSL_END_NAMESPACE(Forms)

YSL_END_NAMESPACE(Components)

YSL_END

