#include "dynamicIntArray.h"

///////////////////////////////////////////////////////////////////////////////////////
/// Predefinitions of Static Util Functions
///////////////////////////////////////////////////////////////////////////////////////

static int getBufferSize(const char *format, ...);
static int getDigitOfNumber(int number);
static int addNumberToString(char *string, int number);

///////////////////////////////////////////////////////////////////////////////////////
/// Functions for dynamicIntArray_t
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn dynamicIntArray_t *dynamicIntArrayNew(int size)
 * @brief 동적 배열 관리 구조체를 새로 생성하는 함수
 * @param size 동적 배열을 생성할 크기(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayNew(int size)
{
	dynamicIntArray_t *array = (dynamicIntArray_t*)malloc(sizeof(dynamicIntArray_t));
	if(dynamicIntArrayInitialize(array, size) == FAIL)
	{
		printMsg("초기화 실패, 생성한 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayNew)", DEBUG, 0);
		free(array);
		return NULL;
	}

	return array;
}

/**
 * @fn int dynamicIntArrayInitialize(dynamicIntArrya_t *array, int size)
 * @brief 동적 배열 관리 구조체의 내부(멤버 변수)를 초기화하는 함수
 * @param array 동적 배열 관리 구조체 포인터(출력)
 * @param size 동적 배열 크기(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayInitialize(dynamicIntArray_t *array, int size)
{
	if(checkObjectNull(array, "메모리 참조 실패, 생성한 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayInitialize)") == YES)
	{
		return FAIL;
	}

	if(size <= 0)
	{
		printMsg("크기 지정 실패. 크기가 0 보다 작거나 같음. (dynamicIntArrayInitialize, size:%d)", ERROR, 1, size);
		return FAIL;
	}

	array->size = size;
	array->stringOfArray = NULL;

	size_t totalSize = (size_t)size * sizeof(int);
	array->data = (int*)calloc(totalSize, sizeof(int));
	if(checkObjectNull(array->data, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayInitialize)") == YES)
	{
		return FAIL;
	}

	return SUCCESS;
}

/**
 * @fn dynamicIntArray_t *dynamicIntArrayResize(dynamicIntArray_t *array, int size, int isKeep)
 * @brief 동적 배열 관리 구조체가 관리하고 있는 동적 배열을 재생성하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @param size  변경할 크기(입력)
 * @param isKeep 재생성 시 기존 데이터 유지 여부(입력)
 * @return 성공 시 동적 배열 관리 구조체의 주소, 실패 시 NULL 반환
 */
dynamicIntArray_t *dynamicIntArrayResize(dynamicIntArray_t *array, int size, int isKeep)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체가 NULL. (dynamicIntArrayResize)") == YES)
	{
		return NULL;
	}

	if(size <= 0)
	{
		printMsg("동적 배열의 크기가 0 보다 작거나 같음. (dynamicIntArrayResize, size:%d)", ERROR, 1, size);
		return NULL;
	}

	if((isKeep != YES) && (isKeep != NO))
	{
		printMsg("알 수 없는 isKeep 변수 값 사용. 재생성 취소. (dynamicIntArrayResize, isKeep:%d)", ERROR, 1, isKeep);
		return NULL;
	}

	int *arrayData = NULL;
	size_t totalSize = (size_t)size * sizeof(int);

	if(isKeep == YES)
	{
		arrayData = (int*)realloc(array->data, totalSize);
		if(checkObjectNull(arrayData, "메모리 참조 실패, 재생성한 동적 배열이 NULL. (dynamicIntArrayResize)") == YES)
		{
			return NULL;
		}
	}
	else if(isKeep == NO)
	{
		arrayData = (int*)calloc(totalSize, sizeof(int));
		if(checkObjectNull(arrayData, "메모리 참조 실패, 새로 생성한 동적 배열이 NULL. (dynamicIntArrayResize)") == YES)
		{
			return NULL;
		}
		free(array->data);
	}

	array->data = arrayData;
	return array;
}

