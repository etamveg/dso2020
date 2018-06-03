#include <iostream>
#include <windows.h>
#include <string>

#ifdef HTMARCH_EXPORTS
#define HTMARCH_API extern "C" __declspec(dllexport)
#else
#define HTMARCH_API extern "C" __declspec(dllimport)
#endif

#define WINAPI __stdcall

using namespace std;

#define DSO_DEVICE_ID 0

#define OSC_CHANNEL_1 0
#define OSC_CHANNEL_2 1

#define OSC_VOTAGE_RESOLUTION_20mVpDIV 0
#define OSC_VOTAGE_RESOLUTION_50mVpDIV 1
#define OSC_VOTAGE_RESOLUTION_100mVpDIV 2
#define OSC_VOTAGE_RESOLUTION_200mVpDIV 3
#define OSC_VOTAGE_RESOLUTION_500mVpDIV 4
#define OSC_VOTAGE_RESOLUTION_1VpDIV 5
#define OSC_VOTAGE_RESOLUTION_2VpDIV 6
#define OSC_VOTAGE_RESOLUTION_5VpDIV 7


#define OSC_SAMPLING_RATE_48MSa 0//0 ~ 10 : 48MSa/s
#define OSC_SAMPLING_RATE_16MSa 11//11: 16MSa/s
#define OSC_SAMPLING_RATE_8MSa 12//12: 8MSa/s
#define OSC_SAMPLING_RATE_4MSa 13//13: 4MSa/s
#define OSC_SAMPLING_RATE_1MSa 14//14 ~ 24: 1MSa/s
#define OSC_SAMPLING_RATE_500kSa 25//25: 500KSa/s
#define OSC_SAMPLING_RATE_200kSa 26//26: 200KSa/s
#define OSC_SAMPLING_RATE_100kSa 27//27: 100KSa/s

#define OSC_TRIGGER_SWEEP_MODE_Auto 0
#define OSC_TRIGGER_SWEEP_MODE_Normal 1
#define OSC_TRIGGER_SWEEP_MODE_Single 2

#define OSC_TRIGGER_SLOPE_MODE_Rise 0
#define OSC_TRIGGER_SLOPE_MODE_Fall 1

#define SAMPLING_BUFFER_SIZE_IN_SCOPE 100000
#define SAMPLING_BUFFER_SIZE_IN_RAM   10000000
HINSTANCE hinstDLL;

typedef short (WINAPI* sdkOpenDevice)(WORD DeviceIndex);

typedef short (WINAPI* sdkSetVoltDiv)(WORD DeviceIndex,int nCH,int nVoltDIV);
typedef short (WINAPI* sdkGetCalLevel)(WORD DeviceIndex,short* level,short nLen);
typedef short (WINAPI* sdkReadHardData)(WORD DeviceIndex,short* pCH1Data, short* pCH2Data,ULONG nReadLen,short* pCalLevel,int nCH1VoltDIV,int nCH2VoltDIV,short nTrigSweep,short nTrigSrc,short nTrigLevel,short nSlope,int nTimeDIV,short nHTrigPos,ULONG nDisLen,ULONG* nTrigPoint,short nInsertMode);
typedef short (WINAPI* sdkSetTimeDIV)(WORD DeviceIndex,int nTimeDIV);

//HTMARCH_API short WINAPI dsoSetVoltDIV(WORD DeviceIndex,int nCH,int nVoltDIV);
//HTMARCH_API short WINAPI dsoSetTimeDIV(WORD DeviceIndex,int nTimeDIV);
//HTMARCH_API WORD WINAPI dsoGetCalLevel(WORD DeviceIndex,short* level,short nLen);
//HTMARCH_API short WINAPI dsoReadHardData(WORD DeviceIndex,short* pCH1Data, short* pCH2Data,ULONG nReadLen,short* pCalLevel,int nCH1VoltDIV,int nCH2VoltDIV,short nTrigSweep,short nTrigSrc,short nTrigLevel,short nSlope,int nTimeDIV,short nHTrigPos,ULONG nDisLen,ULONG* nTrigPoint,short nInsertMode);
//HTMARCH_API short (WINAPI dsoSetVoltDIV)(WORD DeviceIndex,int nCH,int nVoltDIV);
//HTMARCH_API short (WINAPI dsoSetTimeDIV)(WORD DeviceIndex,int nTimeDIV);
//HTMARCH_API short WINAPI dsoReadHardData(WORD DeviceIndex,short* pCH1Data, short* pCH2Data,ULONG nReadLen,short* pCalLevel,int nCH1VoltDIV,int nCH2VoltDIV,short nTrigSweep,short nTrigSrc,short nTrigLevel,short nSlope,int nTimeDIV,short nHTrigPos,ULONG nDisLen,ULONG* nTrigPoint,short nInsertMode);
//HTMARCH_API WORD WINAPI dsoGetCalLevel(WORD DeviceIndex,short* level,short nLen);
//HTMARCH_API short WINAPI dsoCalibrate(WORD nDeviceIndex,int nTimeDIV,int nCH1VoltDIV,int nCH2VoltDIV,short* pCalLevel);
//HTMARCH_API WORD WINAPI dsoSetCalLevel(WORD DeviceIndex,short* level,short nLen);

