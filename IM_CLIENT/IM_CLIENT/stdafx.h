
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <gdiplus.h> 
using namespace Gdiplus; 

#pragma comment(lib, "gdiplus.lib")

#include ".\UI\Base\GlobalFunction.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#if   defined(_WIN64)   
typedef   __int64   INT_PTR, *PINT_PTR;
typedef   unsigned   __int64   UINT_PTR, *PUINT_PTR;

typedef   __int64   LONG_PTR, *PLONG_PTR;
typedef   unsigned   __int64   ULONG_PTR, *PULONG_PTR;

#define   __int3264       __int64   

#else   
typedef   _W64   int   INT_PTR, *PINT_PTR;
typedef   _W64   unsigned   int   UINT_PTR, *PUINT_PTR;

typedef   _W64   long   LONG_PTR, *PLONG_PTR;
typedef   _W64   unsigned   long   ULONG_PTR, *PULONG_PTR;

#define   __int3264       __int32   

#endif

#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4800)
#pragma warning(disable:4818)
#pragma warning(disable:4018)


#include "targetver.h"
#include "resource.h"
#include <afxcontrolbars.h>
