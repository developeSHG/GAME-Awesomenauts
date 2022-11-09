#pragma once

// Standard
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// ��Ƽ�̵�� ����� �ʿ��� ����� ���̺귯���� �߰�. 
//#include <Vfw.h>
//#pragma comment(lib, "Vfw32.lib")

// ���� �Ŵ����� ����ϱ� ���� �߰��� �ش���. 
#include <io.h>
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

// �ܼ�â ����
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

// User
#include "Define.h"
#include "Enum.h"
#include "Extern.h"
#include "Struct.h"
#include "Pattern.h"
#include "Function.h"

// Managers
#include "TimeMgr.h"
#include "DeviceMgr.h"
#include "TextureMgr.h"
#include "ObjectMgr.h"
#include "SceneMgr.h"
#include "FrameMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"
#include "ItemMgr.h"
#include "AstarMgr.h"