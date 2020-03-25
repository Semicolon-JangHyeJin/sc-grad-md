#include "Stdafx.h"
#include "ImageStreamingServer.h"

ImageStreamingServer::ImageStreamingServer():ImageManager()
{
	// ���� �ʱ�ȭ 
	mServerThread = mClientThread = mCacheThread = NULL; 
	mClientThreadSock = NULL;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		AfxMessageBox(L"���� �ʱ�ȭ ����!!");
	}

	//ȣ��Ʈ�̸��� IP�ּ� �˾ƿ���
	struct hostent *lphost;
	struct in_addr in;
	gethostname(mHostName,99);
	lphost = gethostbyname(mHostName);
	in.s_addr = ((struct in_addr *)lphost->h_addr)->s_addr;
	mHostIp = CA2W(inet_ntoa(in));
}

ImageStreamingServer::~ImageStreamingServer()
{
	// ������ �������¶�� Stop �޼ҵ� ����
	if( isRunning ) {
		Stop();
	}

	// ���� ��� ����
	WSACleanup();
}


BOOL ImageStreamingServer::Start(UINT port)
{
	// �̹��� �Ŵ��� ����
	if( !ImageManager::Start() ) {
		return FALSE;
	}

	mPort = port;

	if(mServerThread == NULL && mCacheThread == NULL)	{ 
		//Server Thread �� ����ǰ� ���� ���� ��
		mServerThread = AfxBeginThread(Server_Thread, (LPVOID)this);
		mCacheThread = AfxBeginThread(Cache_Thread, (LPVOID)this);
		return TRUE;
	}
	else {
		AfxMessageBox(L"Server Thread �� �̹� �������Դϴ�.");
		return FALSE;
	}
}

BOOL ImageStreamingServer::Start()
{
	// �μ� ���� �����ϸ� default �� 8080 port�� ����
	return Start(8080);
}

void ImageStreamingServer::Stop()
{
	if(mServerThread->m_hThread != NULL) {
		// Server Thread ����
		TerminateThread(mServerThread->m_hThread, NULL);
		mServerThread = NULL;
	}

	if(mCacheThread->m_hThread != NULL) {
		// Cache Thread ����
		TerminateThread(mCacheThread->m_hThread, NULL);
		mCacheThread = NULL;
	}

	for(unsigned int i=0;i<mClientSockets.size();i++) {
		// Client Socket ����
		closesocket(mClientSockets[i]);
		Sleep(10);
	}

	for(unsigned int i=0;i<mClientThreads.size();i++) {
		if(mClientThreads[i]->m_hThread != NULL) {
			// Client Thread ����
			TerminateThread(mClientThreads[i]->m_hThread, NULL);
			mClientThreads[i] = NULL;
		}
	}

	// ���� ���� �� ���� ����
	mServerThread = NULL;
	closesocket(mServerThreadSock);

	// vector�� clear
	mClientThreads.clear();
	mClientSockets.clear();

	// �̹��� �Ŵ��� ����
	ImageManager::Stop();
}

UINT ImageStreamingServer::Server_Thread(LPVOID arg)
{
	ImageStreamingServer *pISS = (ImageStreamingServer*)arg;

	// socket()
	int option = 1;
	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (LPSTR)&option, sizeof(option)); // ���� �ɼǼ���
	pISS->mServerThreadSock = server_sock;

	if(server_sock == INVALID_SOCKET)
	{
		AfxMessageBox(L"socket() ����");
		return 0;
	}

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(pISS->mPort);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(server_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) 	{
		AfxMessageBox(L"bind() ����");
		return 0;
	}
	
	// listen()
	if(listen(server_sock, SOMAXCONN) == SOCKET_ERROR) {
		AfxMessageBox(L"listen() ����");
		return 0;
	}

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;
	addrlen = sizeof(client_addr);

	while(1)
	{
		while(pISS->mClientThreadSock != NULL)	//�ڽĽ����忡�� ���ϰ��� ó���Ҷ����� ��ٸ���.
			Sleep(50);

		// accept()
		client_sock = accept(server_sock, (SOCKADDR *)&client_addr, &addrlen);
		if(client_sock == INVALID_SOCKET)
		{
			AfxMessageBox(L"accept() ����");
			return 0;
		}

		pISS->mClientThreadSock = client_sock;
		pISS->mClientAddr = client_addr;
		// Client_Thread ����
		pISS->mClientThread = AfxBeginThread(Client_Thread, (LPVOID)pISS);
	}

	return 0;
}