/**
 * @fn int dynamicIntArrayClear(dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체의 동적 배열 관련 멤버 변수들을 모두 0 으로 설정하는 함수
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

	int size = array->size;
	if(size <= 0)
	{
		printMsg("동적 배열의 크기가 0 보다 작거나 같음. (dynamicIntArrayClear, size:%d)", DEBUG, 1, size);
		return FAIL;
	}

	size_t totalSize = (size_t)size * sizeof(int);
	memset(arrayData, 0, totalSize);
	array->size = 0;

	if(checkObjectNull(array->stringOfArray, NULL) == NO)
	{
		memset(array->stringOfArray, 0, strlen(array->stringOfArray));
	}

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayFinal(dynamicIntArray_t *array)
 * @brief 동적 배열 관리 구조체의 멤버 변수의 동적으로 생성한 메모리는 해제하고 정적 변수는 0 으로 설정하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayFinal(dynamicIntArray_t *array)
{
	if(checkObjectNull(array, "메모리 참조 실패, 동적 배열 관리 구조체가 NULL. (dynamicIntArrayFinal)") == YES)
	{
		return FAIL;
	}

	if(checkObjectNull(array->data, NULL) == NO)
	{
		free(array->data);
		array->data = NULL;
	}

	if(checkObjectNull(array->stringOfArray, NULL) == NO)
	{
		free(array->stringOfArray);
		array->stringOfArray = NULL;
	}

	array->size = 0;

	return SUCCESS;
}

/**
 * @fn void dynamicIntArrayDelete(dynamicIntArray_t **array)
 * @brief 동적 배열 관리 구조체의 생성한 메모리를 해제하는 함수
 * @param array 동적 배열 관리 구조체 포인터 변수의 주소를 가지는 포인터(더블 포인터, 입력, 읽기 전용)
 * @return 반환값 없음
 */
