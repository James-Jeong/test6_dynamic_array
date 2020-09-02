#include "dynamicIntArray.h"

///////////////////////////////////////////////////////////////////////////////////////
/// Predefinitions of Static Util Functions
///////////////////////////////////////////////////////////////////////////////////////

static void *reallocateMemory(void *memory, size_t size);
static void *allocateMemory(size_t size);
static void clearMemory(void *memory, size_t size);
static void freeMemory(void **memory);
static int getBufferSize(const char *format, ...);
__inline static int getDigitOfNumber(int number);
__inline static void addNumberToString(char *string, int number);

///////////////////////////////////////////////////////////////////////////////////////
/// Functions for dynamicIntArray_t
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn dynamicIntArray_t *dynamicIntArrayNew(int size)
 * @brief 동적 배열 관리 구조체를 새로 할당하는 함수
 * @param size 동적 배열을 할당할 크기(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayNew(int size)
{
	dynamicIntArray_t *array = (dynamicIntArray_t*)allocateMemory(sizeof(dynamicIntArray_t));
	if(checkObjectNull(array, "메모리 참조 실패, 할당된 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayNew)") == YES)
	{
		return NULL;
	}

	if(dynamicIntArrayInitialize(array, size) == FAIL)
	{
		printMsg("초기화 실패, 할당된 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayNew)", DEBUG, 0);
		return NULL;
	}

	return array;
}

/**
 * @fn int dynamicIntArrayInitialize(dynamicIntArrya_t *array, int size)
 * @brief 동적 배열 관리 구조체를 초기화하는 함수
 * @param array 동적 배열 관리 구조체 포인터(출력)
 * @param size 동적 배열 크기(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayInitialize(dynamicIntArray_t *array, int size)
{
	if(checkObjectNull(array, "메모리 참조 실패, 할당된 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayInitialize)") == YES)
	{
		return FAIL;
	}

	size_t totalSize = size * sizeof(int);
	array->data = (int*)allocateMemory(totalSize);
	if(checkObjectNull(array->data, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayInitialize)") == YES)
	{
		free(array);
		return FAIL;
	}
	clearMemory(array->data, totalSize);

	if(dynamicIntArraySetSize(array, size) == FAIL)
	{
		printMsg("크기 지정 실패. (dynamicIntArrayInitialize, size:%d)", DEBUG, 1, size);
		if(dynamicIntArrayDelete(&array) == FAIL)
		{
			printMsg("동적 배열 관리 구조체 메모리 해제 실패. (dynamicIntArrayDelete)", DEBUG, 0);
		}
		return FAIL;
	}

	array->stringOfArray = NULL;

	return SUCCESS;
}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayResize(dynamicIntArray_t *array, int isKeep)
 * @brief 동적 배열 관리 구조체가 관리하고 있는 동적 배열을 재할당하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @param isKeep 재할당 시 기존 데이터 유지 여부(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayResize(dynamicIntArray_t *array, int isKeep)
{
	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayResize, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	if(size <= 0)
	{
		printMsg("크기 지정 실패. 크기가 0 보다 작거나 같음. (dynamicIntArrayResize, size:%d)", ERROR, 1, size);
		return NULL;
	}

	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayResize)") == YES)
	{
		return NULL;
	}

	size_t totalSize = size * sizeof(int);

	if(isKeep == YES)
	{
		arrayData = (int*)reallocateMemory(arrayData, totalSize);
	}
	else if(isKeep == NO)
	{
		freeMemory((void**)(&arrayData));
		arrayData = (int*)allocateMemory(totalSize);
		dynamicIntArrayClear(array);
	}
	else
	{
		printMsg("알 수 없는 isKeep 변수 값 사용. 재할당 취소. (dynamicIntArrayResize)", ERROR, 0);
		return NULL;
	}

	array->data = arrayData;
	return array;
}

/**
 * @fn int dynamicIntArrayClear(dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체를 초기화하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayClear(dynamicIntArray_t *array)
{
	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayClear)") == YES)
	{
		return FAIL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayClear, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	size_t totalSize = size * sizeof(int);
	clearMemory(arrayData, totalSize);
	array->size = 0;

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayFinal(dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체의 멤버 변수의 할당된 메모리를 해제하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayFinal(dynamicIntArray_t *array)
{
	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayFinal)") == YES)
	{
		return FAIL;
	}

	freeMemory((void**)(&arrayData));

	char *string = array->stringOfArray;
	if(checkObjectNull(string, NULL) == NO)
	{
		freeMemory((void**)(&string));
	}

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayDelete(dynamicIntArray_t **array)
 * @brief 동적 배열 관리 구조체의 할당된 메모리를 해제하는 함수
 * @param array 동적 배열 관리 구조체 포인터 변수의 주소를 가지는 포인터(더블 포인터, 입력, 읽기 전용)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayDelete(dynamicIntArray_t **array)
{
	if(dynamicIntArrayClear(*array) == FAIL)
	{
		return FAIL;
	}

	if(dynamicIntArrayFinal(*array) == FAIL)
	{
		return FAIL;
	}

	freeMemory((void**)(array));

	return SUCCESS;
}

/**
 * @fn int dynamicIntArraySetElement(const dynamicIntArray_t *array, int index, int datum)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 지정한 인덱스에 특정 값을 저장하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param index 지정할 인덱스(입력)
 * @param datum 저장할 특정 값(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArraySetElement(const dynamicIntArray_t *array, int index, int datum)
{
	if(dynamicIntArrayCheckBoundary(array, index) == FAIL)
	{
		printMsg("Set 실패. 인덱스 오류. (dynamicIntArraySetElement)", ERROR, 0);
		return FAIL;
	}

	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArraySetElement)") == YES)
	{
		return FAIL;
	}

	arrayData[index] = datum;

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayGetElement(const dynamicIntArray_t *array, int index, int *isError)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 지정한 인덱스에 저장된 값을 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param index 지정할 인덱스(입력)
 * @param isError 함수 실행 결과(출력)
 * @return 성공 시 지정한 인덱스에 저장된 값, 실패 시 FAIL 반환
 */
int dynamicIntArrayGetElement(const dynamicIntArray_t *array, int index, int *isError)
{
	if(dynamicIntArrayCheckBoundary(array, index) == FAIL)
	{
		printMsg("Get 실패. 인덱스 오류. (dynamicIntArrayGetElement)", ERROR, 0);
		*isError = FAIL;
		return NONE;
	}

	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayGetElement)") == YES)
	{
		*isError = FAIL;
		return NONE;
	}

	*isError = SUCCESS;
	return arrayData[index];
}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayAppend(dynamicIntArray_t *array, int datum)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 끝에 새로운 값을 추가하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @param datum 추가할 특정 값(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayAppend(dynamicIntArray_t *array, int datum)
{
	if(dynamicIntArrayIncSize(array) == FAIL)
	{
		printMsg("dynamicIntArrayIncSize 실패. (dynamicIntArrayAppend, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayAppend, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	array = dynamicIntArrayResize(array, YES);
	if(checkObjectNull(array->data, "메모리 재할당 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayAppend)") == YES)
	{
		return NULL;
	}

	if(dynamicIntArraySetElement(array, (size - 1), datum) == FAIL)
	{
		printMsg("배열 저장 오류. dynamicIntArraySetElement 동작 실패. (dynamicIntArrayAppend)", DEBUG, 0);
		return NULL;
	}

	return array;
}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayInsertAt(dynamicIntArray_t *array, int index, int datum)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 지정한 인덱스에 특정 값을 추가하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @param index 지정할 인덱스(입력)
 * @param datum 추가할 특정 값(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayInsertAt(dynamicIntArray_t *array, int index, int datum)
{
	if(dynamicIntArrayCheckBoundary(array, index) == FAIL)
	{
		printMsg("Insert at 실패. 인덱스 오류. (dynamicIntArrayInsertAt)", ERROR, 0);
		return NULL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayInsertAt, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	int tempArraySize = size - index;
	dynamicIntArray_t tempArray[1];

	if(dynamicIntArrayInitialize(tempArray, tempArraySize) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 초기화 실패. (dynamicIntArrayInsertAt)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArraySetSize(tempArray, tempArraySize) == FAIL)
	{
		printMsg("크기 지정 실패. (dynamicIntArrayNew, size:%d)", DEBUG, 1, tempArraySize);
		return NULL;
	}

	if (dynamicIntArrayCopy(tempArray, 0, array, index, tempArraySize) == NULL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayInsertAt, array -> tempArray)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArrayIncSize(array) == FAIL)
	{
		printMsg("dynamicIntArrayIncSize 실패. (dynamicIntArrayInsertAt, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayAppend, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	array = dynamicIntArrayResize(array, YES);
	if(checkObjectNull(array->data, "메모리 재할당 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayInsertAt)") == YES)
	{
		return NULL;
	}

	if (dynamicIntArrayCopy(array, (index + 1), tempArray, 0, tempArraySize) == NULL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayInsertAt, tempArray -> array)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArraySetElement(array, index, datum) == FAIL)
	{
		printMsg("배열 저장 오류. dynamicIntArraySetElement 동작 실패. (dynamicIntArrayInsertAt)", DEBUG, 0);
		return NULL;
	}

	dynamicIntArrayFinal(tempArray);
	return array;

}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayRemoveAt(dynamicIntArray_t *array, int index)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 지정한 인덱스에 저장된 값을 삭제하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @param index 지정할 인덱스(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayRemoveAt(dynamicIntArray_t *array, int index)
{
	if(dynamicIntArrayCheckBoundary(array, index) == FAIL)
	{
		printMsg("Remove at 실패. 인덱스 오류. (dynamicIntArrayRemoveAt)", ERROR, 0);
		return NULL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayRemoveAt, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	int tempArraySize = size - index;
	dynamicIntArray_t tempArray[1];

	if(dynamicIntArrayInitialize(tempArray, tempArraySize) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 초기화 실패. (dynamicIntArrayRemoveAt)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArraySetSize(tempArray, tempArraySize) == FAIL)
	{
		printMsg("크기 지정 실패. (dynamicIntArrayRemoveAt, size:%d)", DEBUG, 1, tempArraySize);
		return NULL;
	}

	if (dynamicIntArrayCopy(tempArray, 0, array, (index + 1), tempArraySize) == NULL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayRemoveAt, tempArray -> array)", DEBUG, 0);
		return NULL;
	}

	if (dynamicIntArrayCopy(array, index, tempArray, 0, tempArraySize) == NULL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayRemoveAt, array -> tempArray)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArrayDecSize(array) == FAIL)
	{
		printMsg("dynamicIntArrayDecSize 실패. (dynamicIntArrayRemoveAt, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayAppend, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	array = dynamicIntArrayResize(array, YES);
	if(checkObjectNull(array->data, "메모리 재할당 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayRemoveAt)") == YES)
	{
		return NULL;
	}

	dynamicIntArrayFinal(tempArray);
	return array;
}

/**
 * @fn int dynamicIntArrayIndexOf(const dynamicIntArray_t *array, int datum)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열에서 지정한 값의 첫 번째 인덱스를 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param datum 지정한 값(입력)
 * @return 성공 시 검색된 인덱스, 검색 실패 시 UNKNOWN, 내부 함수 호출 실패 시 FAIL 반환
 */
int dynamicIntArrayIndexOf(const dynamicIntArray_t *array, int datum)
{
	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayIndexOf, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	int target_index = UNKNOWN;
	int value = UNKNOWN;
	int loopIndex = 0;
	int isError[1] = { FAIL };

	for (; loopIndex < size; loopIndex++)
	{
		value = dynamicIntArrayGetElement(array, loopIndex, isError);
		if(*isError == FAIL) 
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayIndexOf)", ERROR, 0);
			return FAIL;
		}

		if (value == datum)
		{
			target_index = loopIndex;
			break;
		}
	}

	return target_index;
}

/**
 * @fn int dynamicIntArrayLastIndexOf(const dynamicIntArray_t *array, int datum)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열에서 지정한 값의 마지막 인덱스를 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param datum 지정한 값(입력)
 * @return 성공 시 검색된 인덱스, 검색 실패 시 UNKNOWN, 내부 함수 호출 실패 시 FAIL 반환
 */
int dynamicIntArrayLastIndexOf(const dynamicIntArray_t *array, int datum)
{
	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	int target_index = UNKNOWN;
	int value = UNKNOWN;
	int loopIndex = size - 1;
	int isError[1] = { FAIL };

	for (; loopIndex >= 0; loopIndex--)
	{
		value = dynamicIntArrayGetElement(array, loopIndex, isError);
		if(*isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayLastIndexOf)", ERROR, 0);
			return FAIL;
		}

		if (value == datum)
		{
			target_index = loopIndex;
			break;
		}
	}

	return target_index;
}

/**
 * @fn int dynamicIntArrayFind(const dynamicIntArray_t *array, compareInt1Param_f func)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열에서 지정한 조건 함수의 조건과 일치하는 값의 첫 번째 인덱스를 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param func 지정한 조건 함수(입력)
 * @return 성공 시 검색된 인덱스, 검색 실패 시 UNKNOWN, 내부 함수 호출 실패 시 FAIL 반환
 */
int dynamicIntArrayFind(const dynamicIntArray_t *array, compareInt1Param_f func)
{
	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayFind, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	int target_index = UNKNOWN;
	int value = UNKNOWN;
	int loopIndex = 0;
	int isError[1] = { FAIL };

	for (; loopIndex < size; loopIndex++)
	{
		value = dynamicIntArrayGetElement(array, loopIndex, isError);
		if(*isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayFind)", ERROR, 0);
			return FAIL;
		}

		if (func(value) == YES)
		{
			target_index = loopIndex;
			break;
		}
	}

	return target_index;
}

/**
 * @fn int dynamicIntArrayReverse(const dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 전체 원소들을 역순으로 저장하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayReverse(const dynamicIntArray_t *array)
{
	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayReverse, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	dynamicIntArray_t *tempArray;
	int loopIndex = 0;
	int reverseLoopIndex = size - 1;
	int tempArrayValue = UNKNOWN;
	int isError[1] = { FAIL };

	if ((tempArray = dynamicIntArrayClone(array)) == NULL)
	{
		printMsg("배열 복제 오류. dynamicIntArrayClone 동작 실패. (dynamicIntArrayReverse)", DEBUG, 0);
		return FAIL;
	}

	for (loopIndex = 0; loopIndex < size; loopIndex++)
	{
		tempArrayValue = dynamicIntArrayGetElement(tempArray, reverseLoopIndex, isError);
		if(*isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayReverse)", ERROR, 0);
			return FAIL;
		}

		if(dynamicIntArraySetElement(array, loopIndex, tempArrayValue) == FAIL)
		{
			printMsg("배열 저장 오류. dynamicIntArraySetElement 동작 실패. (dynamicIntArrayReverse)", DEBUG, 0);
			return FAIL;
		}

		reverseLoopIndex--;
	}

	if(dynamicIntArrayDelete(&tempArray) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 메모리 해제 실패. (dynamicIntArrayDelete)", DEBUG, 0);
	}
	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayFill(const dynamicIntArray_t *array, int datum)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열을 특정 값으로 채우는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param datum 채울 값(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayFill(const dynamicIntArray_t *array, int datum)
{
	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayFill)") == YES)
	{
		return FAIL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayFill, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	int loopIndex = 0;
	for( ; loopIndex < size; loopIndex++)
	{
		arrayData[loopIndex] = datum;
	}

	return SUCCESS;
}

/**
 * @fn int *dynamicIntArrayGetArrayPtr(const dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 주소를 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @return 성공 시 구조체 포인터가 가지고 있는 동적 배열의 주소, 실패 시 NULL 반환
 */
int *dynamicIntArrayGetArrayPtr(const dynamicIntArray_t *array)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayGetArrayPtr)") == YES)
	{
		return NULL;
	}

	return array->data;
}

/**
 * @fn int dynamicIntArrayGetSize(const dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 전체 크기를 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @return 성공 시 동적 배열의 전체 크기, 실패 시 UNKNOWN 반환
 */
int dynamicIntArrayGetSize(const dynamicIntArray_t *array)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayGetSize)") == YES)
	{
		return UNKNOWN;
	}

	return array->size;
}

