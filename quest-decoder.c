#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAXBUFF  65536

typedef struct {
	int m_EncodeNum;
	int m_Identification;
	int m_FileSize;
	int m_DataNumber;
	int m_Parity;
} TotalHeader;

typedef struct _DataHeader {
	int m_EncodeNum;
	int m_DataSize;
	int m_Parity;
	char m_FileName[10];
} DataHeader;

int main(int argc, char* argv[])
{
	char key[] = "alkdjh flds flsjadhf ;kjdfha;sdflkha;s33";
	int key_len = strlen(key);
	FILE* fp = fopen("quest.tex", "rb");
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	FILE* fpo = fopen("quest-decode.tex", "wb");
	char buff[MAXBUFF];
	int n, i;
	while (n = fread(buff, 1, MAXBUFF, fp))
	{
		for (i = 0; i < n; i++)
		{
			buff[i] ^= key[i % key_len];
		}
		fwrite(buff, 1, n, fpo);
	}
	fclose(fp);
	fclose(fpo);
	
	TotalHeader* pTotal_header;
	DataHeader* pData_header;
	system("mkdir decode");
	fp = fopen("quest-decode.tex", "rb");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* mbuff = malloc(size);
	fread(mbuff, 1, size, fp);
	fclose(fp);
	
	pTotal_header = (TotalHeader*)mbuff;
	printf("%d\n", pTotal_header->m_DataNumber);
	unsigned int data_ptr = sizeof(TotalHeader);
	char str[128];
	for (i = 0; i < pTotal_header->m_DataNumber; i++)
	{
		pData_header = (DataHeader*)&mbuff[data_ptr];
		
		sprintf(str, "decode\\%s.cfg", pData_header->m_FileName);
		fpo = fopen(str, "wb");
		fwrite(mbuff + data_ptr + sizeof(DataHeader), 1, pData_header->m_DataSize, fpo);
		fclose(fpo);
		
		printf("%s\n", pData_header->m_FileName);
		data_ptr += (sizeof(DataHeader) + pData_header->m_DataSize);
	}
	system("del quest-decode.tex");
	
	return 0;
}

