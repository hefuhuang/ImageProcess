; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDIBProcessView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DIBProcess.h"
LastPage=0

ClassCount=11
Class1=CDIBProcessApp
Class2=CDIBProcessDoc
Class3=CDIBProcessView
Class4=CMainFrame

ResourceCount=6
Resource1=IDR_BMPTYPE
Resource2=IDD_DIALOG2
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDD_DIALOG1
Class7=CTPCL
Class8=channel
Class9=niose2
Resource4=IDR_MAINFRAME
Class10=niose
Resource5=IDD_ABOUTBOX
Class11=PINYI
Resource6=IDD_DIALOG3

[CLS:CDIBProcessApp]
Type=0
HeaderFile=DIBProcess.h
ImplementationFile=DIBProcess.cpp
Filter=N

[CLS:CDIBProcessDoc]
Type=0
HeaderFile=DIBProcessDoc.h
ImplementationFile=DIBProcessDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=ID_EDIT_RELOAD

[CLS:CDIBProcessView]
Type=0
HeaderFile=DIBProcessView.h
ImplementationFile=DIBProcessView.cpp
Filter=C
LastObject=ID_MENU_JHJZ
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=DIBProcess.cpp
ImplementationFile=DIBProcess.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308480

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_PRINT_SETUP
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_APP_ABOUT
CommandCount=7

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_EDIT_RELOAD
Command4=ID_APP_ABOUT
CommandCount=4

[MNU:IDR_BMPTYPE]
Type=1
Class=CDIBProcessView
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_RELOAD
Command8=ID_MEJHH
Command9=ID_MEQFS
Command10=ID_MEYuZhiHua
Command11=ID_zuixiaozhichuli
Command12=ID_zhongzhilvbo
Command13=ID_MENU_CSTJ
Command14=NXBJZLB
Command15=ID_MENUXBJZLB
Command16=ID_Geopro
Command17=ID_Mpengzhangyufushi
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_APP_ABOUT
CommandCount=24

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_EDIT_RELOAD
Command3=ID_FILE_SAVE
Command4=ID_NEXT_PANE
Command5=ID_PREV_PANE
CommandCount=5

[DLG:IDD_DIALOG1]
Type=1
Class=CTPCL
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CTPCL]
Type=0
HeaderFile=TPCL.h
ImplementationFile=TPCL.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[CLS:channel]
Type=0
HeaderFile=channel.h
ImplementationFile=channel.cpp
BaseClass=CDialog
Filter=D
LastObject=channel

[DLG:IDD_DIALOG2]
Type=1
Class=niose
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_HUJIAOpanbie,button,1342308361
Control4=IDC_BAIYANpanbie,button,1342308361
Control5=IDC_HUJIAO,edit,1350762624
Control6=IDC_BAIYAN,edit,1350762624

[CLS:niose2]
Type=0
HeaderFile=niose2.h
ImplementationFile=niose2.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_ABOUT

[CLS:niose]
Type=0
HeaderFile=niose1.h
ImplementationFile=niose1.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BAIYANpanbie

[DLG:IDD_DIALOG3]
Type=1
Class=PINYI
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_x,edit,1350697088
Control4=IDC_EDIT_Y,edit,1350697088
Control5=IDC_STATIC,static,1342177280
Control6=IDC_STATIC,static,1342177280
Control7=IDC_EDIT1,edit,1350697088
Control8=IDC_STATIC,static,1342177280

[CLS:PINYI]
Type=0
HeaderFile=PINYI.h
ImplementationFile=PINYI.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_ABOUT
VirtualFilter=dWC

