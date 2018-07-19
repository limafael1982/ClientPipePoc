#ifndef __CLIENT_PIPE_CLASS_H__
#define __CLIENT_PIPE_CLASS_H__

#pragma once

#include "stdafx.h"
#include <windows.h> 
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 1024
#define TIMETOWAIT 20000

class ClientPipeClass
{
public:
	ClientPipeClass();
	~ClientPipeClass();

	int tryToOpenPipe();
	int readFromServer();
	int sendMessageToServer();

protected:

	void closeHandle();

private:

	const wchar_t *lpvMessage;
	const wchar_t *lpszPipename;
	HANDLE hPipe;
	TCHAR chBuf[BUFSIZE];
	BOOL fSuccess;
	DWORD cbRead;
	DWORD cbToWrite;
	DWORD cbWritten;
	DWORD dwMode;

};
#endif /* __CLIENT_PIPE_CLASS_H__ */
