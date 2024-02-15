#include "DIALOG.h"

#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_1 (GUI_ID_USER + 0x10)
#define ID_LISTWHEEL_0 (GUI_ID_USER + 0x11)
#define ID_LISTWHEEL_1 (GUI_ID_USER + 0x12)
#define ID_LISTWHEEL_2 (GUI_ID_USER + 0x13)
#define ID_TEXT_0 (GUI_ID_USER + 0x15)
#define ID_TEXT_1 (GUI_ID_USER + 0x16)
#define ID_TEXT_2 (GUI_ID_USER + 0x17)
#define ID_TEXT_3 (GUI_ID_USER + 0x18)

// _aDialogCreate
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 0, 10, 400, 205, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Set", ID_BUTTON_0, 308, 155, 50, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_1, 32, 155, 50, 30, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_0, 24, 28, 100, 110, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_1, 150, 28, 94, 111, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_2, 275, 28, 91, 108, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "now time is 2019-09-29", ID_TEXT_0, 114, 160, 160, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Year", ID_TEXT_1, 50, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Month", ID_TEXT_2, 175, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Day", ID_TEXT_3, 300, 6, 50, 20, 0, 0x0, 0 },
};


//   _cbDialog
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
   case WM_INIT_DIALOG:
    
    // Initialization of ''
  
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTitleHeight(hItem, 2);
    
    // Initialization of 'Set'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_13B_ASCII);
    
    // Initialization of 'Cancel'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_13B_ASCII);
    
    // Initialization of 'Listwheel'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
    LISTWHEEL_AddString(hItem, "String");
    
    // Initialization of 'Listwheel'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
    LISTWHEEL_AddString(hItem, "String");
    
    // Initialization of 'Listwheel'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
    LISTWHEEL_AddString(hItem, "String");
    
    // Initialization of 'now time is 2019-09-29'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    // Initialization of 'Year'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    // Initialization of 'Month'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

    // Initialization of 'Day'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Set'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Cancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_LISTWHEEL_0: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
      }
      break;
    case ID_LISTWHEEL_1: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
      }
      break;
    case ID_LISTWHEEL_2: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

// Create

WM_HWIN Create(void);
WM_HWIN Create(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

