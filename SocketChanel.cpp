#include "SocketChanel.h"

SocketChanel::SocketChanel()
{
	;
}
SocketChanel::~SocketChanel()
{
	
}
//�Ƴ�
void SocketChanel::removeClient(const SOCKET key)
{
	removeClient(key);
}
//��ÿͻ���Ϣ
ClientMsg* SocketChanel::getClientMsg(const SOCKET key)
{
	ClientMsg* pValue=NULL;
	map <SOCKET, ClientMsg >::iterator iter;
	iter = m_clientsMap.find(key); 
	if (iter != m_clientsMap.end()) 
	{
		pValue=&(iter->second); 
	}
	return pValue;
}
//���öԷ���ַ
void SocketChanel::addPeerAddr(const SOCKET key,sockaddr_in addr)
{
	ClientMsg client;
	client.receivedLength=0;
	client.request.contenfLength=0;
	client.clientAddr=addr;
	this->m_clientsMap.insert(map<SOCKET,ClientMsg> :: value_type(key,client));
	m_bClose=false;
}
void SocketChanel::removePeer(const SOCKET key)
{
	/*
	ClientMsg *pClientMsg=this->getClientMsg(key);
	if(pClientMsg!=NULL)
	{
		if(pClientMsg->request!=NULL)
		{
			delete pClientMsg->request;
			pClientMsg->pRequest=NULL;
		}
	}*/
	this->m_clientsMap.erase(key);
}
//��ȡ�Է���ַ
void SocketChanel::getPeerAddr(const SOCKET key,sockaddr_in& addr)
{
	ClientMsg* pValue=getClientMsg(key);
	if(pValue!=NULL)
		addr=pValue->clientAddr;
}
//��ȡ�Է�ip
string SocketChanel::getPeerHost(const SOCKET key)
{
	sockaddr_in addr;
	getPeerAddr( key,addr);
	string host=::inet_ntoa(addr.sin_addr);
	return host;
}
//��öԷ��˿�
int SocketChanel::getPeerPort(const SOCKET key)
{
	sockaddr_in addr;
	getPeerAddr(key,addr);
	return addr.sin_port;
}

//���ֽ�,����ȡ���ֽ���
int SocketChanel::readBytes(char buffer[],int maxLength,SOCKET& socket)
{
	int length=::recv(socket,buffer,maxLength,0);	
	if(length==SOCKET_ERROR)//SOCKET_ERROR
	{
		int errorCode=WSAGetLastError();
			throw SocketException(errorCode);
	}	
	/*
	FILE *pFile; 
	if((pFile=fopen("ewceiveData.log","a+")))
	{
		fwrite(buffer,1,length,pFile);
		fclose(pFile);
	}	*/
	return length;
}
//������
int SocketChanel::readInt(SOCKET& socket)
{
	int sizeOfInt=sizeof(int);	
	char *buf=new char[sizeOfInt];
	int nReturnCode=readBytes(buf,sizeOfInt,socket);
	if(nReturnCode==sizeOfInt)
	{
		int value=SocketTools::bytesToInt(buf,sizeOfInt);
		delete[]buf;
		value=::ntohl(value);
		return value;
	}
	else
	{	
		delete[]buf;
		throw SocketException(::WSAGetLastError());
	}
}
//��ȡһ������,��������Ϊgbk������ʽ,����gbk����
string SocketChanel::readLineByGbk(SOCKET& socket)
{
	int count=0;
	char buffer[BUFFER_SIZE];
	char receivedChar;
	do{
		count+=readBytes(&buffer[count],1,socket);
		receivedChar=buffer[count-1];
	}while(receivedChar!='\n');
	buffer[count]='\0';
	string str(buffer);
	return str;
}
//���ַ���,��������Ϊutf-8������ʽ,����gbk���� --��ʵ��
string SocketChanel::readUtfString(SOCKET& socket)
{	
	string str;
	int len=this->readInt(socket);
	int count=0;
	if(len>0)
	{
		char *buffer=new char[len];
		count=this->readBytes(buffer,len,socket);
		if(count!=len)
			throw SocketException(::WSAGetLastError(),"��ȡutf�����ַ���ʧ��.");
		else
		{
			SocketTools::utf8ToGbk(str,buffer);
		}
	}
	return str;
}