/**
 * @fn int dynamicIntArraySetSize(dynamicIntArray_t *array, int size)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 전체 크기를 특정 값으로 저장하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @param size 저장할 동적 배열의 전체 크기(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArraySetSize(dynamicIntArray_t *array, int size)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArraySetSize)") == YES)
	{
		return FAIL;
	}

	if(size <= 0)
	{
		printMsg("크기 지정 실패. 크기가 0 보다 작거나 같음. (dynamicIntArraySetSize, size:%d)", ERROR, 1, size);
		return FAIL;
	}

	array->size = size;

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayIncSize(dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 전체 크기를 1 증가시키는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayIncSize(dynamicIntArray_t *array)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayIncSize)") == YES)
	{
		return FAIL;
	}

	++(array->size);

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayDecSize(dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 전체 크기를 1 감소시키는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayDecSize(dynamicIntArray_t *array)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayDecSize)") == YES)
	{
		return FAIL;
	}

	--(array->size);

	return SUCCESS;
}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayCopy(dynamicIntArray_t *dst, int dstIndex, dynamicIntArray_t *src, int srcIndex, int size)
 * @brief 하나의 동적 배열 관리 구조체의 동적 배열을 다른 동적 배열 관리 구조체의 동적 배열로 복사하는 함수
 * @param dst 복사될 동적 배열 관리 구조체 포인터(출력)
 * @param dstIndex 복사될 배열의 시작 인덱스(입력)
 * @param src 복사할 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param srcIndex 복사할 배열의 시작 인덱스(입력)
 * @param size 복사할 크기(입력)
 * @return 성공 시 복사된 동적 배열 관리 구조체의 주소, 메모리 참조 실패 또는 복사할 크기가 0 이하면 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayCopy(dynamicIntArray_t *dst, int dstIndex, const dynamicIntArray_t *src, int srcIndex, int size)
{
	if(size <= 0)
	{
		printMsg("복사 실패. 복사할 크기가 0. (dynamicIntArrayCopy, size:%d)", DEBUG, 1, size);
		return NULL;
	}

	if(dynamicIntArrayCheckBoundary(dst, dstIndex) == FAIL)
	{
		printMsg("복사 실패. destination 인덱스 오류. (dynamicIntArrayCopy)", ERROR, 0);
		return NULL;
	}

	if(dynamicIntArrayCheckBoundary(src, srcIndex) == FAIL)
	{
		printMsg("복사 실패. source 인덱스 오류. (dynamicIntArrayCopy)", ERROR, 0);
		return NULL;
	}

	int srcSize = dynamicIntArrayGetSize(src);
	if(srcSize == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayCopy, array:%p)", DEBUG, 1, src);
		return NULL;
	}

	if(size > srcSize)
	{
		printMsg("복사하려는 크기가 배열의 크기보다 큼. (dynamicIntArrayCopy, size:%d, srcSize:%d)", DEBUG, 2, size, srcSize);
		return NULL;
	}

	int dstSize = dynamicIntArrayGetSize(dst);
	if(dstSize == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayCopy, array:%p)", DEBUG, 1, dst);
		return NULL;
	}

	if(size > dstSize)
	{
		printMsg("복사하려는 크기가 배열의 크기보다 큼. (dynamicIntArrayCopy, size:%d, dstSize:%d)", DEBUG, 2, size, dstSize);
		return NULL;
	}

	int *dstArrayData = dynamicIntArrayGetArrayPtr(dst);
	if(checkObjectNull(dstArrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayCopy)") == YES)
	{
		return NULL;
	}

	int *srcArrayData = dynamicIntArrayGetArrayPtr(src);
	if(checkObjectNull(srcArrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayCopy)") == YES)
	{
		return NULL;
	}

	dstArrayData += dstIndex;
	srcArrayData += srcIndex;

	while (size--)
	{
		*dstArrayData++ = *srcArrayData++;
	}

	return dst;
}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayClone(const dynamicIntArray_t *original)
 * @brief 하나의 동적 배열 관리 구조체를 새로운 동적 배열 관리 구조체에 복제하는 함수
 * @param original 복제할 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @return 성공 시 새로 복제된 동적 배열 관리 구조체, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayClone(const dynamicIntArray_t *original)
{
	int size = dynamicIntArrayGetSize(original);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayClone, array:%p)", DEBUG, 1, original);
		return NULL;
	}

	dynamicIntArray_t *new = dynamicIntArrayNew(size);
	if(checkObjectNull(new, "메모리 할당 실패, 새 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayClone)") == YES)
	{
		return NULL;
	}

	if (dynamicIntArrayCopy(new, 0, original, 0, size) == NULL)
	{
		printMsg("구조체 복제 실패. (dynamicIntArrayClone)", DEBUG, 0);
		return NULL;
	}

	return new;
}

/**
 * @fn int dynamicIntArrayCheckBoundary(const dynamicIntArray_t *array, int index)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 바운더리에 지정한 인덱스가 허용되는지 검사하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param index 검사할 인덱스(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayCheckBoundary(const dynamicIntArray_t *array, int index)
{
	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayCheckBoundary)") == YES)
	{
		return FAIL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayCheckBoundary, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	if ((index < 0) || (index >= size))
	{
		printMsg("동적 배열의 인덱스 바운더리 오류. (dynamicIntArrayCheckBoundary, array->data:%p, index:%d, range:0~%d)", DEBUG, 3, arrayData, index, size);
		return FAIL;
	}

	return SUCCESS;
}

/**
 * @fn char *dynamicIntArrayToString(dynamicIntArray_t *array)
 * @brief 동적 배열의 모든 원소를 담고 있는 문자열을 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 생성된 문자열, 실패 시 NULL 반환
 */
