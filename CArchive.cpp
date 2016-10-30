#include "CArchive.h"


//バイトペア圧縮
//AAAABBBBBBCDDDDDDDDDDDDDDDDD
//AAAA111C22222222D
//BPE1BB2DD322 AAAA111C3333D
int CArchive::CmpBPE(DATA_BUF *rdata)
{
	unsigned char PairName[256] = { 0 };		//ペアテーブルの名前
	unsigned char PairTable[256][2] = { 0 };	//ペアテーブル
	unsigned char TableNum = 0;					//使用しているペアテーブルの数

	//データを代入
	BufferCopy(rdata, this->data);

	long i = 0;
	int cmpcnt = 0;//圧縮回数
	while (1)
	{
		//仮にペア設定
		PairTable[TableNum][0] = rdata->buf[i];
		PairTable[TableNum][1] = rdata->buf[i + 1];

		//ペアの数を数える
		int PairNum = CountPair(PairTable[TableNum]);

		if (PairNum >= 3)
		{
			unsigned char c;
			if (UnuseChar(rdata,&c)) {
				PairName[TableNum] = c;

				//文字置き換え
				BufferSubStr(rdata, PairTable[TableNum], 2, &PairName[TableNum], 1);

				TableNum++;

				//圧縮回数加算
				cmpcnt++;

				//これ以上圧縮できないので終了
				if (TableNum >= 255)break;

				continue;
			} else {
				//これ以上登録できないので終了
				break;
			}
		}

		//2文字ずつ見ていく
		i += 2;

		//ファイルを全部見たので終了
		if (i > rdata->size) {
			if (cmpcnt >= 1) {
				//1回以上圧縮したのでもう一度圧縮する
				i = 0;
				cmpcnt = 0;
				continue;
			}
			break;
		}

		//これ以上圧縮できないので終了
		if (TableNum >= 255)break;
	}
	
	//圧縮ファイルにペアテーブル情報を組み込み(3Bytes*TableNum + 1(テーブルの数) + 3(ヘッダー))
	int offset = 0;

	//ヘッダー
	BufferWrite(rdata, offset, (unsigned char*)"BPE", 3, FW_INSERT);
	offset += 3;

	//テーブル数(1バイト)
	unsigned char *tmp;
	tmp = SetByteNum(TableNum, 1);
	BufferWrite(rdata, offset, (unsigned char*)tmp, 1, FW_INSERT);
	SAFE_DELETE_ARRAY(tmp);
	offset++;

	//テーブルデータ
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
	unsigned char PairName[256] = { 0 };		//ペアテーブルの名前
	unsigned char PairTable[256][2] = { 0 };	//ペアテーブル
	unsigned char TableNum = 0;	//使用しているペアテーブルの数

	int offset = 0;

	//テーブル情報代入
	if (this->data->buf[0] != 'B' || this->data->buf[1] != 'P' || this->data->buf[2] != 'E') {
		//圧縮されていないので失敗
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

	//メイン情報代入
	rdata->size = this->data->size - offset;
	BufferCreate(rdata, rdata->size);
	memcpy(rdata->buf, &this->data->buf[offset], rdata->size);

	for (int i = (TableNum - 1); i >= 0; i--) {
		//文字置き換え
		BufferSubStr(rdata, &PairName[i], 1, PairTable[i], 2);
	}

	return 0;
}

/**
* 使用していない文字を返す
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