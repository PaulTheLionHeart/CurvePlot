/*------------------------------------------------
   KEYBOARD.CPP -- Handle Keyboard Messages
  ------------------------------------------------*/

#include <windows.h>
#include "CurvePlot.h"
#include "resource.h"
//#include "menu.h"

extern	BOOL	SaveGIFOpenDlg (HWND, LPSTR, LPSTR);
extern	void	SaveFile(HWND, LPSTR, LPSTR);

/**************************************************************************
	Analyse Keyboard activity
**************************************************************************/

void	ProcessKeys(HWND hwnd, UINT wParam)

    {
    char	szTitleName[] = "Curve Plot";

    switch (wParam) 
	{
/*
	case VK_F1:						// Help    
	    PostMessage (hwnd, WM_COMMAND, IDM_HELP,   0L);
	    break;
*/    	
	case VK_UP:						// Scrolling messages    
	    PostMessage (hwnd, WM_VSCROLL, SB_LINEUP,   0L);
	    break;
    	
	case VK_DOWN:  
	    PostMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
	    break;
    	
	case VK_PRIOR: 
	    PostMessage (hwnd, WM_VSCROLL, SB_PAGEUP,   0L);
	    break;
    	
	case VK_NEXT:  
	    PostMessage (hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
	    break;
    	
	case VK_HOME:  
	    PostMessage (hwnd, WM_HSCROLL, SB_PAGEUP,   0L);
	    break;
    	
	case VK_END:   
	    PostMessage (hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L);
	    break;
    	
	case VK_LEFT:  
	    PostMessage (hwnd, WM_HSCROLL, SB_LINEUP,   0L);
	    break;
    	
	case VK_RIGHT: 
	    PostMessage (hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
	    break;
	case 'C':  						
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_COPY, 0L);			// Edit Colours 
	    break;
	case 'Z':  
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_IMAGE_SIZE, 0L);	// image size
	    break;
	case 'T':						// Fractal Type  
	    SendMessage (hwnd, WM_COMMAND, IDC_FRACTYPE, 0L);
	    break;
	
	case VK_RETURN:							// Let's get out of here
	case VK_ESCAPE: 

	    if (GetKeyState(VK_SHIFT) >= 0)
		{
		    {
		    if (MessageBox (hwnd, "Do you want to Quit?", "ManpWin", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
			{
			SendMessage (hwnd, WM_DESTROY, 0, 0L);
			}
		    }
		}
	    else
		{
		    {
		    SendMessage (hwnd, WM_DESTROY, 0, 0L);
		    }
		}
	    break;
	}
    }