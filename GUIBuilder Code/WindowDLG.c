
#include "DIALOG.h"

#define ID_WINDOW_0        (GUI_ID_USER + 0x00)

//  _aDialogCreate

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
};


//  _cbDialog
static void _cbDialog(WM_MESSAGE * pMsg) {

  switch (pMsg->MsgId) {

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

//  CreateWindow

WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

