/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0        (GUI_ID_USER + 0x00)
#define ID_BUTTON_0        (GUI_ID_USER + 0x02)
#define ID_BUTTON_1        (GUI_ID_USER + 0x04)
#define ID_LISTWHEEL_0        (GUI_ID_USER + 0x05)
#define ID_LISTWHEEL_1        (GUI_ID_USER + 0x06)
#define ID_LISTWHEEL_2        (GUI_ID_USER + 0x07)
#define ID_TEXT_0        (GUI_ID_USER + 0x09)
#define ID_TEXT_1        (GUI_ID_USER + 0x0A)
#define ID_TEXT_2        (GUI_ID_USER + 0x0B)
#define ID_TEXT_3        (GUI_ID_USER + 0x0C)
#define ID_TEXT_4        (GUI_ID_USER + 0x0D)
#define ID_TEXT_5        (GUI_ID_USER + 0x0E)
#define ID_BUTTON_2        (GUI_ID_USER + 0x11)
#define ID_BUTTON_3        (GUI_ID_USER + 0x12)
#define ID_BUTTON_4        (GUI_ID_USER + 0x13)
#define ID_BUTTON_5        (GUI_ID_USER + 0x14)
#define ID_BUTTON_6        (GUI_ID_USER + 0x15)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Set Alarm Clocks", ID_FRAMEWIN_0, 41, 20, 400, 205, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Add Alarm Clock", ID_BUTTON_0, 34, 151, 120, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_1, 311, 151, 50, 30, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Week", ID_LISTWHEEL_0, 10, 20, 60, 120, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Hour", ID_LISTWHEEL_1, 74, 20, 55, 120, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Minute", ID_LISTWHEEL_2, 133, 20, 55, 120, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Select", ID_TEXT_0, 67, 0, 60, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Added", ID_TEXT_1, 262, 0, 60, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Wed-23-23", ID_TEXT_2, 200, 25, 80, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Mon-23-23", ID_TEXT_3, 296, 25, 80, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Sat-23-23", ID_TEXT_4, 200, 85, 80, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Sun-23-23", ID_TEXT_5, 296, 85, 80, 25, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "AC_1", ID_BUTTON_2, 210, 50, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AC_2", ID_BUTTON_3, 306, 50, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AC_3", ID_BUTTON_4, 210, 110, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AC_4", ID_BUTTON_5, 306, 110, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_6, 215, 152, 50, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Set Alarm Clocks'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 14);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_13B_ASCII);
    FRAMEWIN_SetTextColor(hItem, GUI_MAKE_COLOR(0x00000000));
    //
    // Initialization of 'Add Alarm Clock'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_13B_ASCII);
    //
    // Initialization of 'OK'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_13B_ASCII);
    //
    // Initialization of 'Week'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
    LISTWHEEL_AddString(hItem, "String");
    //
    // Initialization of 'Hour'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
    LISTWHEEL_AddString(hItem, "String");
    //
    // Initialization of 'Minute'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
    LISTWHEEL_AddString(hItem, "String");
    //
    // Initialization of 'Select'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x004080FF));
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    //
    // Initialization of 'Added'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x004080FF));
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    //
    // Initialization of 'Wed-23-23'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Mon-23-23'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Sat-23-23'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    //
    // Initialization of 'Sun-23-23'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    //
    // Initialization of 'Cancel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
    BUTTON_SetFont(hItem, GUI_FONT_13B_ASCII);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Add Alarm Clock'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'OK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTWHEEL_0: // Notifications sent by 'Week'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTWHEEL_1: // Notifications sent by 'Hour'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTWHEEL_2: // Notifications sent by 'Minute'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'AC_1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'AC_2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'AC_3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_5: // Notifications sent by 'AC_4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_6: // Notifications sent by 'Cancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateSet Alarm Clocks
*/
WM_HWIN CreateSet Alarm Clocks(void);
WM_HWIN CreateSet Alarm Clocks(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