/*
//�ر�
void SocketChanel::close()
{
	int nReturnCode=closesocket(this->m_hSocket);
	if(nReturnCode==SOCKET_ERROR)
	{
		throw SocketException(::WSAGetLastError());
	}
	m_bClose=true;
}*/
void SocketChanel::onReceive(SOCKET& socket,SelectModelServer& server)
{		
	ClientMsg *pClientMsg=this->getClientMsg(socket);
	string requestContent="";
	/*try{
		str=this->readLineByGbk(socket);
	}catch(SocketException e)
	{
		e.printErrorMsg();
		if(e.getError()==10054)
			server.close(socket);
		return;
	}*/
	char buf[BUFFER_SIZE];
	int len,count=0;
	try{
		while(true)
		{
			len=this->readBytes(buf,BUFFER_SIZE,socket);
			//if(pClientMsg->request.isStart)
				pClientMsg->receivedLength+=len;				
			requestContent+=string(buf,len);
			if(len==BUFFER_SIZE)
			{
				count++;
				if(count<10)
					continue;
			}
			break;			
		}
	}catch(SocketException e)
	{
		e.printErrorMsg();
		if(e.getError()==10054)
			server.close(socket);//���뽻��SelectModelServer����!
		return;
	}
	//printf("�յ���Ϣ(last size is %d) :%s\n",len,requestContent.c_str());
	//����	
	m_httpParser.parse(requestContent,pClientMsg->request);
	//if(pClientMsg->request.contenfLength>pClientMsg->receivedLength)
	//	printf("���ղ�ȫ(contenfLength is %d,received %d).\n",pClientMsg->request.contenfLength,pClientMsg->receivedLength);		
	
	if(pClientMsg->request.type==UPLOAD)
	{	
		/*if(request->boundary!="" && request->contenfLength>requestContent.length())
		{
			onReceive(socket,server);
			return;
		}*/
		m_httpParser.dealWithUpload(requestContent,pClientMsg->request);	
		//pClientMsg->request.requestUrl="./root/index.html";		
	}
	//��Ӧ
	HttpResponse response;
	string status="200 OK"; 
	File* pRequestFile=NULL;
	char *fileBuf=NULL;
	int size=0,readLength=0;
	if(pClientMsg->request.type!=UPLOAD)
	{
		try{
			pRequestFile=new File(pClientMsg->request.requestUrl,"rb");
		
			size=pRequestFile->getSize();
			fileBuf=new char[size+1];
			readLength=pRequestFile->readAll(fileBuf,size);
			
		}catch(IOException e)//�Ѿ�������Դ�Ƿ���Ҫ�ֶ��ͷ�?
		{
			status="404 file not found";
			e.printException();
		}
	}
	else
	{	
		//printf(">>>>>%d>>>>>>%d>>>>>>",pClientMsg->receivedLength,pClientMsg->request.contenfLength);
		float percent=((float)100*pClientMsg->receivedLength)/pClientMsg->request.contenfLength;
		if(!pClientMsg->request.isStart)
			pClientMsg->receivedLength=0;
		size=107;
		fileBuf=new char[size+1];
		sprintf(fileBuf,"�ϴ���С :%4.2f%%\n",percent);
		size=readLength=strlen(fileBuf);	
		if(percent<100)
		{
			printf(fileBuf);
			delete[]fileBuf;
			fileBuf=NULL;
			return;
		}
	}
	try{	
		if(readLength>0)
		{
			if(readLength!=size)
			{			
				printf("��ȡ�ļ�����.\n");
				status="500 cam't open the requested file.";
			}
			else
			{
				response.appendContentLength(readLength);
				fileBuf[readLength]=0;
			}
		}
		//response.writeString("HTTP/1.1 200 OK\nServer: blue 1.0\nContent-Type: text/html\n\n",socket);
		response.appendStatus(status.c_str());
		response.appendContentType("text/html");
			response.writeHttpHeader(socket);
		if(readLength>0)
		{
			response.writeBytes(fileBuf,readLength,socket);	
		}
		//server.close(socket);//Ϊ��Ҫ�رղ���ʾ?
	}
	catch(Exception e)
	{
		e.printException();
		server.close(socket);
	}
	if(fileBuf!=NULL)
		delete[]fileBuf;
}
