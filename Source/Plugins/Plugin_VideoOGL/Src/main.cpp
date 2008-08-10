// Copyright (C) 2003-2008 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#include "Globals.h"

#ifdef _WIN32
#include "OS\Win32.h"
#endif

#include "GUI/ConfigDlg.h"

#include "Render.h"
#include "GLInit.h"
#include "Fifo.h"
#include "OpcodeDecoding.h"
#include "TextureMngr.h"
#include "BPStructs.h"
#include "VertexLoader.h"
#include "PixelShaderManager.h"
#include "VertexShaderManager.h"

SVideoInitialize g_VideoInitialize;
#define VERSION_STRING "0.1"


void GetDllInfo (PLUGIN_INFO* _PluginInfo) 
{
    _PluginInfo->Version = 0x0100;
    _PluginInfo->Type = PLUGIN_TYPE_VIDEO;
#ifdef DEBUGFAST 
    sprintf(_PluginInfo->Name, "Dolphin OGL (DebugFast) " VERSION_STRING);
#else
#ifndef _DEBUG
    sprintf(_PluginInfo->Name, "Dolphin OGL " VERSION_STRING);
#else
    sprintf(_PluginInfo->Name, "Dolphin OGL (Debug) " VERSION_STRING);
#endif
#endif
}


void DllAbout(HWND _hParent) 
{
	wxAboutDialogInfo info;
	info.AddDeveloper(_T("zerofrog(@gmail.com)"));
	info.SetDescription(_T("Vertex/Pixel Shader 2.0 or higher, framebuffer objects, multiple render targets"));
	wxAboutBox(info);
}

void DllConfig(HWND _hParent)
{
#ifdef _WIN32
	wxWindow win;
	win.SetHWND(_hParent);
	ConfigDialog frame(&win);

	DWORD iModeNum = 0;
	DEVMODE dmi;
	unsigned int x = 0, y = 0;
	ZeroMemory(&dmi, sizeof(dmi));
	dmi.dmSize = sizeof(dmi);
	
	while(EnumDisplaySettings(NULL, iModeNum++, &dmi) != 0)
	{	
		if((x != dmi.dmPelsWidth && y != dmi.dmPelsHeight) && (dmi.dmPelsWidth > x))
		{
			char szBuffer[100];
			sprintf(szBuffer,"%dx%d", dmi.dmPelsWidth, dmi.dmPelsHeight);
			frame.AddFSReso(szBuffer);
			frame.AddWindowReso(szBuffer);
			x = dmi.dmPelsWidth;
			y = dmi.dmPelsHeight;
		}
        ZeroMemory(&dmi, sizeof(dmi));
	}
	frame.ShowModal();
	win.SetHWND(0);

#else
	ConfigDialog frame(NULL);
	g_Config.Load();
    XVisualInfo *vi;
    Colormap cmap;
    int dpyWidth, dpyHeight;
    int glxMajorVersion, glxMinorVersion;
    int vidModeMajorVersion, vidModeMinorVersion;
    GLWin.dpy = XOpenDisplay(0);
    glXQueryVersion(GLWin.dpy, &glxMajorVersion, &glxMinorVersion);
    XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion, &vidModeMinorVersion);
	//Get all full screen resos for the config dialog
	 XF86VidModeModeInfo **modes = NULL;
        int modeNum = 0;
        int bestMode = 0;

        //set best mode to current
        bestMode = 0;
        XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);
        int px = 0, py = 0;
        if (modeNum > 0 && modes != NULL) 
        {
            for (int i = 0; i < modeNum; i++) 
            {
                if(px != modes[i]->hdisplay && py != modes[i]->vdisplay)
                {
                    char temp[32];
                    sprintf(temp,"%dx%d", modes[i]->hdisplay, modes[i]->vdisplay);
                    frame.AddFSReso(temp);
                    frame.AddWindowReso(temp);//Add same to Window ones, since they should be nearly all that's needed
                    px = modes[i]->hdisplay;//Used to remove repeating from different screen depths
                    py = modes[i]->vdisplay;
                }
            }
        }    
	frame.ShowModal();
#endif
}


void Video_Initialize(SVideoInitialize* _pVideoInitialize)
{
    if (_pVideoInitialize == NULL)
        return;

#ifdef _WIN32
//    OpenConsole();
#endif
   
    frameCount = 0;
    g_VideoInitialize = *_pVideoInitialize;
    InitLUTs();
    g_Config.Load();
    
    if (!OpenGL_Create(g_VideoInitialize, 640, 480)) { //640x480 will be the default if all else fails//
        g_VideoInitialize.pLog("Renderer::Create failed\n", TRUE);
        return;
    }
    _pVideoInitialize->pPeekMessages = g_VideoInitialize.pPeekMessages;
    _pVideoInitialize->pUpdateFPSDisplay = g_VideoInitialize.pUpdateFPSDisplay;
    _pVideoInitialize->pWindowHandle = g_VideoInitialize.pWindowHandle;

	Renderer::AddMessage("Dolphin OpenGL Video Plugin v" VERSION_STRING ,5000);

}


void Video_Prepare(void)
{
	OpenGL_MakeCurrent();
	if (!Renderer::Create2()) {
        g_VideoInitialize.pLog("Renderer::Create2 failed\n", TRUE);
        return;
	}

    TextureMngr::Init();

    BPInit();
    VertexManager::Init();
    OpcodeDecoder_Init();
    Fifo_Init();
    VertexShaderMngr::Init();
    PixelShaderMngr::Init();
    GL_REPORT_ERRORD();
}


void Video_Shutdown(void) 
{
    VertexShaderMngr::Shutdown();
    PixelShaderMngr::Shutdown();
    Fifo_Shutdown();
    VertexManager::Destroy();
    TextureMngr::Shutdown();
    OpcodeDecoder_Shutdown();
    Renderer::Shutdown();
	OpenGL_Shutdown();
}


void DebugLog(const char* _fmt, ...)
{
#ifdef _DEBUG
    
    char* Msg = (char*)alloca(strlen(_fmt)+512);
    va_list ap;

    va_start( ap, _fmt );
    vsnprintf( Msg, strlen(_fmt)+512, _fmt, ap );
    va_end( ap );

    g_VideoInitialize.pLog(Msg, FALSE);
#endif
}


bool ScreenShot(TCHAR *File) 
{
    char str[64];
    int left = 200, top = 15;
    sprintf(str, "Dolphin OGL " VERSION_STRING);

    Renderer::ResetGLState();
    Renderer::RenderText(str, left+1, top+1, 0xff000000);
    Renderer::RenderText(str, left, top, 0xffc0ffff);
    Renderer::RestoreGLState();

    if (Renderer::SaveRenderTarget(File, 0)) {
        char str[255];
        sprintf(str, "saved %s\n", File);
        Renderer::AddMessage(str, 500);
    	return true;
    }
	return false;
}


BOOL Video_Screenshot(TCHAR* _szFilename)
{
    if (ScreenShot(_szFilename))
        return TRUE;

    return FALSE;
}


void Video_UpdateXFB(BYTE* _pXFB, DWORD _dwWidth, DWORD _dwHeight)
{
}