char *dynamicIntArrayToString(dynamicIntArray_t *array)
{
	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayToString)") == YES)
	{
		return NULL;
	}

	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayToString, array:%p)", DEBUG, 1, array);
		return NULL;
	}

	int sumOfDigits = 0;
	int digitOfSize = 0;
	int loopIndex = 0;
	int stringLength = 0;
	char *string = array->stringOfArray;

	// 1. 동적 배열에 담긴 숫자들을 문자열로 변환하기 위해서 자리수를 모두 더한 값을 구한다.
	for( ; loopIndex < size; loopIndex++)
	{
		sumOfDigits += getDigitOfNumber(arrayData[loopIndex]);
	}
	// 맨 뒤에 배열 전체 크기도 추가해야하므로 크기값의 자리수도 구한다.
	digitOfSize = getDigitOfNumber(size);

	// 생성할 문자열의 총 길이를 계산한다.
	// [총 길이] = [모든 원소들의 자리수 합] + [동적 배열의 전체 크기의 자리수] + [동적 배열의 전체 크기(size) * 3]
	// [size * 3] : [쉼표 개수] + [공백 개수] + [추가적인 여유 공간]
	stringLength = sumOfDigits + digitOfSize + (size * 3);

	// 2. 동적 배열 관리 구조체 최초 할당 시 문자열이 할당되지 않으므로 
	// 함수 최초 호출 시 할당하도록 한다.
	if(checkObjectNull(string, NULL) == YES)
	{
		string = (char*)allocateMemory(stringLength);
		if(checkObjectNull(string, "메모리 참조 실패, 새로 할당된 문자열이 NULL. (dynamicIntArrayToString)") == YES)
		{
			return NULL;
		}
	}
	// 이전에 한 번 이상 호출되었다면, 내용을 모두 지우고 새로운 크기로 재할당한다.
	else
	{
		clearMemory(string, strlen(string));
		string = (char*)reallocateMemory(string, stringLength);
		if(checkObjectNull(string, "메모리 참조 실패, 재할당된 문자열이 NULL. (dynamicIntArrayToString)") == YES)
		{
			return NULL;
		}
	}

	// 3. 할당된 문자열에 동적 배열에 저장된 값을 하나씩 문자열로 변환해서 추가한다.
	loopIndex = 0;
	string[loopIndex] = '{';
	for (; loopIndex < size; loopIndex++)
	{
		addNumberToString(string, arrayData[loopIndex]);
		if ((loopIndex + 1) < size) strcat(string, ",");
	}
	strcat(string, " },");

	// 동적 배열의 전체 크기도 문자열로 변환해서 추가한다.
	addNumberToString(string, size);

	// 4. 문자열의 쓰레기값 출력 방지하기 위해 문자열의 맨 끝에 널 문자 추가한다.
	string[strlen(string)] = '\0';

	// 5. 사용된 주소값을 다시 구조체의 문자열 포인터 변수에 저장한다.
	array->stringOfArray = string;
	return string;
}

