#include "Util.h"
#pragma once

enum {
	FW_INSERT,
	FW_WRITE
};

struct DATA_BUF {
	unsigned char *buf;
	int size;

	DATA_BUF() {
		buf = NULL;
		size = 0;
	}
};

//空バッファを作成する
void BufferCreate(DATA_BUF *data, int size);

//ファイルをバッファに読込み
int BufferLoad(DATA_BUF *data, char* fname);

//バッファ開放
void BufferRelease(DATA_BUF *data);

//バッファをコピーする
void BufferCopy(DATA_BUF *dest, DATA_BUF *src);

//書き込み
int BufferWrite(DATA_BUF *data, int offset, unsigned char *str, int size, int Mode);

//文字置き換え
int BufferSubStr(DATA_BUF *data, unsigned char *str1, int arg1, unsigned char *str2, int arg2);

//指定バイトのintを指定サイズのchar配列に格納する
unsigned char* SetByteNum(int Num, int Byte);

//char型の指定バイトをまとめてintで返す
int GetByteNum(unsigned char *p, int Byte);
