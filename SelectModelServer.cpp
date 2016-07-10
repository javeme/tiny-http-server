#include "SelectModelServer.h"

#include "ReceiveAble.h"

#pragma comment(lib,"ws2_32.lib")

SelectModelServer::SelectModelServer(ReceiveAble& receiver)
{
	this->m_pReceiver=&receiver;
	m_nCurrentConCount=0;
	/*for (int i=0;i<FD_SETSIZE;i++)
	{
		m_bUseableArray[i]=true;
	}*/
	for (int i=0;i<FD_SETSIZE;i++)
	{
		m_socketArray[i]=INVALID_SOCKET;
	}
}
SelectModelServer::~SelectModelServer()
{
	closeServer();
}
void SelectModelServer::init(unsigned short port)
{
	printf("init server.\n");
	if(m_nCurrentConCount!=0)
		return;
	//1.��ʼ������
	SocketTools::initSocketContext();
	//2.�����׽���
	m_serverSocket=socket(AF_INET,SOCK_STREAM,0);//DGRAM SOCK_STREAM
	if(m_serverSocket<0)
		throw SocketException(WSAGetLastError(),"���������׽���ʧ��");
	//3.�󶨱���ip�Ͷ˿�
	sockaddr_in local_addr;
	int length =sizeof(sockaddr_in);
	memset(&local_addr,0,length);
	local_addr.sin_family=AF_INET;
	local_addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//htonl(inet_addr("127.0.0.1"));
	local_addr.sin_port=htons(port);
	if(bind(m_serverSocket,(struct sockaddr*)&local_addr,length) == SOCKET_ERROR)		
		throw SocketException(WSAGetLastError(),"��ʧ��");
	//4.�����˿�
	if(listen(m_serverSocket,SOMAXCONN)== SOCKET_ERROR)		
		throw SocketException(WSAGetLastError(),"����ʧ��");
	//5.���÷�����
	unsigned long nonBlock=1;
	if(ioctl(m_serverSocket,FIONBIO,&nonBlock)==SOCKET_ERROR) 
		throw SocketException(WSAGetLastError(),"����Ϊ������ʧ��"); 
	//6.���뼯��
	this->m_socketArray[0]=m_serverSocket;
	m_nCurrentConCount++;
}
void SelectModelServer::startServer()
{
	printf("star server.\n");
	int nReturn,i;
	struct timeval timeDelay;
	timeDelay.tv_sec=0;
	timeDelay.tv_usec=1000*10;
	fd_set readFdSet;
	while (true)
	{
		FD_ZERO(&readFdSet);
		for (i = 0; i <FD_SETSIZE; i++)// m_nCurrentConCount
		{
			if(m_socketArray[i]!=INVALID_SOCKET)
				FD_SET(this->m_socketArray[i],&readFdSet);
		}	
		nReturn=select(0,&readFdSet,NULL,NULL,&timeDelay);
		if(nReturn==0)
		{
			if(timeDelay.tv_usec<1000*1000)//1��
				timeDelay.tv_usec+=100;//����0.1����
			continue;
		}
		else if(timeDelay.tv_usec>1000*2)//����2����
		{
			timeDelay.tv_usec-=1000;//����1����
		}
		else{
			printf("select sleep time:%d\n",timeDelay.tv_usec);
			//SocketTools::showWinsockErrorMsg(::WSAGetLastError());
		}
		//����ж϶Է��ر�
		for (i=0;i<FD_SETSIZE;i++)
		{
			if (FD_ISSET(m_socketArray[i], &readFdSet))
			{
				if(i==0)
					onAccept();
				else
					onReceive(m_socketArray[i]);
			}
		}
	}
}
void SelectModelServer::closeServer()
{
	//7.�رշ����׽���
	closesocket(m_serverSocket);
	//8.�������
	SocketTools::cleanUpSocketContext();
}
void SelectModelServer::onAccept()
{
	//5.��������
	SOCKET clientSocket;
	sockaddr_in peerAddr;
	int length=sizeof(sockaddr);
	//clientSocket=accept(m_serverSocket,(struct sockaddr*)(peerAddrs+m_nCurrentConCount),&length);
	clientSocket=accept(m_serverSocket,(struct sockaddr*)(&peerAddr),&length);
	this->m_socketArray[m_nCurrentConCount]=clientSocket;
	m_pReceiver->addPeerAddr(clientSocket,peerAddr);
	m_nCurrentConCount++;
}
void SelectModelServer::onReceive(SOCKET& clientSocket)
{
	//6.������Ϣ
	/*
	char buf[1024];
	int length=recv(clientSocket,buf,sizeof(buf),0);
	if(length<=0 ||  strcmp(buf,"exit")==0)
	{
		printf("\n error\n");
		onClose(clientSocket);
		return;
	}
	buf[length]='\0';
	printf("�յ���Ϣ(�ֽ���%d):%s",length,buf);
	//*/
	unsigned long length=0;
	ioctl(clientSocket,FIONREAD,&length);
	if(length==0)
	{
		this->onClose(clientSocket);
		return;
	}
	this->m_pReceiver->onReceive(clientSocket,*this);
}
void SelectModelServer::onClose(SOCKET& clientSocket)
{	
	m_pReceiver->removePeer(clientSocket);
	closesocket(clientSocket);
	clientSocket=INVALID_SOCKET;
	m_nCurrentConCount--;
}
void SelectModelServer::close(SOCKET&  socket)
{
	this->onClose(socket);
}