///////////////////////////////////////////////////////////////////////////////////////
/// Common Util Functions
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn void printMsg(const char *msg, int type, int argc, ...)
 * @brief 메시지를 출력하는 함수
 * @param msg 출력할 문자열(입력, 읽기 전용)
 * @param type 출력 형태(입력, PRINT_TYPE 열거형 참고)
 * @param argc 가변 인자 개수(입력)
 * @return 반환값 없음
 */
void printMsg(const char *msg, int type, int argc, ...)
{
	va_list argPointer;
	va_start(argPointer, argc);

	int size = getBufferSize(msg, argPointer);
	if(size == FAIL)
	{
		printf("가변 인자를 포함한 문자열의 길이 계산 실패. (printMsg)\n");
		return;
	}

	char printBuffer[size];
	clearMemory(printBuffer, size);

	int isErrorForVSN = vsnprintf(printBuffer, size, msg, argPointer);
	if(isErrorForVSN <= 0 // for glibc < 2.1
			|| isErrorForVSN >= sizeof(printBuffer)) // for glibc >= 2.1
	{
#if IS_PRINT_DEBUG
		printf("출력 실패. vsnprintf 함수 동작 오류. (printMsg)\n");
#endif
		return;
	}

	if (type == NORMAL)
	{
		printf("%s\n", printBuffer);
	}
#if IS_PRINT_DEBUG
	else if (type == DEBUG)
	{
		printf("%s\n", printBuffer);
	}
#endif
#if IS_PRINT_ERROR
	else if (type == ERROR)
	{
		printf("%s\n", printBuffer);
	}
#endif

	va_end(argPointer);
}