UINT ImageStreamingServer::Client_Thread(LPVOID arg) 
{
	ImageStreamingServer *pISS = (ImageStreamingServer*)arg;
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	CWinThread *clientThread;

	int retval;
	
	client_sock = pISS->mClientThreadSock;	//���� ������κ��� ��� ������ Client�� ������ ����
	client_addr = pISS->mClientAddr;
	pISS->mClientThreadSock = NULL;
	clientThread = pISS->mClientThread;

	// client�� GET ��û �ޱ�
	if((retval = recv(client_sock, (LPSTR)pISS->buf, BUF_SIZE,0)) == SOCKET_ERROR) {
		AfxMessageBox(L"recv() ����");
		return 0;
	}

	if(strncmp(pISS->buf, "GET /?method=camView", 20) == 0) {
		// camView�� vector�� �����Ѵ�.
		pISS->mClientThreads.push_back(clientThread);
		pISS->mClientSockets.push_back(client_sock);
		pISS->sendMjpeg(client_sock, pISS->currentImage);
	} else if(strncmp(pISS->buf, "GET /?method=listView", 21) == 0) {
		// ���� ����Ʈ�� ����
		pISS->sendList(client_sock);
	} else if(strncmp(pISS->buf, "GET /?method=imageView&name=", 28) == 0) {
		CString parameters, name, width, height;
		CString name_val, width_val, height_val;
		
		// name ���� ������� �����ϰ� �Ľ��ؼ� ������ ����
		AfxExtractSubString(parameters, CA2T(pISS->buf), 1, ' ');

		AfxExtractSubString(name, parameters, 1, '&');
		AfxExtractSubString(width, parameters, 2, '&');
		AfxExtractSubString(height, parameters, 3, '&');

		AfxExtractSubString(name_val, name, 1, '=');
		AfxExtractSubString(width_val, width, 1, '=');
		AfxExtractSubString(height_val, height, 1, '=');

		// name �� �̻��Ѱɷ� ���� �� ó��...
		/*if(fileName.) {

		}*/
		
		if(width_val == "" || height_val == "") {
			// width �� height�� ������ default�� 640 x 480
			pISS->sendResizedImage(client_sock, CT2A(name_val), 640, 480);
		} else {
			pISS->sendResizedImage(client_sock, CT2A(name_val), _ttoi(width_val), _ttoi(height_val));
		}
	} else {
		// ���� �޽���
		// pISS->sendErrorMsg();
	}

	// Ŭ���̾�Ʈ ���� �� ���� ����
	pISS->mClientThread = NULL;
	closesocket(client_sock);

	return 0;
}

// camView Thread�� Ư�� �� �̻� ����� ���� �������� ����� Thread�� ���δ�.
UINT ImageStreamingServer::Cache_Thread(LPVOID arg)
{
	ImageStreamingServer *pISS = (ImageStreamingServer*)arg;
	CWinThread *clientThread = NULL;
	SOCKET client_sock;

	// Server�� Stop() �޼ҵ带 �θ��� ������ 
	while(1) {
		if(pISS->mClientThreads.size() >= MAX_NUM_CAMVIEW_THREAD) {
			// ���� ������(���� �տ� �ִ�) Thread�� socket�� �����Ѵ�.
			clientThread = pISS->mClientThreads.front();
			client_sock = pISS->mClientSockets.front();
			
			if(clientThread->m_hThread != NULL) {
				// Thread �� socket�� ���� ���� ��Ű�� vector���� ����.
				TerminateThread(clientThread->m_hThread, NULL);
				clientThread = NULL;
				pISS->mClientThreads.erase(pISS->mClientThreads.begin());

				closesocket(client_sock);
				pISS->mClientSockets.erase(pISS->mClientSockets.begin());
			}
		}
		Sleep(1000);
	}

	return 0;
}

