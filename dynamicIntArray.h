#ifndef __DYNAMIC_INT_ARRAY_H__
#define __DYNAMIC_INT_ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

///////////////////////////////////////////////////////////////////////////////////////
/// Definitions & Macros
///////////////////////////////////////////////////////////////////////////////////////

// 프로그램 디버그 출력 여부 매크로 상수
#define IS_PRINT_DEBUG	1
// 프로그램 에러 출력 여부 매크로 상수
#define IS_PRINT_ERROR	1

// 프로그램 출력 열거형
enum PRINT_TYPE
{
	ERROR	= -1,	// 에러 메시지
	NORMAL	= 0,	// 정상 메시지
	DEBUG	= 1	// 디버그 메시지
};

// 프로그램 상태 열거형
enum STATUS
{
	UNKNOWN		= -2,	// 알 수 없는 상태
	NO		= -1,	// 아니오
	FAIL		= -1,	// 실패
	NONE		= 0,	// 존재하지 않음
	SUCCESS		= 1,	// 성공
	YES		= 1	// 예
};

// 조건 함수를 지정하기 위한 함수 포인터
typedef int (*compareInt1Param_f)(int value);

// int 형 동적 배열을 관리하는 구조체
typedef struct dynamicIntArray_s dynamicIntArray_t;
struct dynamicIntArray_s
{
	// 동적 배열 전체 크기
	int size;
	// 동적 배열의 주소
	int *data;
};

///////////////////////////////////////////////////////////////////////////////////////
/// Functions for dynamicIntArray_t
///////////////////////////////////////////////////////////////////////////////////////

dynamicIntArray_t *dynamicIntArrayNew(int size);
int dynamicIntArrayInitialize(dynamicIntArray_t *array, int size);
dynamicIntArray_t *dynamicIntArrayResize(dynamicIntArray_t *array, int isKeep);
int dynamicIntArrayClear(dynamicIntArray_t *array);
int dynamicIntArrayFinal(dynamicIntArray_t *array);
int dynamicIntArrayDelete(dynamicIntArray_t **array);

int dynamicIntArraySetElement(const dynamicIntArray_t *array, int index, int datum);
int dynamicIntArrayGetElement(const dynamicIntArray_t *array, int index, int *isError);

dynamicIntArray_t *dynamicIntArrayAppend(dynamicIntArray_t *array, int datum);
dynamicIntArray_t *dynamicIntArrayInsertAt(dynamicIntArray_t *array, int index, int datum);
dynamicIntArray_t *dynamicIntArrayRemoveAt(dynamicIntArray_t *array, int index);

int dynamicIntArrayIndexOf(const dynamicIntArray_t *array, int datum);
int dynamicIntArrayLastIndexOf(const dynamicIntArray_t *array, int datum);
int dynamicIntArrayFind(const dynamicIntArray_t *array, compareInt1Param_f func);

dynamicIntArray_t *dynamicIntArrayCopy(dynamicIntArray_t *dst, int dstIndex, const dynamicIntArray_t *src, int srcIndex, int size);
dynamicIntArray_t *dynamicIntArrayClone(const dynamicIntArray_t *original);

int dynamicIntArrayReverse(const dynamicIntArray_t *array);
int dynamicIntArrayFill(const dynamicIntArray_t *array, int datum);
int *dynamicIntArrayGetArrayPtr(const dynamicIntArray_t *array);
int dynamicIntArrayCheckBoundary(const dynamicIntArray_t *array, int index);
void dynamicIntArrayPrintAll(const dynamicIntArray_t *array);

int dynamicIntArrayGetSize(const dynamicIntArray_t *array);
int dynamicIntArraySetSize(dynamicIntArray_t *array, int size);
int dynamicIntArrayIncSize(dynamicIntArray_t *array);
int dynamicIntArrayDecSize(dynamicIntArray_t *array);

///////////////////////////////////////////////////////////////////////////////////////
/// Common Util Functions
///////////////////////////////////////////////////////////////////////////////////////

void printMsg(const char *msg, int type, int argc, ...);
int checkObjectNull(const void *object, const char *msg);

#endif // #ifndef __DYANMIC_INT_ARRAY_H__