/**
 * @fn int checkObjectNull(const void *object, const char *msg)
 * @brief 객체가 NULL 인지 검사하는 함수
 * @param object 검사할 객체(입력, 읽기 전용)
 * @param msg 출력할 오류 문자열(입력, 읽기 전용)
 * @return NULL 이 아니면 NO, NULL 이면 YES 반환
 */
int checkObjectNull(const void *object, const char *msg)
{
	if (object == NULL)
	{
		if(msg != NULL) printMsg(msg, DEBUG, 0);
		return YES;
	}
	return NO;
}

//////////////////////////////////////////////////////////////////////////////////////
/// Static Util Functions
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static void *reallocateMemory(void *memory, size_t size)
 * @brief 메모리를 재할당하는 함수
 * @param memory 재할당할 주소를 저장할 포인터(입력)
 * @param size 재할당할 크기(입력)
 * @return 재할당된 주소를 가지는 포인터
 */
static void *reallocateMemory(void *memory, size_t size)
{
	if(checkObjectNull(memory, "메모리 참조 실패, 재할당될 메모리의 포인터가 NULL. (reallocateMemory)") == YES)
	{
		printMsg("memory:%p", DEBUG, 1, memory);
		return NULL;
	}

	void *newMemory = memory;
	memory = realloc(memory, size);

	if(checkObjectNull(memory, "메모리 참조 실패, 재할당된 메모리의 포인터가 NULL. (reallocateMemory)") == YES)
	{
		printMsg("memory:%p, newMemory:%p", DEBUG, 1, memory, newMemory);
		free(newMemory);
		memory = newMemory;
	}

	return memory;
}

