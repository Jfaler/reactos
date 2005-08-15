#ifndef _WIN32K_WINDOW_H
#define _WIN32K_WINDOW_H

struct _PROPERTY;
struct _WINDOW_OBJECT;
typedef struct _WINDOW_OBJECT *PWINDOW_OBJECT;

#include <include/object.h>
#include <include/class.h>
#include <include/msgqueue.h>
#include <include/winsta.h>
#include <include/dce.h>
#include <include/prop.h>
#include <include/scroll.h>


VOID FASTCALL
WinPosSetupInternalPos(VOID);

typedef struct _INTERNALPOS
{
  RECT NormalRect;
  POINT IconPos;
  POINT MaxPos;
} INTERNALPOS, *PINTERNALPOS;

typedef struct _WINDOW_OBJECT
{
  /* Pointer to the window class. */
  PWNDCLASS_OBJECT Class;
  /* entry in the window list of the class object */
  LIST_ENTRY ClassListEntry;
  /* Extended style. */
  DWORD ExStyle;
  /* Window name. */
  UNICODE_STRING WindowName;
  /* Style. */
  DWORD Style;
  /* Context help id */
  DWORD ContextHelpId;
  /* system menu handle. */
  HMENU SystemMenu;
  /* Handle of the module that created the window. */
  HINSTANCE Instance;
  /* Entry in the thread's list of windows. */
  LIST_ENTRY ListEntry;
  /* Pointer to the extra data associated with the window. */
  PCHAR ExtraData;
  /* Size of the extra data associated with the window. */
  ULONG ExtraDataSize;
  /* Position of the window. */
  RECT WindowRect;
  /* Position of the window's client area. */
  RECT ClientRect;
  /* Handle for the window. */
  HWND hSelf;
  /* Window flags. */
  ULONG Flags;
  /* Window menu handle or window id */
  UINT IDMenu;
  /* Handle of region of the window to be updated. */
  HANDLE UpdateRegion;
  HANDLE NCUpdateRegion;
  /* Handle of the window region. */
  HANDLE WindowRegion;
  /* Lock to be held when manipulating (NC)UpdateRegion */
 // FAST_MUTEX UpdateLock;
  struct _WINDOW_OBJECT* FirstChild;
  struct _WINDOW_OBJECT* LastChild;
  struct _WINDOW_OBJECT* NextSibling;
  struct _WINDOW_OBJECT* PrevSibling;
  /* Entry in the list of thread windows. */
  LIST_ENTRY ThreadListEntry;
  /* Handle to the parent window. */
  struct _WINDOW_OBJECT* ParentWnd;
  /* Handle to the owner window. */
  HWND hOwner;
  /* DC Entries (DCE) */
  PDCE Dce;
  /* Property list head.*/
  LIST_ENTRY PropListHead;
  ULONG PropListItems;
  /* Scrollbar info */
  PWINDOW_SCROLLINFO Scroll;
  LONG UserData;
  BOOL Unicode;
  WNDPROC WndProcA;
  WNDPROC WndProcW;
  /* owner queue/thread */
  union {
  PUSER_MESSAGE_QUEUE Queue;
  PW32THREAD WThread;
   };
  HWND hWndLastPopup; /* handle to last active popup window (wine doesn't use pointer, for unk. reason)*/
  PINTERNALPOS InternalPos;
  ULONG Status;
  /* counter for tiled child windows */
  ULONG TiledCounter;
  /* WNDOBJ list */
  LIST_ENTRY WndObjListHead;
} WINDOW_OBJECT; /* PWINDOW_OBJECT already declared at top of file */

/* Window flags. */
#define WINDOWOBJECT_NEED_SIZE            (0x00000001)
#define WINDOWOBJECT_NEED_ERASEBKGND      (0x00000002)
#define WINDOWOBJECT_NEED_NCPAINT         (0x00000004)
#define WINDOWOBJECT_NEED_INTERNALPAINT   (0x00000008)
#define WINDOWOBJECT_RESTOREMAX           (0x00000020)

#define WINDOWSTATUS_DESTROYING         (0x1)
#define WINDOWSTATUS_DESTROYED          (0x2)

