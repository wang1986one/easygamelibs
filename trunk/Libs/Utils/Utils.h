/****************************************************************************/
/*                                                                          */
/*      文件名:    Utils.h                                                  */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/

#pragma once



#define UTILS_VERSION	"2.0.0.0"

#ifdef WIN32

#include "Win32AddOn.h"

#else

#include "LinuxAddOn.h"

#endif

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

const float PI				= float(3.14159265358979323846);
const float TWO_PI			= float(2.0 * 3.14159265358979323846);
const float HALF_PI			= float(3.14159265358979323846/ 2.0);


union ULONG64_CONVERTER
{
	ULONG64 QuadPart;
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	};
	struct 
	{
		WORD Words[4];
	};
	struct  
	{
		BYTE Bytes[8];
	};
};


#include "ExceptionHandler.h"


enum EASY_DATA_STORAGE_MODE
{
	EDSM_STATIC,
	EDSM_NEW_ONCE,
	EDSM_NEW_EVERY_TIME,
};


#include "MemoryAllocatee.h"
#include "MemoryLeakDetective.h"

#include "SmartPtr.h"

#include "EasyString.h"
#include "StringSplitter.h"


#include "IndexSet.h"
#include "EasyArray.h"
#include "EasyMap.h"


#include "SmartValue.h"
#include "SmartStruct.h"

#include "StaticObject.h"
#include "NameObject.h"

#include "AutoLock.h"
#include "AutoReadLock.h"
#include "AutoWriteLock.h"

#include "ILogPrinter.h"
#include "LogManager.h"

#include "ToolsAll.h"

#ifdef WIN32
#include "ToolsWin32.h"
#else
#include "ToolsLinux.h"
#endif

#include "lzma/LzmaLib.h"
#include "lzo/minilzo.h"


#include "EasyTime.h"
#include "EasyTimer.h"




#include "VSOutputLogPrinter.h"
#include "FileLogPrinter.h"

#ifdef WIN32
#include "EasyTimerEx.h"
#include "ExceptionParser.h"
#include "PerformanceStatistician.h"
#include "AutoPerformanceCounter.h"
#include "ThreadPerformanceCounter.h"
#endif






#include "IFileAccessor.h"
#include "IFileObjectCreator.h"
#include "FileSystemManager.h"

#include "StandardFileAccessor.h"
#include "StandardFileObjectCreator.h"

#ifdef WIN32
#include "WinFileAccessor.h"
#include "WinFileAccessorObjectCreator.h"
#include "ResourceFileAccessor.h"
#include "ResourceFileObjectCreator.h"
#else
#include "LinuxFileAccessor.h"
#include "LinuxFileAccessorObjectCreator.h"
#endif

#include "FilePathManager.h"


#include "SettingFile.h"
#include "StringFile.h"
#include "CSVReader.h"

#ifdef WIN32
#include "FileSearcherWin.h"
#else
#include "FileSearcherLinux.h"
#endif

#include "USOObjectCreateFilter.h"
#include "USOResourceManager.h"


#include "EasyList.h"
#include "IDStorage.h"
#include "StaticMap.h"
#include "TreeObject.h"


#include "FloatRect.h"

#ifdef WIN32
#include "WinTypeWrap.h"
#endif

#ifdef WIN32
#include "EasyThreadWin.h"
#else
#include "EasyThreadLinux.h"
#endif
#include "GuardThread.h"

#include "EasyBuffer.h"
#include "GrowBuffer.h"
#include "CycleBuffer.h"
#include "CycleBufferEx.h"
#include "ThreadSafeList.h"
#include "ThreadSafeIDStorage.h"
#include "ThreadSafeCycleBuffer.h"
#include "ThreadSafeCycleBufferEx.h"
#include "ThreadSafeStaticMap.h"
#include "FastMemoryPool.h"

#include "EasyScriptTypes.h"
#include "ESVariableList.h"
#include "ESFunctionList.h"
#include "ESBolanStack.h"
#include "ESThread.h"
#include "ESFunctionLib.h"
#include "EasyScriptExecutor.h"

#include "LZWPack.h"
#ifdef WIN32
#include "Gif.h"
#endif

#include "AsyncFileLogWorkThread.h"
#include "AsyncFileLogPrinter.h"

#include "HashMD5.h"

#ifdef WIN32
#include "SystemProcessList.h"
#include "WinServiceController.h"
#endif



#ifdef WIN32
#pragma comment(lib,"Dbghelp.lib")
#endif