void dynamicIntArrayDelete(dynamicIntArray_t **array)
{
	if(dynamicIntArrayFinal(*array) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 메모리 해제 실패. dynamicIntArrayFinal 실패. (dynamicIntArrayDelete, array:%p)", DEBUG, 1, *array);
		return;
	}

	free(*array);
	*array = NULL;
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

	if(checkObjectNull(array->data, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayGetElement)") == YES)
	{
		return FAIL;
	}

	array->data[index] = datum;

	return SUCCESS;
}

/**
 * @fn int dynamicIntArrayGetElement(const dynamicIntArray_t *array, int index, int *isError)
 * @brief 동적 배열 관리 구조체가 관리하는 동적 배열의 지정한 인덱스에 저장된 값을 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param index 지정할 인덱스(입력)
 * @param isError 함수 실행 결과(출력, 함수 실행 성공 시 SUCCESS, 실패 시 FAIL 저장)
 * @return 성공 시 지정한 인덱스에 저장된 값, 실패 시 NONE 반환
 */
int dynamicIntArrayGetElement(const dynamicIntArray_t *array, int index, int *isError)
{
	if(dynamicIntArrayCheckBoundary(array, index) == FAIL)
	{
		printMsg("Get 실패. 인덱스 오류. (dynamicIntArrayGetElement)", ERROR, 0);
		*isError = FAIL;
		return NONE;
	}

	if(checkObjectNull(array->data, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayGetElement)") == YES)
	{
		*isError = FAIL;
		return NONE;
	}

	*isError = SUCCESS;
	return array->data[index];
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
	array = dynamicIntArrayResize(array, ++(array->size), YES);
	if(checkObjectNull(array, "메모리 재생성 실패, 동적 배열 관리 구조체가 NULL. (dynamicIntArrayAppend)") == YES)
	{
		--(array->size);
		return NULL;
	}

	if(dynamicIntArraySetElement(array, (array->size - 1), datum) == FAIL)
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

	int size = array->size;
	int tempArraySize = size - index;
	dynamicIntArray_t tempArray;

	if(dynamicIntArrayInitialize(&tempArray, tempArraySize) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 초기화 실패. (dynamicIntArrayInsertAt)", DEBUG, 0);
		return NULL;
	}

	if (dynamicIntArrayCopy(&tempArray, 0, array, index, tempArraySize) == FAIL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayInsertAt, array -> tempArray)", DEBUG, 0);
		return NULL;
	}

	array = dynamicIntArrayResize(array, ++(array->size), YES);
	if(checkObjectNull(array, "메모리 재생성 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayInsertAt)") == YES)
	{
		--(array->size);
		return NULL;
	}

	if (dynamicIntArrayCopy(array, (index + 1), &tempArray, 0, tempArraySize) == FAIL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayInsertAt, tempArray -> array)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArraySetElement(array, index, datum) == FAIL)
	{
		printMsg("배열 저장 오류. dynamicIntArraySetElement 동작 실패. (dynamicIntArrayInsertAt)", DEBUG, 0);
		return NULL;
	}

	if(dynamicIntArrayFinal(&tempArray) == FAIL)
	{
		printMsg("tempArray 내부 동적 배열 메모리 해제 실패. dynamicIntArrayFinal 실패. (dynamicIntArrayInsertAt)", DEBUG, 0);
		return NULL;
	}

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

	int size = array->size;
	int positionOfCopy = index + 1;
	int tempArraySize = size - positionOfCopy;
	dynamicIntArray_t tempArray;

	if(dynamicIntArrayInitialize(&tempArray, tempArraySize) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 초기화 실패. (dynamicIntArrayRemoveAt)", DEBUG, 0);
		return NULL;
	}

	if (dynamicIntArrayCopy(&tempArray, 0, array, positionOfCopy, tempArraySize) == FAIL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayRemoveAt, tempArray -> array)", DEBUG, 0);
		return NULL;
	}

	array = dynamicIntArrayResize(array, --(array->size), YES);
	if(checkObjectNull(array, "메모리 재생성 실패, 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayRemoveAt)") == YES)
	{
		++(array->size);
		return NULL;
	}

	if (dynamicIntArrayCopy(array, index, &tempArray, 0, tempArraySize) == FAIL)
	{
		printMsg("배열 복사 오류. dynamicIntArrayCopy 동작 실패. (dynamicIntArrayRemoveAt, array -> tempArray)", DEBUG, 0);
		return NULL;
	}


	if(dynamicIntArrayFinal(&tempArray) == FAIL)
	{
		printMsg("tempArray 내부 동적 배열 메모리 해제 실패. dynamicIntArrayFinal 실패. (dynamicIntArrayRemoveAt)", DEBUG, 0);
		return NULL;
	}

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

	int targetIndex = UNKNOWN;
	int value = UNKNOWN;
	int loopIndex = 0;
	int isError = FAIL;

	for (; loopIndex < size; loopIndex++)
	{
		value = dynamicIntArrayGetElement(array, loopIndex, &isError);
		if(isError == FAIL) 
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayIndexOf)", ERROR, 0);
			return FAIL;
		}

		if (value == datum)
		{
			targetIndex = loopIndex;
			break;
		}
	}

	return targetIndex;
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
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayLastIndexOf, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	int targetIndex = UNKNOWN;
	int value = UNKNOWN;
	int loopIndex = size - 1;
	int isError = FAIL;

	for (; loopIndex >= 0; loopIndex--)
	{
		value = dynamicIntArrayGetElement(array, loopIndex, &isError);
		if(isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayLastIndexOf)", ERROR, 0);
			return FAIL;
		}

		if (value == datum)
		{
			targetIndex = loopIndex;
			break;
		}
	}

	return targetIndex;
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

	int targetIndex = UNKNOWN;
	int value = UNKNOWN;
	int loopIndex = 0;
	int isError = FAIL;

	for (; loopIndex < size; loopIndex++)
	{
		value = dynamicIntArrayGetElement(array, loopIndex, &isError);
		if(isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayFind)", ERROR, 0);
			return FAIL;
		}

		if (func(value) == YES)
		{
			targetIndex = loopIndex;
			break;
		}
	}

	return targetIndex;
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

	int isError = FAIL;
	int loopIndex = 0;
	int rightValue = UNKNOWN;
	int leftValue = UNKNOWN;

	for( ; loopIndex < (size / 2); loopIndex++)
	{
		if((size - (loopIndex + 1)) <= loopIndex) break;

		rightValue = dynamicIntArrayGetElement(array, (size - (loopIndex + 1)), &isError);
		if(isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayReverse)", ERROR, 0);
			return FAIL;
		}

		leftValue = dynamicIntArrayGetElement(array, loopIndex, &isError);
		if(isError == FAIL)
		{
			printMsg("동적 배열 원소 조회 실패. dynamicIntArrayGetElement 실패. (dynamicIntArrayReverse)", ERROR, 0);
			return FAIL;
		}

		if(dynamicIntArraySetElement(array, (size - (loopIndex + 1)), leftValue) == FAIL)
		{
			printMsg("배열 저장 오류. dynamicIntArraySetElement 동작 실패. (dynamicIntArrayReverse)", DEBUG, 0);
			return FAIL;
		}

		if(dynamicIntArraySetElement(array, loopIndex, rightValue) == FAIL)
		{
			printMsg("배열 저장 오류. dynamicIntArraySetElement 동작 실패. (dynamicIntArrayReverse)", DEBUG, 0);
			return FAIL;
		}
	}

