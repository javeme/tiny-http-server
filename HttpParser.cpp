#include "HttpParser.h"

HttpParser::HttpParser()
{
	;
}
HttpParser::~HttpParser()
{
	;
}
void HttpParser::parse(string& src,HttpRequest& request)
{
	int positionStart, positionEnd ,tmp;
	//��ȡ����ʽ��ҳ������
	positionStart=src.find("/",positionStart)+1;
	if((positionStart=src.find("GET",0))>=0)
	{
		request.type=GET;
	}
	else if((positionStart=src.find("POST"))>=0)
	{
		request.type=POST;
		//��ȡ�ϴ������Ʊ�ʶ
		tmp=src.find("boundary",0);
		if(tmp>0)
		{
			tmp+=sizeof("boundary");//Content-Length
			positionEnd=src.find("\n",tmp)-1;
			request.boundary=src.substr(tmp,positionEnd-tmp);
			if( (request.boundary!=""))//
			{
				request.type=UPLOAD;//�ϴ�	
				request.isStart=false;
				src=src.replace(tmp,positionEnd-tmp,"");
			}
		}		
	}
	else if(request.type!=UPLOAD)
		request.type=UNKNOWN;//δ֪
	if(positionStart>0)
	{
		positionEnd=src.find("HTTP",0);
		request.requestUrl="./root/"+src.substr(positionStart,positionEnd-positionStart);
	}
	else
		request.requestUrl="./root/index.html";
	//��ȡ����
	tmp=src.find("Content-Length:",0);
	if(tmp>0)
	{
		tmp+=sizeof("Content-Length:");
		positionEnd=src.find("\n",tmp);
		request.contenfLength=atol(src.substr(tmp,positionEnd-tmp).c_str());
	}
	else if(request.contenfLength<=0)
		request.contenfLength=src.length();
}
//��ȡ�ϴ�����
void HttpParser::dealWithUpload(string& content, HttpRequest& request)
{
	//Uploader uploader
	int start,end=0;
	start=content.find(request.boundary,0);
	if(start>=0)
	{
		if(!request.isStart)
		{
			//��ȡ�ļ�����
			if(content.find("upload",0)<0 || (start=content.find("filename"))<0)
				return;
			if( (start=content.find("\"",start))<0)
				return;
			start+=1;
			if((end=content.find("\"",start))<0)
				return;
			request.fileName="./root/upload/"+content.substr(start,end-start);
			start=content.find("Content-Type:");
			start=content.find("\n",start);
			start+=string("\n").length()*2;	
			request.isStart=true; 
				printf(">>>>>>");printf(request.fileName.c_str());printf("<<<<<<\n");
		}
		else
		{
			end=start;
			request.isStart=false;
		}
	}
	else if(request.isStart)//�����ʱ���м��
	{
		start=0;
	}
	if(request.isStart)//��ʾδ���
		end=content.find(request.boundary,start)-2;//Ϊ�λ�������
	if(end>0)
	{
		request.isStart=false;
	}
	else if(request.isStart)//�����ʱ���м��
	{
		end=content.length();
	}		
	if(end-start<=0 || start<0 || end<=0)
		return;
	File file(end-start);
	try{
		file.openFile(request.fileName,"ab");
		file.writeBytes((char*)(content.substr(start,end-start).c_str()),end-start);
		file.flush();
	}catch(IOException e)
	{
		e.printException();
	}
}