#include"File.h"

#pragma once

class CArchive
{
private:
	DATA_BUF *data;

public:
	CArchive(DATA_BUF *data) {
		this->data = data;
	}

	int CmpBPE(DATA_BUF *rdata);
	int DecBPE(DATA_BUF *rdata);

private:

	bool UnuseChar(DATA_BUF *rdata, unsigned char *c);
	int CountPair(unsigned char *PairTable);
};