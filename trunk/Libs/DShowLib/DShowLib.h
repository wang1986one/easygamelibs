#pragma once

#define DSHOWLIB_VERSION	"1.0.0.0"



#include "dshow.h"
#include "vmr9.h"

#include ".\FilterBase\Streams.h"
#include "IDirectVobSub.h"

#include "D3DVideoTexture.h"

#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"quartz.lib")
//#pragma comment(lib,"wmvcore.lib")

using namespace DShowLib;