/**
 * @fn static void *allocateMemory(size_t size)
 * @brief 메모리를 새로 할당하는 함수
 * @param size 할당할 크기(입력)
 * @return 할당된 주소를 가지는 포인터
 */
static void *allocateMemory(size_t size)
{
	void *memory = malloc(size);

	if(checkObjectNull(memory, "메모리 참조 실패, 새로 할당된 포인터가 NULL. (allocateMemory)") == YES)
	{
		return NULL;
	}

	return memory;
}

/**
 * @fn static void clearMemory(void *memory, size_t size)
 * @brief 메모리를 0 으로 초기화하는 함수
 * @param memory 초기화할 주소를 가지는 포인터(입력)
 * @param size 초기화할 실제 객체 크기(입력)
 * @return 반환값 없음
 */
static void clearMemory(void *memory, size_t size)
{
	if(checkObjectNull(memory, "메모리 참조 실패, 전달받은 메모리 포인터가 NULL. (clearMemory)") == NO)
	{
		memset(memory, 0, size);
	}
}

/**
 * @fn static void freeMemory(void **memory)
 * @brief 메모리를 해제하는 함수
 * @param memory 해제할 주소를 가지는 포인터 변수의 주소를 가지는 포인터(더블 포인터, 입력)
 * @return 반환값 없음
 */
