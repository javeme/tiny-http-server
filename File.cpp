
#include "File.h"
using namespace bluemeiIO;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

File::File(unsigned long bufferSize)
{
	init("","",bufferSize);
}
/*
File::File(string path,string openMode)
{
	this->File::File(path,openMode,BUFFER_SIZE);
}
*/
File::File(string path,string openMode,unsigned long bufferSize)
{
	init(path,openMode,bufferSize);
}
File::~File()
{
	close();
	delete[]buffer;
}
void File::init(string path,string openMode,unsigned long bufferSize)
{
	m_bCloseAble=false;
	m_nBufferSize=bufferSize;
	m_nUsedBufLength=0;
	buffer=new char[m_nBufferSize];
	if(path!="")
		openFile(path,openMode);
}
void File::openFile(string& path,string openMode)
{
	//path="./root/"+path;
	if(path=="")
		throw IOException("文件为空.");
	m_pFile=fopen(path.c_str(),openMode.c_str());
	if(m_pFile==NULL)
		throw IOException("'"+path+"'文件打开失败.");
	m_bCloseAble=true;
}
void File::writeBytes(char buf[],int length)
{
	if(m_nUsedBufLength<0)
		throw IOException("写入文件异常.");
	for(int i=0;i<length;i++)
	{
		buffer[m_nUsedBufLength++]=buf[i];
		if(m_nUsedBufLength >= m_nBufferSize)
			this->flush();
	}
}
void File::writeLine(string value)
{
	value.append("\n");
	writeBytes( (char*)value.c_str(), strlen(value.c_str()) );
}
void File::writeInt(int value)
{
	char buf[sizeof(int)];
	for(int i=0;i<sizeof(value);i++)
	{
		buf[i]=(value>>i*8) & 0xff;
	}
	writeBytes(buf,sizeof(int));
}

int File::readBytes(char buf[],int length)
{
	unsigned int nReturn=::fread(buf,sizeof(char),length,m_pFile);
	if(nReturn!=0 && nReturn!=length)
	{
		close();
		throw IOException("文件读取出错.");
	}
	return nReturn;
}
string File::readLine()
{
	int readedLen=0;
	char buf[BUFFER_SIZE]="a";
	do
	{
		readedLen+=readBytes(&buf[readedLen],1);
		if(readedLen>BUFFER_SIZE)
			throw IOException("缓冲已满.");
	}while(readedLen!=0 && buf[readedLen-1]!='\0' && buf[readedLen-1]!='\n');
	if(readedLen<=0)
		return "";
	string line(buf,readedLen-1);
	return line;
}
int File::readInt()
{
	int value=0;
	char buf[sizeof(int)];
	this->readBytes(buf,sizeof(int));
	for(int i=0;i<sizeof(int);i++)
	{
		value+=(buf[i] & 0xff)<<8*i;
	}
	return value;
}
int File::getSize()
{
	fseek(m_pFile,0,SEEK_END); 
	int length=ftell(m_pFile);
	rewind(m_pFile);//重定位到开头
	return length;
}
int File::readAll(char buf[],int bufSize)
{
	if(bufSize<getSize())
	{
		return -1;
	}
	return this->readBytes(buf,getSize());
}
void File::flush()
{
	if(m_nUsedBufLength==0)
		return;
	unsigned int nReturn=::fwrite(buffer,sizeof(char),m_nUsedBufLength,m_pFile);
	if(nReturn!=m_nUsedBufLength)
	{
		throw IOException("写入文件出错.");
	}
	m_nUsedBufLength=0;
}
void File::close()
{
	if(!m_bCloseAble)
		return;
	flush();
	if(::fclose(this->m_pFile))
		throw IOException("关闭文件出错.");
	m_bCloseAble=false;
}
