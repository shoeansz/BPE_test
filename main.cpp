#include <windows.h>
#include<crtdbg.h>
#include<stdio.h>
#include"CArchive.h"



int main(int argc, char* argv[]){

	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	


	FILE *fp;

	//圧縮
	DATA_BUF test;
	BufferLoad(&test, "./test.txt");
	CArchive *Archive1 = new CArchive(&test);
	DATA_BUF CmpData;

	Archive1->CmpBPE(&CmpData);

	//ファイル書き出し
	fopen_s(&fp, "./CmpData.txt", "wb");
	if (fp != NULL) {
		fwrite(CmpData.buf, sizeof(unsigned char), CmpData.size, fp);
	}
	delete Archive1;
	fclose(fp);


	//解凍
	CArchive *Archive2 = new CArchive(&CmpData);
	DATA_BUF DecData;

	Archive2->DecBPE(&DecData);

	//ファイル書き出し
	fopen_s(&fp, "./DecData.txt", "wb");
	if (fp != NULL) {
		fwrite(DecData.buf, sizeof(unsigned char), DecData.size, fp);
	}
	delete Archive2;
	fclose(fp);



	BufferRelease(&test);
	BufferRelease(&CmpData);
	BufferRelease(&DecData);



}