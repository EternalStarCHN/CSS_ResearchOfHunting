#pragma comment(lib, "vfw32")
#include <iostream>
#include <string>

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "opencv.hpp"
#include "debugrob.h"
#include "./serial/ComPort.h"
#include "resource.h"

using namespace std;
using namespace cv;
using namespace Debug;

// �Զ���MFC��Ϣ���� �������ݽ�����Ϣ @PostMessageFunc OnReceiveData()
#define WM_RECV_SERIAL_DATA WM_USER + 101

using Contors_t = vector<vector<Point>>;
using Contor_t = vector<Point>;

class CautocarDlg : virtual DebugLabComm::CComPort, public CDialog , public DebugRob
{
public:
  explicit CautocarDlg(CWnd* pParent = NULL);
  virtual BOOL OnInitDialog();
  afx_msg void OnClose();

  IplImage* m_Frame;
  CvvImage m_CvvImage;
  CString m_linedegree;
  /**
   * @func: DoDataExchange - DDX/DDV֧��
   *      : OnPaint - ��Ի��������С����ť
   *      : OnQueryDragIcon - ���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ
   * @Message function mapping
   *      DECLARE_MESSAGE_MAP()
   *      @see autocarDlg.cpp BEGIN_MESSAGE_MAP
   */
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()

  /* ������غ��� ********************************************/
  afx_msg void OnBnClickedBt_OpenSerial();
  afx_msg void OnBnClickedBt_CloseSerial();
  afx_msg void OnBnClickedBt_SendToSerial();
  void PrintlnToSerial(const string& message);
  char HexToChar(char bChar);
 // void PrintToSerial(const string& message);
  afx_msg LONG OnRecvSerialData(WPARAM wParam, LPARAM lParam);
  void SendData(const char arrays[], int lenth);

  /* OpenCV��غ��� *****************************************/
  afx_msg void OnBnClickedBt_OpenCamera();
  afx_msg void OnBnClickedBt_CloseCamera();
  afx_msg void OnBnClickedBt_AutoDrive();
  afx_msg void OnBnClickedBt_ImageIdentification();
  afx_msg void OnBnClickedBttakephoto();
  afx_msg void OnBnClickedBtjl();
  afx_msg void OnBnClickedBtqyfg();
  void ImageRecognition(Mat src);
  void CameraPatrol();
  /* ·����غ���********************************************/
  afx_msg void OnBnClickedBtStopautodrive();//ֹͣ�Զ���ʻ
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  CString m_locationgold;
  int Findrst(char a[]);//�ж��Ƿ����ñ�־����
  /* ����ģʽ��غ���********************************************/
  void Mode(PointMode_t pointMode, int8_t command);//����Э��
protected:
  HICON appIcon_;
private:
  void _SerialOpen(int commNum = 2, int baudRate = 115200);
  void _OnCommReceive(LPVOID pSender, void* pBuf, DWORD InBufferCount) override;
  void _OnCommBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat) override;
  //TAG:����ImageBoxӦ����һ��ö�����ͣ��������
  void _ShowImageOnImageBox(int ImageBox, Mat frame);
  void _StretchBlt(int ImageBox, CDC& cdcSrc,
    int x = 0, int y = 0, int w = 48, int h = 48);

  /* ͼ��ʶ���㷨 ********************************************/
  /**
   * @func: _Binaryzation - �Դ����Mat���ж�ֵ������
   *        _FindContour  - �Դ����Mat���д�����ȡ���������
   * @Message �㷨һ
   *    ����ͷ         -(��ȡͼƬ)->       OpenCV::Mat
   *    OpenCV::Mat   -(��ֵ������)->     �ڰ׵�Mat
   *    �ڰ׵�Mat      -(��ȡ����)->       ������
   *    ������         -(�������������)->  ���������
   *    ������         -(�����ܳ������)->  ����ֵ��ʶ������
   * 
   * @Message �㷨��
   *    OpenCV ģ��ƥ���㷨��Template matching��
   */
  void _Binaryzation(const Mat & inputMat, Mat & outputMat);
  Mat _Binaryzation(const Mat & inputMat);
  void _FindContour(Mat & binaryMat, Contor_t &maximumInterContor);
  Contor_t _FindContour(Mat & binaryMat);
  const Contor_t & _FindContour();
  int _TemplateMatching(Mat & srcMat);
  int _HashMatching(Mat & srcMat);
  void _OldalgorithmMatching();
  void _LKMatching();
  void _MJMatching();
  /* ˽�������� *********************************************/
  CString _msgSerialSend;
  CString _msgSerialReceive;
  CString _datashow;
  CString reset;
  CvvImage _cvvImage;
  Mat ontimer_frame;
  double g_dConLength;
  double g_dConArea;

  /* ͼ��ʶ���� *********************************************/
  const vector<pair<Mat, int>> _TARGET_IMAGE_LIST;
  const vector<pair<Mat, int>> _TARGET_IMAGE_LIST1;

  VideoCapture _cameraForPic;
  VideoCapture _cameraForPath;

  Mat _binaryMat;
  Contors_t _contours_all;
  Contor_t _maximumInterContor;
  //TAG: ����ֵӦ��Ψһ��������doubleֵ���һ��struct���
  double _conLength;
  double _conArea;
  //TAG: _mode������Ӧ������Ϊһ�� ö����
  int _mode;
};
