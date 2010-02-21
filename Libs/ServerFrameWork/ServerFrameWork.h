#pragma once



#include "../Utils/Utils.h"



#include "../NetLib/NetLib.h"
#include "../DBLib/DBLib.h"


#include "../Protocol/BaseProtocol.h"
#include "../Protocol/ServerConsoleProtocol.h"




#define SERVER_LOG_CHANNEL			8001

#define SERVER_STATUS_LOG_CHANNEL	8003

#define SYSTEM_NET_LINK_CONFIG_FILE_NAME	"SysLink.XML"


#define MAX_CONTROL_PANEL_MSG_LEN	20480						//控制台可显示的最多消息

#define SERVER_STATUS_BLOCK_SIZE	10240



#include "IBaseServer.h"


#include "SystemConfig.h"

#include "ControlPanel.h"


#include "FileLogWorkThread.h"
#include "ServerLogPrinter.h"

#include "SystemNetLink.h"
#include "SystemNetLinkManager.h"
#include "SystemControlPort.h"




#include "ServerThread.h"

#include "MainGuardThread.h"

#include "Tools.h"

#ifdef WIN32

#include "ServerFrameWorkResource.h"
#include "ServerConsoleDlg.h"
#include "NTService.h"
#include "ServerAppWin.h"

//#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Version.lib")

#else

	#include "ServerAppLinux.h"

#endif