short data_ch_1[SAMPLING_BUFFER_SIZE_IN_RAM];
short data_ch_2[SAMPLING_BUFFER_SIZE_IN_RAM];
ULONG trig_point;
short calData[32];
int main()
{

    sdkOpenDevice openDevice;
    sdkSetVoltDiv setVoltDiv;
    sdkGetCalLevel getCalLevel;
    sdkReadHardData readHardData;
    sdkSetTimeDIV setTimeDIV;


    cout << "Hello world!" << endl;
    hinstDLL = LoadLibrary("C:/Users/Lenovo/Documents/dso/dso/HTMarch.dll");

    if(hinstDLL == 0) {

        cout << "no dll   " << GetLastError() << endl;
    } else {
        openDevice = (sdkOpenDevice)GetProcAddress(hinstDLL, "dsoOpenDevice");
        setVoltDiv = (sdkSetVoltDiv)GetProcAddress(hinstDLL, "dsoSetVoltDIV");
        getCalLevel = (sdkGetCalLevel)GetProcAddress(hinstDLL, "dsoGetCalLevel");
        readHardData = (sdkReadHardData)GetProcAddress(hinstDLL, "dsoReadHardData");
        setTimeDIV = (sdkSetTimeDIV)GetProcAddress(hinstDLL, "dsoSetTimeDIV");

        if(openDevice != 0 &&
           setVoltDiv != 0 &&
           getCalLevel != 0 &&
           readHardData != 0 &&
           setTimeDIV != 0) {


            if(openDevice(DSO_DEVICE_ID)){
                if(!setVoltDiv(DSO_DEVICE_ID,OSC_CHANNEL_1,OSC_VOTAGE_RESOLUTION_200mVpDIV)){
                    cout << "Device 0 ch1 voltage setup failed" << endl;
                } else {
                    cout << "Device 0 ch1 voltage setup OK" << endl;
                }
                if(!setVoltDiv(DSO_DEVICE_ID,OSC_CHANNEL_2,OSC_VOTAGE_RESOLUTION_200mVpDIV)){
                    cout << "Device 0 ch2 voltage setup failed" << endl;
                } else {
                    cout << "Device 0 ch2 voltage setup OK" << endl;
                }

                if(!setTimeDIV(DSO_DEVICE_ID,OSC_SAMPLING_RATE_1MSa)){
                    cout << "Device 0 ch2 timediv setup failed" << endl;
                } else {
                    cout << "Device 0 ch2 timediv setup OK" << endl;
                }
                getCalLevel(DSO_DEVICE_ID,calData, 32);
                cout << "Sampling" << endl;
                for(int i=0; i< SAMPLING_BUFFER_SIZE_IN_RAM/SAMPLING_BUFFER_SIZE_IN_SCOPE; i++)
                {


                    printf("\r%d/%d",i,SAMPLING_BUFFER_SIZE_IN_RAM/SAMPLING_BUFFER_SIZE_IN_SCOPE);
                    readHardData(DSO_DEVICE_ID,
                                    data_ch_1+SAMPLING_BUFFER_SIZE_IN_SCOPE*i,
                                    data_ch_2+SAMPLING_BUFFER_SIZE_IN_SCOPE*i,
                                    SAMPLING_BUFFER_SIZE_IN_SCOPE,//ULONG nReadLen,
                                    calData,
                                    OSC_VOTAGE_RESOLUTION_200mVpDIV, //int nCH1VoltDIV,
                                    OSC_VOTAGE_RESOLUTION_50mVpDIV, //int nCH2VoltDIV,
                                    OSC_TRIGGER_SWEEP_MODE_Auto,
                                    OSC_CHANNEL_2,
                                    130, // 0-255
                                    OSC_TRIGGER_SLOPE_MODE_Rise,
                                    OSC_SAMPLING_RATE_500kSa, //int nTimeDIV,
                                    50, //short nHTrigPos [0...100],
                                    SAMPLING_BUFFER_SIZE_IN_SCOPE, //ULONG nDisLen,
                                    &trig_point, //ULONG* nTrigPoint,
                                    0); //short nInsertMode
                }

                FILE* file;
                cout << "Save file 1" << endl;
                file = fopen("log_from_dso_ch1.csv", "w+");
                for(int i=0; i<SAMPLING_BUFFER_SIZE_IN_RAM; i++){
                        if(i%(SAMPLING_BUFFER_SIZE_IN_RAM/1000) == 0)
                        {
                            printf("\r%f kesz",(float)i/SAMPLING_BUFFER_SIZE_IN_RAM);
                        }

                        if(i%20 == 0)
                        {
                            fprintf(file, "%d, %d\n",i, data_ch_1[i]);
                        }

                }
                fclose(file);

                cout << "Save file 2" << endl;
                file = fopen("log_from_dso_ch2.csv", "w+");
                for(int i=0; i<SAMPLING_BUFFER_SIZE_IN_RAM; i++){
                        if(i%(SAMPLING_BUFFER_SIZE_IN_RAM/1000) == 0)
                        {
                            printf("\r%f kesz",(float)i/SAMPLING_BUFFER_SIZE_IN_RAM);
                        }

                        if(i%20 == 0)
                        {
                            fprintf(file, "%d, %d\n",i, data_ch_2[i]);
                        }

                }
                fclose(file);

            } else {
                cout << "Device "<< DSO_DEVICE_ID <<" open failed" << endl;
            }

            cout << "Open device 1: " << openDevice(1) << endl;
        } else {
             cout << "no function" << endl;
        }
        FreeLibrary(hinstDLL);
    }
    return 0;
}