static void freeMemory(void **memory)
{
	if(checkObjectNull(*memory, "메모리 참조 실패, 전달받은 메모리 포인터가 NULL. (freeMemory)") == NO)
	{
		free(*memory);
		*memory = NULL;
	}
}

/**
 * @fn static int getBufferSize(const char *format, ...)
 * @brief 가변 인자가 포함된 문자열의 총 길이를 계산하여 반환하는 함수
 * @param msg 가변 인자가 포함된 문자열(입력, 읽기 전용) 
 * @return 성공 시 문자열의 총 길이, 실패 시 FAIL 반환
 */
static int getBufferSize(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	int result = vsnprintf(NULL, 0, msg, args);
	if(result <= 0) // for glibc < 2.1
	{
		return FAIL;
	}

	va_end(args);

	return (result + 1); // [+1] : for NULL character
}

/**
 * @fn __inline static int getDigitOfNumber(int number)
 * @brief 정수의 자리수를 계산해서 반환하는 함수
 * @param number 자리수를 계산할 정수
 * @return 항상 정수의 자리수 반환
 */
__inline static int getDigitOfNumber(int number)
{
	int countOfDigit = 0;

	while(number != 0)
	{
		countOfDigit++;
		number /= 10;
	}

	return countOfDigit;
}

/**
 * @fn __inline static void addNumberToString(char *string, int number)
 * @brief 정수를 문자열로 변환해서 전달받은 문자열에 추가하는 함수
 * @param string 추가될 문자열(출력)
 * @param number 추가할 정수(입력)
 * @return 반환값 없음
 */
__inline static void addNumberToString(char *string, int number)
{
	int digitOfNumber = getDigitOfNumber(number);
	char buf[digitOfNumber + 1];
	sprintf(buf, " %d", number);
	strcat(string, buf);
}

