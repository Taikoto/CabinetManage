#ifndef  _PROTOCOL_Optics
#define  _PROTOCOL_Optics

#ifdef _OT_CPP_
#undef _OT_CPP_
#endif

#define PS_MAXWAITTIME 2000
#define DELAY_TIME     100
///////////////////错误返回码////////////////////
//#define PS_OK                0x00
//#define PS_COMM_ERR          0x01
//#define PS_NO_FINGER         0x02
//#define PS_GET_IMG_ERR       0x03
//#define PS_FP_TOO_DRY        0x04
//#define PS_FP_TOO_WET        0x05
//#define PS_FP_DISORDER       0x06
//#define PS_LITTLE_FEATURE    0x07
//#define PS_NOT_MATCH         0x08
//#define PS_NOT_SEARCHED      0x09
//#define PS_MERGE_ERR         0x0a
//#define PS_ADDRESS_OVER      0x0b
//#define PS_READ_ERR          0x0c
//#define PS_UP_TEMP_ERR       0x0d
//#define PS_RECV_ERR          0x0e
//#define PS_UP_IMG_ERR        0x0f
//#define PS_DEL_TEMP_ERR      0x10
//#define PS_CLEAR_TEMP_ERR    0x11
//#define PS_SLEEP_ERR         0x12
//#define PS_INVALID_PASSWORD  0x13
//#define PS_RESET_ERR         0x14
//#define PS_INVALID_IMAGE     0x15
//#define PS_HANGOVER_UNREMOVE 0X17


///////////////缓冲区//////////////////////////////
#define CHAR_BUFFER_A          0x01
#define CHAR_BUFFER_B          0x02
#define MODEL_BUFFER           0x03

//////////////////串口号////////////////////////
#define COM1                   0x01
#define COM2                   0x02
#define COM3                   0x03

//////////////////波特率////////////////////////
#define BAUD_RATE_9600         0x00
#define BAUD_RATE_19200        0x01
#define BAUD_RATE_38400        0x02
#define BAUD_RATE_57600        0x03   //default
#define BAUD_RATE_115200       0x04

#define MAX_PACKAGE_SIZE_		350   // 数据包最大长度
#define CHAR_LEN_AES1711		1024  // 512->1024 [2009.11.12] AES1711使用1024大小模版
#define CHAR_LEN_NORMAL			512	  // 512 通用版本使用512大小的模版


#define DEVICE_USB		0
#define DEVICE_COM		1
#define DEVICE_UDisk	2

#define IMAGE_X 256
#define IMAGE_Y 288

