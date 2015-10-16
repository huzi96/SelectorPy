// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
//
//  Header.h
//  Classifier0
//
//  Created by Michael on 15/10/4.
//  Copyright ? 2015�� Michael. All rights reserved.
//
#pragma pack(8)
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define FileScale 10000000
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>
#include <sstream>
#include <Python.h>
#define HASHLEN 17
using namespace std;
const int line = 120;
const long num = 10000;
const long cache_size = line * num;
struct HASH_VALUE
{
	//char bytes[HASHLEN];
	long long seg1, seg2;
	HASH_VALUE() {}
	HASH_VALUE(string &str)//����ȫ
	{
		char *dist = (char *)this;
		for (int i = 0; i<16; i++)
		{
			dist[i] = str[i];
		}
	}
	HASH_VALUE(const char str[])//����ȫ
	{
		char *dist = (char *)this;
		for (int i = 0; i<16; i++)
		{
			dist[i] = str[i];
		}
	}
	char * c_str()//��ֵ�д��
	{
		char * p = (char *)this;
		return p;
	}
	void c_str(char *pos)//Dangerous!!!
	{
		for (int i = 0; i < 16; i++)
		{
			pos[i] = ((char *)this)[i];
		}
		pos[16] = 0;
	}
};
bool operator == (const HASH_VALUE &op1, const HASH_VALUE &op2);
ostream &operator << (ostream &out, const HASH_VALUE &op);

struct Info
{
	HASH_VALUE id, ads_id, pos_id, ip_id;//16bytes HASH value
	int lang;//en OR zh , 0->zh, 1->en
	char OS_info[HASHLEN];//whose length is unknown
	long timeStamp;//Unix Time stamp
	int stable, click;//is stable / is clicked
};


static string directory = "C:\\Data\\d";
//�����ļ�ѡ����
struct Selector
{
private:
	fstream file;
	int openedFileCode;
	int whichHalf;
	char *cache;
	//ѡ���i����¼
public:
	Selector() :cache(NULL), openedFileCode(0)
	{
		whichHalf = -1;
		cout << "Requesting for cache" << endl;
		cache = new char[FileScale * 104 /2 + 1000];
		cout << "Succeed" << endl;
	}

	//Ϊ˳������Ż���ѡ����
	Info sequence_read(int i)
	{
		//����Ӧ��ѡ���ĸ��ļ�
		unsigned file_code = i / FileScale + 1;
		if (openedFileCode == file_code)
		{
			//�Ѿ�������ȷ���ļ�
			//�����ڻ������еĵ�ַ
			unsigned bias = i % FileScale;
			if (i < 5000000)
			{
				if (whichHalf == 0)
				{
					return *(Info *)(cache + 104 * bias);
				}
				else
				{
					whichHalf = 0;
					file.seekg(0, file.beg);
					file.read(cache, 52 * FileScale);
					return *(Info *)(cache + 104 * bias);
				}
			}
			else
			{
				if (whichHalf == 1)
				{
					return *(Info *)(cache + 104 * (bias - 5000000));
				}
				else
				{
					whichHalf = 1;
					file.seekg(104 * 5000000, file.beg);
					file.read(cache, 52 * FileScale);
					return *(Info *)(cache + 104 * (bias - 5000000));
				}
			}
		}
		else if (openedFileCode == 0)
		{
			openedFileCode = file_code;
			stringstream ss;
			ss << file_code;
			file.open(directory + ss.str(), ios::in | ios::binary);
		}
		else
		{
			file.close();
			openedFileCode = file_code;
			stringstream ss;
			ss << file_code;
			file.open(directory + ss.str(), ios::in | ios::binary);
		}


		unsigned bias = i % FileScale;

		if (i < 5000000)
		{
			whichHalf = 0;
			file.seekg(0, file.beg);
			file.read(cache, 52 * FileScale);
			return *(Info *)(cache + 104 * bias);
		}
		else
		{
			whichHalf = 1;
			file.seekg(104 * 5000000, file.beg);
			file.read(cache, 52 * FileScale);
			return *(Info *)(cache + 104 * (bias - 5000000));
		}
	}

	~Selector()
	{
		if (openedFileCode != 0)
		{
			file.close();
		}
		delete[]cache;
	}
};