void ImageStreamingServer::sendImageHeader(SOCKET client_sock, BOOL mode)
{
	char buf[BUF_SIZE];

	if(mode) {
		// �̹��� ��� ����
		sprintf_s(buf, BUF_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n");
		send(client_sock, buf, strlen(buf), 0);
	} else {
		// �̹��� ��� ����(replace)
		sprintf_s(buf, BUF_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=--boundary\r\n");
		send(client_sock, buf, strlen(buf), 0);
	}
}

void ImageStreamingServer::sendImage(SOCKET client_sock, IplImage *sendImage, BOOL mode)
{
	// �޸𸮿��� IplImage �� jpeg stream ���� ��ȯ
	vector<int> params(0);
	vector<unsigned char> imgbuf(sendImage->imageSize);
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(95);
	imencode(".jpg", Mat(sendImage), imgbuf, params);

	// jpg stream (vector<unsigned char>) �� char* �� ��ȯ
	unsigned char *pimgbuf =  &imgbuf[0]; 

	if(mode) {
		// �Ϲ� �̹��� ���� ���
		sprintf_s(buf, BUF_SIZE,"Content-Length: %d\r\n\r\n", imgbuf.size());
		send(client_sock, buf, strlen(buf), 0);
		send(client_sock, (char*)pimgbuf, imgbuf.size(), 0);
		
		sprintf_s(buf, BUF_SIZE, "\r\n");
		send(client_sock, buf, strlen(buf), 0);
	} else {
		// ������ �̹��� ���� ���
		sprintf_s(buf, BUF_SIZE,"\r\n--boundary\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", imgbuf.size());
		send(client_sock, buf, strlen(buf), 0);
		send(client_sock, (char*)pimgbuf, imgbuf.size(), 0);
		
		sprintf_s(buf, BUF_SIZE, "\r\n");
		send(client_sock, buf, strlen(buf), 0);
	}
}

void ImageStreamingServer::sendResizedImage(SOCKET client_sock, char *fileName, int width, int height)
{
	CString strFullPath = getImagePath()+"\\"+fileName;

	IplImage *resizedImage = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U,3);
	IplImage *loadImage = cvLoadImage(CT2A(strFullPath), CV_LOAD_IMAGE_UNCHANGED);
	if(loadImage == NULL) {
		//������ ���� ���
		return;
	}
	cvResize(loadImage, resizedImage, CV_INTER_LINEAR);

	// �Ϲ� �̹��� ���� ���� ����
	sendImageHeader(client_sock, DEFAULT_MODE);
	sendImage(client_sock, resizedImage, DEFAULT_MODE);

	if(loadImage != NULL) cvReleaseImage(&loadImage);
	if(resizedImage != NULL) cvReleaseImage(&resizedImage);
}

void ImageStreamingServer::sendMjpeg(SOCKET client_sock, IplImage *sendImage)
{
	// ������ �̹��� ���� ���� ����
	sendImageHeader(client_sock, REPLACE_MODE);
	
	while(1) {
		this->sendImage(client_sock, sendImage, REPLACE_MODE);

		// 15 ���������� ����
		Sleep(1000/15);
	}
}

void ImageStreamingServer::sendList(SOCKET client_sock)
{
	char buf[BUF_SIZE];
	
	// ĸ�� ���� ����Ʈ ������
	CFileFind finder;
	CString strWildCard(getImagePath());
	strWildCard += "\\cam-*.jpg";
	
	CString sendString;
	vector<CString> sendList;

	BOOL bWorking = finder.FindFile( strWildCard );
	if(!bWorking) 
		sendString = "empty";

	while(bWorking){
		bWorking = finder.FindNextFile();
		
		if( finder.IsDots() || finder.IsDirectory() )
			continue;  
		else 
			sendList.push_back(finder.GetFileName());
	} 
	finder.Close();

	reverse(sendList.begin(), sendList.end());
	for(unsigned int i=0;i<sendList.size();i++) {
		sendString = sendString+";"+sendList[i];
	}

	// ����Ʈ�� string ���� ����
	sprintf_s(buf, BUF_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n", strlen(CT2A(sendString)));
	send(client_sock, buf, strlen(buf), 0);
	send(client_sock, CT2A(sendString), strlen(CT2A(sendString)), 0);

	sprintf_s(buf, BUF_SIZE, "\r\n");
	send(client_sock, buf, strlen(buf), 0);
}

CString ImageStreamingServer::getImagePath()
{
	CString	mPath;
	TCHAR iniPath[MAX_PATH];
	TCHAR imagePath[MAX_PATH];

	// iniPath ���
	GetCurrentDirectory(MAX_PATH, iniPath);
	wcscat_s(iniPath, L"\\config.ini");

	// imagePath ���
	GetPrivateProfileString(L"Motion_Detector", L"Path", L"C:\\", imagePath, MAX_PATH, iniPath);
	mPath.Append(imagePath);

	return mPath;
}