#define HAS_DLGFRAME(Style, ExStyle) \
            (((ExStyle) & WS_EX_DLGMODALFRAME) || \
            (((Style) & WS_DLGFRAME) && (!((Style) & WS_THICKFRAME))))

#define HAS_THICKFRAME(Style, ExStyle) \
            (((Style) & WS_THICKFRAME) && \
            (!(((Style) & (WS_DLGFRAME | WS_BORDER)) == WS_DLGFRAME)))

#define HAS_THINFRAME(Style, ExStyle) \
            (((Style) & WS_BORDER) || (!((Style) & (WS_CHILD | WS_POPUP))))

#define IntIsDesktopWindow(WndObj) \
  (WndObj->ParentWnd == NULL)

#define IntIsBroadcastHwnd(hWnd) \
  (hWnd == HWND_BROADCAST || hWnd == HWND_TOPMOST)

#if 0
#define IntWndBelongsToThread(WndObj, W32Thread) \
  (((WndObj->W32Thread->Thread && WndObj->W32Thread->Thread->Tcb.Win32Thread)) && \
   (WndObj->W32Thread->Thread->Tcb.Win32Thread == W32Thread))
#endif

#define IntWndBelongsToThread(WndObj, _W32Thread) (QUEUE_2_WTHREAD((WndObj)->Queue) == (_W32Thread))


#define IntGetWndThreadId(WndObj) \
  WndObj->WThread->Thread->Cid.UniqueThread

#define IntGetWndProcessId(WndObj) \
  WndObj->WThread->Thread->ThreadsProcess->UniqueProcessId

PWINDOW_OBJECT FASTCALL
IntGetProcessWindowObject(PW32THREAD Thread, HWND hWnd);

BOOL FASTCALL
IntIsWindow(HWND hWnd);

PWINDOW_OBJECT* FASTCALL
UserListChildWnd(PWINDOW_OBJECT Window);

HWND* FASTCALL 
IntWinListChildren(PWINDOW_OBJECT Window);

NTSTATUS FASTCALL
InitWindowImpl (VOID);

NTSTATUS FASTCALL
CleanupWindowImpl (VOID);

VOID FASTCALL
IntGetClientRect (PWINDOW_OBJECT WindowObject, PRECT Rect);

PWINDOW_OBJECT FASTCALL 
UserGetActiveWindow(VOID);

BOOL FASTCALL
UserIsWindowVisible (PWINDOW_OBJECT hWnd);

BOOL FASTCALL
UserIsChildWindow (PWINDOW_OBJECT Parent, PWINDOW_OBJECT Child);

VOID FASTCALL
IntUnlinkWindow(PWINDOW_OBJECT Wnd);

VOID FASTCALL
IntLinkWindow(PWINDOW_OBJECT Wnd, PWINDOW_OBJECT WndParent, PWINDOW_OBJECT WndPrevSibling);

PWINDOW_OBJECT FASTCALL
UserGetAncestor(PWINDOW_OBJECT Wnd, UINT Type);

PWINDOW_OBJECT FASTCALL
UserGetParent(PWINDOW_OBJECT Wnd);

PWINDOW_OBJECT FASTCALL
IntGetOwner(PWINDOW_OBJECT Wnd);

INT FASTCALL
IntGetWindowRgn(HWND hWnd, HRGN hRgn);

INT FASTCALL
IntGetWindowRgnBox(HWND hWnd, RECT *Rect);

PWINDOW_OBJECT FASTCALL
UserGetShellWindow();

BOOL FASTCALL
IntGetWindowInfo(PWINDOW_OBJECT WindowObject, PWINDOWINFO pwi);

VOID FASTCALL
IntGetWindowBorderMeasures(PWINDOW_OBJECT WindowObject, UINT *cx, UINT *cy);

BOOL FASTCALL
IntAnyPopup(VOID);

BOOL FASTCALL
IntIsWindowInDestroy(PWINDOW_OBJECT Window);

DWORD IntRemoveWndProcHandle(WNDPROC Handle);
DWORD IntRemoveProcessWndProcHandles(HANDLE ProcessID);
DWORD IntAddWndProcHandle(WNDPROC WindowProc, BOOL IsUnicode);

BOOL FASTCALL
IntShowOwnedPopups( HWND owner, BOOL fShow );

#endif /* _WIN32K_WINDOW_H */

/* EOF */
