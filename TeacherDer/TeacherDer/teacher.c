#include "teacher.h"
#include "itcast_asn1_der.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	typedef struct _Teacher
	{
		char name[64];
		int age;
		char *p;
		long plen;
	}Teacher;
*/
int encodeTeacher(Teacher * p, char ** outData, int * outlen)
{
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = NULL;
	ITCAST_ANYBUF* pnext = NULL;
	// 1. ����name, char* -> ITCAST_ANYBUF, ����û�б���
	DER_ITCAST_String_To_AnyBuf(&ptemp, p->name, strlen(p->name)+1);
	// 1.1 ����name, phead������ĵ�һ�����ݽڵ�
	DER_ItAsn1_WritePrintableString(ptemp, &phead);
	DER_ITCAST_FreeQueue(ptemp);

	// 2. ����age, �������֮��, ��Ϊ�ڶ�������ڵ�, phead->next
	DER_ItAsn1_WriteInteger(p->age, &phead->next);

	// 3. ����p
	pnext = phead->next;
	EncodeChar(p->p, strlen(p->p)+1, &pnext->next);

	// 4. ����plen
	// nextָ�����
	pnext = pnext->next;
	DER_ItAsn1_WriteInteger(p->plen, &pnext->next);

	// 5. �������������, �õ�һITCAST_ANYBUF
	// ptemp �д洢���������������
	DER_ItAsn1_WriteSequence(phead, &ptemp);

	// 6. ����������ֵ����
	*outData = ptemp->pData;
	*outlen = ptemp->dataLen;

	// 7. �ͷ�����
	DER_ITCAST_FreeQueue(phead);

	return 0;
}

/*
	typedef struct _Teacher
	{
		char name[64];
		int age;
		char *p;
		long plen;
	}Teacher;
*/
int decodeTeacher(char * inData, int inLen, Teacher ** p)
{
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = NULL;
	ITCAST_ANYBUF* pnext = NULL;
	// ����һ���ڴ�洢TEacher�ṹ������
	Teacher* teacher = (Teacher*)malloc(sizeof(Teacher));
	memset(teacher, 0, sizeof(Teacher));

	// inData -> ���л�֮����ַ���, �����л�
	// inData->ITCAST_ANYBUF
	// ptemp -> ���л�֮��������е���������
	DER_ITCAST_String_To_AnyBuf(&ptemp, inData, inLen);
	// ptemp��ԭΪһ������
	// phead�д洢����name���л�֮�������
	DER_ItAsn1_ReadSequence(ptemp, &phead);
	DER_ITCAST_FreeQueue(ptemp);
	// ��ԭname��ֵ
	// ptemp�洢�Ľ���֮�������
	DER_ItAsn1_ReadPrintableString(phead, &ptemp);
	// ȡ��name��ԭʼֵ
	strcpy(teacher->name, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);

	// 2. ����age, pnextָ��Ľڵ�����ITCAST_ANYBUF
	pnext = phead->next;
	DER_ItAsn1_ReadInteger(pnext, &teacher->age);

	// 3. ����p
	pnext = pnext->next;
	DER_ItAsn1_ReadPrintableString(pnext, &ptemp);
	teacher->p = (char*)malloc(ptemp->dataLen);
	memset(teacher->p, 0, ptemp->dataLen);
	strcpy(teacher->p, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);

	// 4. plen
	pnext = pnext->next;
	DER_ItAsn1_ReadInteger(pnext, &teacher->plen);

	// 5. ����������ֵ
	*p = teacher;

	// 6. �ͷ�����
	DER_ITCAST_FreeQueue(phead);

	return 0;
}

void freeTeacher(Teacher ** p)
{
	if ((*p) != NULL)
	{
		if ((*p)->p != NULL)
		{
			free((*p)->p);
		}
		free(*p);
	}
}
