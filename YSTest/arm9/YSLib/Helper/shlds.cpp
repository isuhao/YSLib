﻿/*
	Copyright (C) by Franksoft 2010.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file shlds.cpp
\ingroup Helper
\ingroup DS
\brief Shell 类库 DS 版本。
\version 0.1510;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2010-03-13 14:17:14 + 08:00;
\par 修改时间:
	2010-12-21 16:15 + 08:00;
\par 字符集:
	UTF-8;
\par 模块名称:
	YSLib::Helper::Shell_DS;
*/


#include "shlds.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Shells)

using namespace Messaging;

int
ShlCLI::OnActivated(const Message&)
{
	return 0;
}

int
ShlCLI::OnDeactivated(const Message&)
{
	return 0;
}

int
ShlCLI::ExecuteCommand(const uchar_t* s)
{
	return 0;
}


int
ShlGUI::OnDeactivated(const Message&)
{
	ClearScreenWindows(*hDesktopUp);
	ClearScreenWindows(*hDesktopDown);
	return 0;
}

void
ShlGUI::SendDrawingMessage()
{
//	hDesktopUp->ClearDesktopObjects();
//	hDesktopDown->ClearDesktopObjects();
	DispatchWindows();
	SendMessage(NULL, SM_PAINT, 0xE0,
		new GHandleContext<GHHandle<YDesktop> >(hDesktopUp));
	SendMessage(NULL, SM_PAINT, 0xE0,
		new GHandleContext<GHHandle<YDesktop> >(hDesktopDown));
}

void
ShlGUI::UpdateToScreen()
{
	YAssert(hDesktopUp,
		"In function \"void\nDS::ShlGUI::UpdateToScreen()\":\n"
		"The desktop pointer is null.");
	YAssert(hDesktopDown,
		"In function \"void\nDS::ShlGUI::UpdateToScreen()\":\n"
		"The desktop pointer is null.");

	hDesktopUp->Refresh();
	hDesktopUp->Update();
	hDesktopDown->Refresh();
	hDesktopDown->Update();
}

YSL_END_NAMESPACE(Shells)


YSL_BEGIN_NAMESPACE(DS)

ShlDS::ShlDS()
	: ShlGUI()
{}

int
ShlDS::ShlProc(const Message& msg)
{
	switch(msg.GetMessageID())
	{
	case SM_INPUT:
		ResponseInput(msg);
		return 0;

	default:
		return Shells::DefShellProc(msg);
	}
}

int
ShlDS::OnDeactivated(const Message& m)
{
	ShlGUI::OnDeactivated(m);
	GHHandle<YGUIShell> hShl(FetchGUIShellHandle());

	if(hShl)
	{
		*hShl -= hWndUp;
		*hShl -= hWndDown;
	}
	YReset(hWndUp);
	YReset(hWndDown);
	return 0;
}


void
ResponseInput(const Message& msg)
{
	using namespace Messaging;

	InputContext* const pContext(CastMessage<SM_INPUT>(msg));

	if(!pContext)
		return;

	Runtime::KeysInfo* const p(pContext->Key);

	if(p)
	{
		using namespace Runtime::KeySpace;

		if(p->up & Touch)
			OnTouchUp(pContext->CursorLocation);
		else if(p->up)
			OnKeyUp(p->up);
		if(p->down & Touch)
			OnTouchDown(pContext->CursorLocation);
		else if(p->down)
			OnKeyDown(p->down);
		if(p->held & Touch)
			OnTouchHeld(pContext->CursorLocation);
		else if(p->held)
			OnKeyHeld(p->held);
	}
}


void
ShlClearBothScreen(GHHandle<YGUIShell> h)
{
	if(h)
	{
		h->ClearScreenWindows(*hDesktopUp);
		h->ClearScreenWindows(*hDesktopDown);
	}
}

YSL_END_NAMESPACE(DS)

YSL_END