/*	int reverseLoopIndex = size - 1;
	dynamicIntArray_t *tempArray;

	if ((tempArray = dynamicIntArrayClone(array)) == NULL)
	{
		printMsg("배열 복제 오류. dynamicIntArrayClone 동작 실패. (dynamicIntArrayReverse)", DEBUG, 0);
		return FAIL;
	}

	for (loopIndex = 0; loopIndex < size; loopIndex++)
	{
		tempArrayValue = dynamicIntArrayGetElement(tempArray, reverseLoopIndex, &isError);
		if(isError == FAIL)
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

	dynamicIntArrayDelete(&tempArray);
*/
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

	int size = array->size;
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
 * @fn int dynamicIntArrayCopy(dynamicIntArray_t *dst, int dstIndex, dynamicIntArray_t *src, int srcIndex, int size)
 * @brief 하나의 동적 배열 관리 구조체의 동적 배열을 다른 동적 배열 관리 구조체의 동적 배열로 복사하는 함수
 * @param dst 복사될 동적 배열 관리 구조체 포인터(출력, 읽기 전용)
 * @param dstIndex 복사될 배열의 시작 인덱스(입력)
 * @param src 복사할 동적 배열 관리 구조체 포인터(입력, 읽기 전용)
 * @param srcIndex 복사할 배열의 시작 인덱스(입력)
 * @param size 복사할 크기(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int dynamicIntArrayCopy(const dynamicIntArray_t *dst, int dstIndex, const dynamicIntArray_t *src, int srcIndex, int size)
{
	if(size <= 0)
	{
		printMsg("복사 실패. 복사할 크기가 0. (dynamicIntArrayCopy, size:%d)", DEBUG, 1, size);
		return FAIL;
	}

	if(dynamicIntArrayCheckBoundary(dst, dstIndex) == FAIL)
	{
		printMsg("복사 실패. destination 인덱스 오류. (dynamicIntArrayCopy)", ERROR, 0);
		return FAIL;
	}

	if(dynamicIntArrayCheckBoundary(src, srcIndex) == FAIL)
	{
		printMsg("복사 실패. source 인덱스 오류. (dynamicIntArrayCopy)", ERROR, 0);
		return FAIL;
	}

	int srcSize = src->size;
	if(size > (srcSize - srcIndex))
	{
		printMsg("복사하려는 크기가 지정한 인덱스부터 배열의 마지막까지의 크기보다 큼. (dynamicIntArrayCopy, size:%d, srcSize:%d, srcIndex:%d)", DEBUG, 3, size, srcSize, srcIndex);
		return FAIL;
	}

	int dstSize = dst->size;
	if(size > (dstSize - dstIndex))
	{
		printMsg("복사하려는 크기가 지정한 인덱스부터 배열의 마지막까지의 크기보다 큼. (dynamicIntArrayCopy, size:%d, dstSize:%d, dstIndex:%d)", DEBUG, 3, size, dstSize, dstIndex);
		return FAIL;
	}

	int *dstArrayData = dst->data;
	int *srcArrayData = src->data;

	dstArrayData += dstIndex;
	srcArrayData += srcIndex;

	while (size--)
	{
		*dstArrayData++ = *srcArrayData++;
	}

	return SUCCESS;
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
	if(checkObjectNull(new, "메모리 생성 실패, 새 동적 배열 관리 구조체 포인터가 NULL. (dynamicIntArrayClone)") == YES)
	{
		return NULL;
	}

	if (dynamicIntArrayCopy(new, 0, original, 0, size) == FAIL)
	{
		printMsg("동적 배열 관리 구조체 복사 실패. (dynamicIntArrayClone, new:%p, original:%p, size:%d)", DEBUG, 3, new, original, size);
		dynamicIntArrayDelete(&new);
		return NULL;
	}

	if(checkObjectNull(original->stringOfArray, NULL) == NO)
	{
		size_t arrayLength = strlen(original->stringOfArray);
		new->stringOfArray = (char*)calloc(arrayLength, sizeof(char));
		if(checkObjectNull(new->stringOfArray, "메모리 생성 실패, 새로 생성한 문자열이 NULL. (dynamicIntArrayClone)") == YES)
		{
			dynamicIntArrayDelete(&new);
			return NULL;
		}
		strncpy(new->stringOfArray, original->stringOfArray, arrayLength);
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
	int size = dynamicIntArrayGetSize(array);
	if(size == UNKNOWN)
	{
		printMsg("dynamicIntArrayGetSize 실패. (dynamicIntArrayCheckBoundary, array:%p)", DEBUG, 1, array);
		return FAIL;
	}

	if ((index < 0) || (index >= size))
	{
		printMsg("동적 배열의 인덱스 바운더리 오류. (dynamicIntArrayCheckBoundary, index:%d, range:0~%d)", DEBUG, 2, index, size);
		return FAIL;
	}

	return SUCCESS;
}

/**
 * @fn char *dynamicIntArrayToString(dynamicIntArray_t *array)
 * @brief 동적 배열의 모든 원소를 담고 있는 문자열을 반환하는 함수
 * @param array 동적 배열 관리 구조체 포인터(입력)
 * @return 성공 시 생성한 문자열, 실패 시 NULL 반환
 */
char *dynamicIntArrayToString(dynamicIntArray_t *array)
{
	int *arrayData = dynamicIntArrayGetArrayPtr(array);
	if(checkObjectNull(arrayData, "메모리 참조 실패, 동적 배열이 NULL. (dynamicIntArrayToString)") == YES)
	{
		return NULL;
	}

	int size = array->size;
	int sumOfDigits = 0;
	int loopIndex = 0;
	int stringLength = 0;
	char *string = NULL;

	// 1. 동적 배열에 담긴 숫자들을 문자열로 변환하기 위해서 자리수를 모두 더한 값을 구한다.
	for( ; loopIndex < size; loopIndex++)
	{
		sumOfDigits += getDigitOfNumber(arrayData[loopIndex]);
	}

	// 생성할 문자열의 총 길이를 계산한다.
	// [총 길이] = [모든 원소들의 자리수 합] + [동적 배열의 전체 크기(size) * 2 + 3]
	// [size * 2 + 2] : [쉼표 개수(size 보다 1개 적다. size - 1)] + [공백 개수(size 만큼 있고 앞에 하나 더 있다. size + 1)] + [중괄호 개수(2)] + [널 문자(1)]
	stringLength = sumOfDigits + (size * 2 + 3);

	// 2. 동적 배열 관리 구조체 최초 생성 시 문자열이 생성되지 않으므로 
	// 함수 최초 호출 시 생성하도록 한다.
	if(checkObjectNull(array->stringOfArray, NULL) == YES)
	{
		string = (char*)calloc((size_t)stringLength, sizeof(char));
		if(checkObjectNull(string, "메모리 참조 실패, 새로 생성한 문자열이 NULL. (dynamicIntArrayToString)") == YES)
		{
			return NULL;
		}
	}
	// 이전에 한 번 이상 호출되었다면, 새로운 크기로 재생성하고 내용을 모두 지운다.
	else
	{
		string = (char*)realloc(array->stringOfArray, (size_t)stringLength);
		if(checkObjectNull(string, "메모리 참조 실패, 재생성한 문자열이 NULL. (dynamicIntArrayToString)") == YES)
		{
			return NULL;
		}
		memset(string, 0, strlen(string));
	}

	// 3. 생성한 문자열에 동적 배열에 저장된 값을 하나씩 문자열로 변환해서 추가한다.
	loopIndex = 0;
	string[loopIndex] = '{';
	for (; loopIndex < size; loopIndex++)
	{
		if(addNumberToString(string, arrayData[loopIndex]) == FAIL)
		{
			return NULL;
		}
		if ((loopIndex + 1) < size) strcat(string, ",");
	}
	strcat(string, " }");

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
	if((type < UNKNOWN) && (type > YES))
	{
		printf("[DEBUG] 출력 실패. 알 수 없는 메시지 타입. (printMsg, type:%d)\n", type);
		return;
	}

	if(argc < 0)
	{
		printf("[DEBUG] 출력 실패. 출력문 매개변수 개수가 음수. (printMsg, argc:%d)\n", argc);
		return;
	}

	va_list argPointer;
	va_start(argPointer, argc);

	int size = getBufferSize(msg, argPointer);
	if(size == FAIL)
	{
		printf("[ERROR] 가변 인자를 포함한 문자열의 길이 계산 실패. (printMsg)\n");
		va_end(argPointer);
		return;
	}

	char printBuffer[size];
	memset(printBuffer, 0, sizeof(printBuffer));

	int isErrorForVSN = vsnprintf(printBuffer, sizeof(printBuffer), msg, argPointer);
	if(isErrorForVSN < 0 // for glibc < 2.1
			|| (size_t)isErrorForVSN >= sizeof(printBuffer)) // for glibc >= 2.1
	{
#if IS_PRINT_DEBUG
		printf("[DEBUG] 출력 실패. vsnprintf 함수 동작 오류.\n");
		printf("(printMsg, returnValue:[%d], printBuffer:[%s], size:[%d], msg:[%s], argc:[%d], type:[%d])\n", isErrorForVSN, printBuffer, size, msg, argc, type);
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
		printf("[DEBUG] %s\n", printBuffer);
	}
#endif
#if IS_PRINT_ERROR
	else if (type == ERROR)
	{
		printf("[ERROR] %s\n", printBuffer);
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
 * @fn static int getBufferSize(const char *format, ...)
 * @brief 가변 인자가 포함된 문자열의 총 길이를 계산하여 반환하는 함수
 * @param msg 가변 인자가 포함된 문자열(입력, 읽기 전용) 
 * @return 성공 시 문자열의 총 길이, 실패 시 FAIL 반환
 */
static int getBufferSize(const char *msg, ...)
{
	if(checkObjectNull(msg, "메모리 참조 실패, 메시지 문자열이 NULL. (getBufferSize)") == YES)
	{
		return FAIL;
	}

	va_list args;
	va_start(args, msg);

	int result = vsnprintf(NULL, 0, msg, args);
	if(result < 0)
	{
		return FAIL;
	}

	va_end(args);

	return (result + 1); // [+1] : for NULL character
}

/**
 * @fn static int getDigitOfNumber(int number)
 * @brief 정수의 자리수를 계산해서 반환하는 함수
 * 음수이면 '-' 부호도 자리수에 포함한다.
 * @param number 자리수를 계산할 정수(입력)
 * @return 항상 숫자의 자리수 반환
 */
static int getDigitOfNumber(int number)
{
	int countOfDigit = 0;

	if(number < 0) countOfDigit++;
	else if(number == 0) return 1;

	while(number != 0)
	{
		countOfDigit++;
		number /= 10;
	}

	return countOfDigit;

	/* if (sprintf(buf, "%d", number) < 0)
	   {
	   return FAIL;
	   }
	   return strlen(buf); */
}

/**
 * @fn static int addNumberToString(char *string, int number)
 * @brief 정수를 문자열로 변환해서 전달받은 문자열에 추가하는 함수
 * @param string 추가될 문자열(출력)
 * @param number 추가할 정수(입력)
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
static int addNumberToString(char *string, int number)
{
	if(checkObjectNull(string, "메모리 참조 실패, 추가될 문자열이 NULL. (addNumberToString)") == YES)
	{
		return FAIL;
	}

	int digitOfNumber = getDigitOfNumber(number);
	char buf[digitOfNumber + 1];
	int result = sprintf(buf, " %d", number);
	if(result < 0)
	{
		printMsg("snprintf 함수 동작 오류. (addNumberToString, returnValue:%d)", DEBUG, 1, result);
		return FAIL;
	}
	strcat(string, buf);

	return SUCCESS;
}

