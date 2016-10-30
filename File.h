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

//��o�b�t�@���쐬����
void BufferCreate(DATA_BUF *data, int size);

//�t�@�C�����o�b�t�@�ɓǍ���
int BufferLoad(DATA_BUF *data, char* fname);

//�o�b�t�@�J��
void BufferRelease(DATA_BUF *data);

//�o�b�t�@���R�s�[����
void BufferCopy(DATA_BUF *dest, DATA_BUF *src);

//��������
int BufferWrite(DATA_BUF *data, int offset, unsigned char *str, int size, int Mode);

//�����u������
int BufferSubStr(DATA_BUF *data, unsigned char *str1, int arg1, unsigned char *str2, int arg2);

//�w��o�C�g��int���w��T�C�Y��char�z��Ɋi�[����
unsigned char* SetByteNum(int Num, int Byte);

//char�^�̎w��o�C�g���܂Ƃ߂�int�ŕԂ�
int GetByteNum(unsigned char *p, int Byte);
