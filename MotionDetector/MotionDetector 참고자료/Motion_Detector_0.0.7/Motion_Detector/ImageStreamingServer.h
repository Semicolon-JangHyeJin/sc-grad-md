#pragma once
#include "ImageManager.h"

#define TIMER_FRAME				1000/15		// 15FRMAE�� ms�� ȯ��
#define BUF_SIZE				1024		// �⺻ ����ũ��
#define IMGBUF_SIZE				1024*1024	// �̹����� �����ϱ� ���� �޸𸮿� ������ �� ����ϴ� ���� ũ��
#define DEFAULT_MODE			TRUE		// �׳� �̹����� ������ ��
#define REPLACE_MODE			FALSE		// �������� ������ �� 
#define MAX_NUM_CAMVIEW_THREAD	20			// �������� ���� thread�� �ִ� ����

using namespace std;
using namespace cv;

class ImageStreamingServer:public ImageManager	// ImageManager ���
{
public:
	ImageStreamingServer();
	virtual ~ImageStreamingServer();

private:
	ImageManager* pIM;					// ImageManager ��ü
	CWinThread *mServerThread;			// ServerThread
	CWinThread *mClientThread;			// ClientThread
	CWinThread *mCacheThread;			// CacheThread
	vector<CWinThread*> mClientThreads;	// �������� �����ִ� client thread ���� ��Ƴ��� vector
	vector<SOCKET> mClientSockets;		// �������� �����ִ� socket ���� ��Ƴ��� vector
	SOCKET mServerThreadSock;			// ServerThread�� ����
	SOCKET mClientThreadSock;			// ClientThread�� ����
	SOCKADDR_IN mClientAddr;			// Client�� �ּ�

	UINT mPort;							// ��Ʈ��ȣ
	char mHostName[100];				// ȣ��Ʈ�̸�
	char buf[BUF_SIZE];					// �����͸� ���� ����
	TCHAR *mHostIp;						// ȣ��Ʈ ip

private:
	static UINT Server_Thread(LPVOID arg);					// ServerThread
	static UINT Client_Thread(LPVOID arg);					// ClientThread
	static UINT Cache_Thread(LPVOID arg);					// CacheThread
	void sendImageHeader(SOCKET client_sock, BOOL mode);	// ��Ŷ ����� ����
	void sendImage(SOCKET client_sock, IplImage *sendImage, BOOL mode);	// �̹����� ���̳ʸ� ���·� ����
	void sendResizedImage(SOCKET client_sock, char *fileName, int width, int height);	// �̹����� ���������ؼ� ����
	void sendMjpeg(SOCKET client_sock, IplImage *sendImage);	// ���������� ����
	void sendList(SOCKET client_sock);						// ����� �������� ����Ʈ ����
	CString getImagePath();									// ����� ������ path �˾Ƴ�

public:
	BOOL Start(UINT port);			// �ش� port�� ����
	BOOL Start();					// Default ����(8080 port)
	void Stop();					// ����
};