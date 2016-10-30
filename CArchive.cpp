#include "CArchive.h"


//�o�C�g�y�A���k
//AAAABBBBBBCDDDDDDDDDDDDDDDDD
//AAAA111C22222222D
//BPE1BB2DD322 AAAA111C3333D
int CArchive::CmpBPE(DATA_BUF *rdata)
{
	unsigned char PairName[256] = { 0 };		//�y�A�e�[�u���̖��O
	unsigned char PairTable[256][2] = { 0 };	//�y�A�e�[�u��
	unsigned char TableNum = 0;					//�g�p���Ă���y�A�e�[�u���̐�

	//�f�[�^����
	BufferCopy(rdata, this->data);

	long i = 0;
	int cmpcnt = 0;//���k��
	while (1)
	{
		//���Ƀy�A�ݒ�
		PairTable[TableNum][0] = rdata->buf[i];
		PairTable[TableNum][1] = rdata->buf[i + 1];

		//�y�A�̐��𐔂���
		int PairNum = CountPair(PairTable[TableNum]);

		if (PairNum >= 3)
		{
			unsigned char c;
			if (UnuseChar(rdata,&c)) {
				PairName[TableNum] = c;

				//�����u������
				BufferSubStr(rdata, PairTable[TableNum], 2, &PairName[TableNum], 1);

				TableNum++;

				//���k�񐔉��Z
				cmpcnt++;

				//����ȏ㈳�k�ł��Ȃ��̂ŏI��
				if (TableNum >= 255)break;

				continue;
			} else {
				//����ȏ�o�^�ł��Ȃ��̂ŏI��
				break;
			}
		}

		//2���������Ă���
		i += 2;

		//�t�@�C����S�������̂ŏI��
		if (i > rdata->size) {
			if (cmpcnt >= 1) {
				//1��ȏ㈳�k�����̂ł�����x���k����
				i = 0;
				cmpcnt = 0;
				continue;
			}
			break;
		}

		//����ȏ㈳�k�ł��Ȃ��̂ŏI��
		if (TableNum >= 255)break;
	}
	
	//���k�t�@�C���Ƀy�A�e�[�u������g�ݍ���(3Bytes*TableNum + 1(�e�[�u���̐�) + 3(�w�b�_�[))
	int offset = 0;

	//�w�b�_�[
	BufferWrite(rdata, offset, (unsigned char*)"BPE", 3, FW_INSERT);
	offset += 3;

	//�e�[�u����(1�o�C�g)
	unsigned char *tmp;
	tmp = SetByteNum(TableNum, 1);
	BufferWrite(rdata, offset, (unsigned char*)tmp, 1, FW_INSERT);
	SAFE_DELETE_ARRAY(tmp);
	offset++;

	//�e�[�u���f�[�^
	for (int i = 0; i<TableNum; i++)
	{
		BufferWrite(rdata, offset, (unsigned char*)&PairName[i], 1, FW_INSERT);
		offset++;
		BufferWrite(rdata, offset, (unsigned char*)&PairTable[i][0], 1, FW_INSERT);
		offset++;
		BufferWrite(rdata, offset, (unsigned char*)&PairTable[i][1], 1, FW_INSERT);
		offset++;
	}

	return 0;
}

int CArchive::DecBPE(DATA_BUF *rdata)
{
	unsigned char PairName[256] = { 0 };		//�y�A�e�[�u���̖��O
	unsigned char PairTable[256][2] = { 0 };	//�y�A�e�[�u��
	unsigned char TableNum = 0;	//�g�p���Ă���y�A�e�[�u���̐�

	int offset = 0;

	//�e�[�u�������
	if (this->data->buf[0] != 'B' || this->data->buf[1] != 'P' || this->data->buf[2] != 'E') {
		//���k����Ă��Ȃ��̂Ŏ��s
		return 1;
	}
	offset += 3;

	TableNum = GetByteNum(&this->data->buf[offset], 1);
	offset++;

	for (int i = 0; i<TableNum; i++) {
		PairName[i]		= this->data->buf[offset];
		PairTable[i][0]	= this->data->buf[offset + 1];
		PairTable[i][1]	= this->data->buf[offset + 2];

		offset += 3;
	}

	//���C�������
	rdata->size = this->data->size - offset;
	BufferCreate(rdata, rdata->size);
	memcpy(rdata->buf, &this->data->buf[offset], rdata->size);

	for (int i = (TableNum - 1); i >= 0; i--) {
		//�����u������
		BufferSubStr(rdata, &PairName[i], 1, PairTable[i], 2);
	}

	return 0;
}

/**
* �g�p���Ă��Ȃ�������Ԃ�
*/
bool CArchive::UnuseChar(DATA_BUF *rdata,unsigned char *c)
{
	for (int i = 1; i <= 255; i++) {
		bool chk = true;
		for (int j = 0; j < rdata->size; j++) {
			if ((unsigned char)i == (unsigned char)rdata->buf[j]) {
				chk = false;
			}
		}
		if (chk) {
			*c = (unsigned char)i;
			return true;
		}
	}
	return false;
}

/**
 * 
 */
int CArchive::CountPair(unsigned char *PairTable) {

	int PairNum = 0;
	for (int i = 0; i < data->size - 1; i += 2) {
		if (PairTable[0] == data->buf[i] && PairTable[1] == data->buf[i + 1]) {
			PairNum++;
		}
	}

	return PairNum;
}