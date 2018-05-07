#ifndef _TEACHER_H
#define _TEACHER_H
typedef struct _Teacher
{
	char name[64]; 
		int age;   
		char *p;
	long plen;
}Teacher;

// ����ṹ��
int encodeTeacher(Teacher* p, char** outData, int* outlen);
// ����ṹ��
int decodeTeacher(char* inData, int inLen, Teacher**p);
// �ͷ��ڴ溯��
void freeTeacher(Teacher** p);

#endif	// _TEACHER_H
