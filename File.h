#ifndef File_H_H
#define File_H_H


#include <iostream>
#include <string>
using namespace std;

#include "IOException.h"
using namespace bluemeiException;

namespace bluemeiIO
{
    #define BUFFER_SIZE 1024

	class File
	{
	public:
		File(unsigned long bufferSize);
		//File(string path,string openMode);
		File(string path="",string openMode="wb+",unsigned long bufferSize=BUFFER_SIZE);
		~File();
	private:
		void init(string path,string openMode,unsigned long bufferSize);
	public:
		void openFile(string& path,string openMode);

		virtual void writeBytes(char buf[],int length);
		virtual void writeLine(string value);
		virtual void writeInt(int value);

		virtual int readBytes(char buf[],int length);
		virtual string readLine();
		virtual int readInt();
		virtual int getSize();
		virtual int readAll(char buf[],int bufSize);

		virtual void flush();
		virtual void close();
	private:
		FILE *m_pFile;
		unsigned long m_nBufferSize,m_nUsedBufLength;
		char *buffer;
		bool m_bCloseAble;
	};
}
#endif