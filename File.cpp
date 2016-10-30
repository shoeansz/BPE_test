#include"File.h"

//��t�@�C�����쐬����
void BufferCreate(DATA_BUF *data,int size) {
	BufferRelease(data);

	data->size = size;
	data->buf = new unsigned char[data->size+1];//\0�̕���ڂɂƂ�
	data->buf[0] = '\0';
}


//�t�@�C�����������ɓǍ���
int BufferLoad(DATA_BUF *data,char *fname) {
	FILE *fp;
	
	fopen_s(&fp,fname,"rb");
	if(fp != NULL) {
		fseek(fp,0,SEEK_END);
		
		//��t�@�C�����쐬
		BufferCreate(data,ftell(fp));

		fseek(fp,0L,SEEK_SET);

		fread((void *)data->buf,sizeof(unsigned char),data->size,fp);
		
		printf("����Ǎ��݁@%s\n",fname);
	} else {
		printf("%s�����݂��Ȃ����ۂ��B\n",fname);
		return 0;
	}
	fclose(fp);
	return 1;
}

//�t�@�C���J��
void BufferRelease(DATA_BUF *data) {
	SAFE_DELETE_ARRAY(data->buf);
	data->size = 0;
}

//�t�@�C���R�s�[
void BufferCopy(DATA_BUF *dest,DATA_BUF *src) {
	BufferCreate(dest,src->size);

	memcpy(dest->buf,src->buf,dest->size);
}

//�t�@�C���ɏ�������
int BufferWrite(DATA_BUF *data,int offset,unsigned char *str,int size,int Mode) {

	if(Mode == FW_INSERT) {
		//�}��
		DATA_BUF tmp;
		BufferCopy(&tmp,data);

		int nSize = data->size + size;
		BufferRelease(data);
		BufferCreate(data,nSize);

		//�R�s�[
		memcpy(&data->buf[0],&tmp.buf[0],offset);
		memcpy(&data->buf[offset],&str[0],size);
		memcpy(&data->buf[offset+size],&tmp.buf[offset],data->size-(offset+size));

		BufferRelease(&tmp);

	} else if(Mode == FW_WRITE) {
		//�㏑��
		int nSize = offset + size;
		if(data->size < nSize) {
			//���ӂ��̂ŃG���[
			return 1;
		}
		
		//�㏑��
		memcpy(&data->buf[offset],str,strlen((char*)str));
	}

	return 0;
}


//�����u������
int BufferSubStr(DATA_BUF *data,unsigned char *str1,int arg1,unsigned char *str2,int arg2) {
	DATA_BUF tmp;
	int offset = 0;
	int i = 0;

	int Num = 0;

	//�u����������̐��𐔂���
	while(1) {
		//�����񌟍�
		bool chk = true;
		for(int j=0;j<arg1;j++) {
			if((i+j) >= data->size || data->buf[i+j] != str1[j]) {
				chk = false;
				break;
			}
		}

		if(chk) {
			//�����񂪈�v�����ꍇ
			Num++;
			i += arg1;
			continue;
		}

		i++;
		if(i >= data->size)break;
	}
	
	int size = data->size + ((arg2 - arg1)*Num);
	
	//printf("Num:%d = middle:%d->%d\n",Num,data->size,size);

	if(size < 0) {
		//�G���[
		return 1;
	}

	//�R�s�[
	BufferCopy(&tmp,data);

	BufferRelease(data);
	BufferCreate(data,size);

	//�o�b�t�@��������
	i = 0;
	while(1) {
		//�����񌟍�
		bool chk = true;
		for(int j=0;j<arg1;j++) {
			if((i+j) >= tmp.size || tmp.buf[i+j] != str1[j]) {
				chk = false;
			}
		}

		if(chk) {
			//�����񂪈�v�����ꍇ
			memcpy(&data->buf[offset],str2,arg2);
			i += arg1;
			offset += arg2;
			continue;
		}

		memcpy(&data->buf[offset],&tmp.buf[i],1);
		i++;
		offset++;

		if(i >= tmp.size)break;
	}

	BufferRelease(&tmp);

	return 0;
}


//�w��o�C�g��int���w��T�C�Y��char�z��Ɋi�[����
unsigned char* SetByteNum(int Num,int Byte) {
	unsigned char *p;
	p = new unsigned char[Byte];

	for(int i = 0;i<Byte;i++) {
		int Shift = (8*i);
		p[i] = ((Num & (255 << Shift)) >> Shift);
	}

	return p;
}


//char�^�̎w��o�C�g���܂Ƃ߂�int�ŕԂ�
int GetByteNum(unsigned char *p,int Byte) {
	int ret = 0;

	for(int i = 0;i<Byte;i++) {
		int Shift = (8*i);
		unsigned char No = *(p+i);
		ret |= (No << Shift);
	}

	return ret;
}