#ifdef _OT_CPP_
extern "C"
{
#endif

#include <Windows.h>
#include <QObject>

//==============================================================================//
//获取 FG_USB 设备数
int WINAPI	ZAZGetUSBDevNum(int* iNums);
int WINAPI	ZAZGetUDiskNum(int* iNums);
//获取 UDISK 设备数


//打开设备
int WINAPI ZAZOpenDeviceEx(HANDLE* pHandle, int nDeviceType,int iCom=1,int iBaud=1,int nPackageSize=2,int iDevNum=0);
int WINAPI ZAZCloseDeviceEx(HANDLE hHandle);
//关闭设备

//根据错误号获取错误信息
char* WINAPI	ZAZErr2Str(int nErrCode);

//检测手指并录取图像
int WINAPI	ZAZGetImage(HANDLE hHandle,int nAddr);
//根据原始图像生成指纹特征
int WINAPI	ZAZGenChar(HANDLE hHandle,int nAddr,int iBufferID);
//直接在窗体上显示指纹图像
int WINAPI	ZAZShowFingerData(HWND hWnd,uchar *pFingerData);
//将CharBufferA与CharBufferB中的特征文件合并生成模板存于ModelBuffer
int	WINAPI	ZAZRegModule(HANDLE hHandle,int nAddr);
//将ModelBuffer中的文件储存到flash指纹库中
int WINAPI	ZAZStoreChar(HANDLE hHandle,int nAddr,int iBufferID, int iPageID);
//精确比对CharBufferA与CharBufferB中的特征文件
int WINAPI	ZAZMatch(HANDLE hHandle,int nAddr,int* iScore);
//以CharBufferA或CharBufferB中的特征文件搜索整个或部分指纹库
int WINAPI	ZAZSearch(HANDLE hHandle,int nAddr,int iBufferID, int iStartPage, int iPageNum, int *iMbAddress,int *iscore=NULL);
//++高速搜索
int WINAPI	ZAZHighSpeedSearch(HANDLE hHandle,int nAddr,int iBufferID, int iStartPage, int iPageNum, int *iMbAddress,int *iscore=NULL);
//删除flash指纹库中的一个特征文件
int WINAPI	ZAZDelChar(HANDLE hHandle,int nAddr,int iStartPageID,int nDelPageNum);
//清空flash指纹库
int WINAPI	ZAZEmpty(HANDLE hHandle,int nAddr);



//读参数表
int WINAPI	ZAZReadParTable(HANDLE hHandle,int nAddr,uchar* pParTable);
//++读Flash
int WINAPI	ZAZReadInfPage(HANDLE hHandle,int nAddr, uchar* pInf);
//++读有效模板个数
int WINAPI	ZAZTemplateNum(HANDLE hHandle,int nAddr,int *iMbNum);
//++写模块寄存器
int WINAPI	ZAZWriteReg(HANDLE hHandle,int nAddr,int iRegAddr,int iRegValue);
//写模块寄存器－波特率设置
int WINAPI	ZAZSetBaud(HANDLE hHandle,int nAddr,int nBaudNum);
//写模块寄存器－安全等级设置
int WINAPI	ZAZSetSecurLevel(HANDLE hHandle,int nAddr,int nLevel);
//写模块寄存器－数据包大小设置
int WINAPI	ZAZSetPacketSize(HANDLE hHandle,int nAddr,int nSize);
//获取随机数
int WINAPI	ZAZGetRandomData(HANDLE hHandle,int nAddr,uchar* pRandom);
//设置芯片地址
int WINAPI	ZAZSetChipAddr(HANDLE hHandle,int nAddr,uchar* pChipAddr);
//读模版索引表	nPage,0,1,2,3对应模版从0-256，256-512，512-768，768-1024
int WINAPI	ZAZReadIndexTable(HANDLE hHandle,int nAddr,int nPage,uchar* UserContent);
//设置红绿灯
int WINAPI	ZAZDoUserDefine(HANDLE hHandle,int nAddr,int GPIO,int STATE);
//把原始数据存为图像格式
int WINAPI	ZAZFingerData2BMPData(uchar *pFingerData, uchar *pBMPData,int* nBMPDataLen);




//从flash指纹库中读取一个模板到ModelBuffer
int WINAPI	ZAZLoadChar(HANDLE hHandle,int nAddr,int iBufferID,int iPageID);
//将特征缓冲区中的文件上传给上位机
int WINAPI	ZAZUpChar(HANDLE hHandle,int nAddr,int iBufferID, uchar* pTemplet, int* iTempletLength);
//从上位机下载一个特征文件到特征缓冲区
int WINAPI	ZAZDownChar(HANDLE hHandle,int nAddr,int iBufferID, uchar* pTemplet, int iTempletLength);


//上传原始图像
int WINAPI	ZAZUpImage(HANDLE hHandle,int nAddr,uchar* pImageData, int* iImageLength);
int WINAPI	ZAZDownImage(HANDLE hHandle,int nAddr,uchar *pImageData, int iLength);
//下载原始图像

//上传原始图像
int WINAPI	ZAZImgData2BMP(uchar* pImgData,const char* pImageFile);
int WINAPI	ZAZGetImgDataFromBMP(HANDLE hHandle,const char *pImageFile,uchar *pImageData,int *pnImageLen);
//下载原始图像

//读记事本
int WINAPI	ZAZReadInfo(HANDLE hHandle,int nAddr,int nPage,uchar* UserContent);
int WINAPI	ZAZWriteInfo(HANDLE hHandle,int nAddr,int nPage,uchar* UserContent);
//写记事本

//设置设备握手口令
int WINAPI	ZAZSetPwd(HANDLE hHandle,int nAddr,uchar* pPassword);
int WINAPI	ZAZVfyPwd(HANDLE hHandle,int nAddr,uchar* pPassword);
//验证设备握手口令

//上传指纹为.BAT文件
int WINAPI	ZAZUpChar2File(HANDLE hHandle,int nAddr,int iBufferID, const char* pFileName);
int WINAPI	ZAZDownCharFromFile(HANDLE hHandle,int nAddr,int iBufferID, const char* pFileName);
//.BAT文件转指纹

//设置特征模版库的大小 通用-512 AES1711-1024
int WINAPI  ZAZSetCharLen(int nLen = CHAR_LEN_NORMAL);
int WINAPI	ZAZGetCharLen(int *pnLen);
//获取1枚特征的大小

//控制灯
int WINAPI ZAZSetledsound(HANDLE hHandle,int nAddr,uchar red, uchar green, uchar sound, uchar moveflag,int timecount);


#ifdef _OT_CPP_
}
#endif

#endif                                                     
