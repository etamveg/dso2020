
#ifdef HTMARCH_EXPORTS
#define HTMARCH_API extern "C" __declspec(dllexport)
#else
#define HTMARCH_API extern "C"__declspec(dllimport)
#endif
#define WINAPI __stdcall

HTMARCH_API short WINAPI dsoOpenDevice(WORD DeviceIndex);
HTMARCH_API short WINAPI dsoSetVoltDIV(WORD DeviceIndex,int nCH,int nVoltDIV);
HTMARCH_API short WINAPI dsoSetTimeDIV(WORD DeviceIndex,int nTimeDIV);
HTMARCH_API short WINAPI dsoReadHardData(WORD DeviceIndex,short* pCH1Data, short* pCH2Data,ULONG nReadLen,short* pCalLevel,int nCH1VoltDIV,int nCH2VoltDIV,short nTrigSweep,short nTrigSrc,short nTrigLevel,short nSlope,int nTimeDIV,short nHTrigPos,ULONG nDisLen,ULONG* nTrigPoint,short nInsertMode);
HTMARCH_API WORD WINAPI dsoGetCalLevel(WORD DeviceIndex,short* level,short nLen);
HTMARCH_API short WINAPI dsoCalibrate(WORD nDeviceIndex,int nTimeDIV,int nCH1VoltDIV,int nCH2VoltDIV,short* pCalLevel);
HTMARCH_API WORD WINAPI dsoSetCalLevel(WORD DeviceIndex,short* level,short nLen);
