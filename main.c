#include "dynamicIntArray.h"

///////////////////////////////////////////////////////////////////////////////////////
/// Predefinitions of Condition Compare functions
///////////////////////////////////////////////////////////////////////////////////////

int compareIntOdd(int value);
int compareIntEven(int value);
int compareIntPositive(int value);
int compareIntNegative(int value);
int compareIntZero(int value);

///////////////////////////////////////////////////////////////////////////////////////
/// Main function
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn int main(int argc, char **argv)
 * @brief 동적 배열 테스트 프로그램
 * @param argc 프로그램 실행 매개변수 개수
 * @param argv 프로그램 실행 매개변수 배열
 * argv[0] : 프로그램 이름
 * argv[1] : 할당할 동적 배열의 크기
 * @return 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printMsg("잘못된 매개변수 입력.", ERROR, 0);
		printMsg("test6 (할당할 동적 배열의 크기) / 총 1 개", ERROR, 0);
		return FAIL;
	}


	//////////////////////////////////////////////////////////////
	printMsg("< 테스트 프로그램 시작 >", NORMAL, 0);

	dynamicIntArray_t *array = dynamicIntArrayNew(atoi(argv[1]));
	if(checkObjectNull(array, "메모리 할당 실패.") == YES)
	{
		return FAIL;
	}

	int loopIndex = UNKNOWN;

	dynamicIntArray_t *appendResult = NULL;
	dynamicIntArray_t *insertatResult = NULL;
	dynamicIntArray_t *removeatResult = NULL;

	int setResult = UNKNOWN;
	int reverseResult = UNKNOWN;
	int fillResult = UNKNOWN;
	int findResult = UNKNOWN;
	int indexofResult = UNKNOWN;
	int lastindexofResult = UNKNOWN;
	int getResult = UNKNOWN;
	int isError[1] = { FAIL };
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[APPEND TEST]", NORMAL, 0);
	printMsg("최초 1부터 5까지의 정수 추가", NORMAL, 0);
	for (loopIndex = 0; loopIndex < 5; loopIndex++)
	{
		appendResult = dynamicIntArrayAppend(array, loopIndex+1);
		if (appendResult == NULL)
		{
			printMsg("dynamicIntArrayAppend 실패.", ERROR, 0);
			//return FAIL;
		}
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INSERT_AT TEST]", NORMAL, 0);
	printMsg("인덱스 1에 2를 추가", NORMAL, 0);
	insertatResult = dynamicIntArrayInsertAt(array, 1, 2);
	if (insertatResult == NULL)
	{
		printMsg("dynamicIntArrayInsertAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INSERT_AT TEST]", NORMAL, 0);
	printMsg("인덱스 11에 2를 추가", NORMAL, 0);
	insertatResult = dynamicIntArrayInsertAt(array, 11, 2);
	if (insertatResult == NULL)
	{
		printMsg("dynamicIntArrayInsertAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INSERT_AT TEST]", NORMAL, 0);
	printMsg("인덱스 3에 9를 추가", NORMAL, 0);
	insertatResult = dynamicIntArrayInsertAt(array, 3, 9);
	if (insertatResult == NULL)
	{
		printMsg("dynamicIntArrayInsertAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REMOVE_AT TEST]", NORMAL, 0);
	printMsg("인덱스 0에 있는 값 제거", NORMAL, 0);
	removeatResult = dynamicIntArrayRemoveAt(array, 0);
	if (removeatResult == NULL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REMOVE_AT TEST]", NORMAL, 0);
	printMsg("인덱스 20에 있는 값 제거", NORMAL, 0);
	removeatResult = dynamicIntArrayRemoveAt(array, 20);
	if (removeatResult == NULL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REVERSE TEST]", NORMAL, 0);
	printMsg("배열 reverse", NORMAL, 0);
	reverseResult = dynamicIntArrayReverse(array);
	if (reverseResult == FAIL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INSERT_AT TEST]", NORMAL, 0);
	printMsg("인덱스 5에 -7을 추가", NORMAL, 0);
	insertatResult = dynamicIntArrayInsertAt(array, 5, -7);
	if (insertatResult == NULL)
	{
		printMsg("dynamicIntArrayInsertAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[CLONE TEST]", NORMAL, 0);
	dynamicIntArray_t *array2 = dynamicIntArrayClone(array);
	if (array2 == NULL)
	{
		printMsg("dynamicIntArrayClone 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array2), NORMAL, 0);
	dynamicIntArrayDelete(&array2);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REVERSE TEST]", NORMAL, 0);
	printMsg("배열 reverse", NORMAL, 0);
	reverseResult = dynamicIntArrayReverse(array);
	if (reverseResult == FAIL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INDEX_OF TEST]", NORMAL, 0);
	printMsg("index_of(5)", NORMAL, 0);
	indexofResult = dynamicIntArrayIndexOf(array, 5);
	if (indexofResult == FAIL)
	{
		printMsg("dynamicIntArrayIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if (indexofResult == UNKNOWN)
	{
		printMsg("dynamicIntArrayIndexOf 검색 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", indexofResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INDEX_OF TEST]", NORMAL, 0);
	printMsg("index_of(1)", NORMAL, 0);
	indexofResult = dynamicIntArrayIndexOf(array, 1);
	if (indexofResult == FAIL)
	{
		printMsg("dynamicIntArrayIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if (indexofResult == UNKNOWN)
	{
		printMsg("dynamicIntArrayIndexOf 검색 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", indexofResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[LAST_INDEX_OF TEST]", NORMAL, 0);
	printMsg("lastIndexOf(2)", NORMAL, 0);
	lastindexofResult = dynamicIntArrayLastIndexOf(array, 2);
	if (lastindexofResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if (lastindexofResult == UNKNOWN)
	{
		printMsg("dynamicIntArrayLastIndexOf 검색 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", lastindexofResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[INDEX_OF TEST]", NORMAL, 0);
	printMsg("index_of(9)", NORMAL, 0);
	indexofResult = dynamicIntArrayIndexOf(array, 9);
	if (indexofResult == FAIL)
	{
		printMsg("dynamicIntArrayIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if (indexofResult == UNKNOWN)
	{
		printMsg("dynamicIntArrayIndexOf 검색 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", indexofResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REMOVE_AT TEST]", NORMAL, 0);
	printMsg("인덱스 0에 있는 값 삭제", NORMAL, 0);
	removeatResult = dynamicIntArrayRemoveAt(array, 0);
	if (removeatResult == NULL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[APPEND TEST]", NORMAL, 0);
	printMsg("1 추가", NORMAL, 0);
	appendResult = dynamicIntArrayAppend(array, 1);
	if (appendResult == NULL)
	{
		printMsg("dynamicIntArrayAppend 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[APPEND TEST]", NORMAL, 0);
	printMsg("2 추가", NORMAL, 0);
	appendResult = dynamicIntArrayAppend(array, 2);
	if (appendResult == NULL)
	{
		printMsg("dynamicIntArrayAppend 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[APPEND TEST]", NORMAL, 0);
	printMsg("3 추가", NORMAL, 0);
	appendResult = dynamicIntArrayAppend(array, 3);
	if (appendResult == NULL)
	{
		printMsg("dynamicIntArrayAppend 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[FIND TEST]", NORMAL, 0);
	printMsg("find 짝수", NORMAL, 0);
	findResult = dynamicIntArrayFind(array, compareIntEven);
	if (findResult == FAIL)
	{
		printMsg("객체 참조 오류. dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if(findResult == UNKNOWN)
	{
		printMsg("검색 실패. dynamicIntArrayLastIndexOf 실패", ERROR, 0);
	}
	else
	{
		printf("result : %d\n", findResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[FIND TEST]", NORMAL, 0);
	printMsg("find 홀수", NORMAL, 0);
	findResult = dynamicIntArrayFind(array, compareIntOdd);
	if (findResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if(findResult == UNKNOWN)
	{
		printMsg("검색 실패. dynamicIntArrayLastIndexOf 실패", ERROR, 0);
	}
	else
	{
		printf("result : %d\n", findResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[FIND TEST]", NORMAL, 0);
	printMsg("find 양수", NORMAL, 0);
	findResult = dynamicIntArrayFind(array, compareIntPositive);
	if (findResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if(findResult == UNKNOWN)
	{
		printMsg("검색 실패. dynamicIntArrayLastIndexOf 실패", ERROR, 0);
	}
	else
	{
		printf("result : %d\n", findResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[FIND TEST]", NORMAL, 0);
	printMsg("find 음수", NORMAL, 0);
	findResult = dynamicIntArrayFind(array, compareIntNegative);
	if (findResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if(findResult == UNKNOWN)
	{
		printMsg("검색 실패. dynamicIntArrayLastIndexOf 실패", ERROR, 0);
	}
	else
	{
		printf("result : %d\n", findResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[FIND TEST]", NORMAL, 0);
	printMsg("find zero", NORMAL, 0);
	findResult = dynamicIntArrayFind(array, compareIntZero);
	if (findResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if(findResult == UNKNOWN)
	{
		printMsg("검색 실패. dynamicIntArrayLastIndexOf 실패", ERROR, 0);
	}
	else
	{
		printf("result : %d\n", findResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[LAST_INDEX_OF TEST]", NORMAL, 0);
	printMsg("lastIndexOf(2)", NORMAL, 0);
	findResult = dynamicIntArrayLastIndexOf(array, 2);
	if (findResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if (findResult == UNKNOWN)
	{
		printMsg("dynamicIntArrayLastIndexOf 검색 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", findResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[LAST_INDEX_OF TEST]", NORMAL, 0);
	printMsg("lastIndexOf(3)", NORMAL, 0);
	lastindexofResult = dynamicIntArrayLastIndexOf(array, 3);
	if (lastindexofResult == FAIL)
	{
		printMsg("dynamicIntArrayLastIndexOf 실패.", ERROR, 0);
		//return FAIL;
	}
	else if (lastindexofResult == UNKNOWN)
	{
		printMsg("dynamicIntArrayLastIndexOf 검색 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", lastindexofResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REMOVE_AT TEST]", NORMAL, 0);
	printMsg("인덱스 4에 있는 값 제거", NORMAL, 0);
	removeatResult = dynamicIntArrayRemoveAt(array, 4);
	if (removeatResult == NULL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[REMOVE_AT TEST]", NORMAL, 0);
	printMsg("인덱스 -2에 있는 값 제거", NORMAL, 0);
	removeatResult = dynamicIntArrayRemoveAt(array, -2);
	if (removeatResult == NULL)
	{
		printMsg("dynamicIntArrayRemoveAt 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[FILL TEST]", NORMAL, 0);
	printMsg("배열을 5로 채움", NORMAL, 0);
	fillResult = dynamicIntArrayFill(array, 5);
	if (fillResult == FAIL)
	{
		printMsg("dynamicIntArrayFill 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[SET TEST]", NORMAL, 0);
	printMsg("인덱스 2에 10을 저장", NORMAL, 0);
	setResult = dynamicIntArraySetElement(array, 2, 10);
	if (setResult == FAIL)
	{
		printMsg("dynamicIntArraySetElement 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[SET TEST]", NORMAL, 0);
	printMsg("인덱스 10에 10을 저장", NORMAL, 0);
	setResult = dynamicIntArraySetElement(array, 10, 10);
	if (setResult == FAIL)
	{
		printMsg("dynamicIntArraySetElement 실패.", ERROR, 0);
		//return FAIL;
	}

	printMsg(dynamicIntArrayToString(array), NORMAL, 0);
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[GET TEST]", NORMAL, 0);
	printMsg("인덱스 20의 값을 가져옴", NORMAL, 0);
	getResult = dynamicIntArrayGetElement(array, 20, isError);
	if (*isError == FAIL)
	{
		printMsg("dynamicIntArrayGetElement 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", getResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	printMsg("[GET TEST]", NORMAL, 0);
	printMsg("인덱스 1의 값을 가져옴", NORMAL, 0);
	getResult = dynamicIntArrayGetElement(array, 1, isError);
	if (*isError == FAIL)
	{
		printMsg("dynamicIntArrayGetElement 실패.", ERROR, 0);
		//return FAIL;
	}
	else
	{
		printf("result : %d\n", getResult);
	}
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	dynamicIntArrayDelete(&array);

	printMsg("< 테스트 프로그램 종료 >", NORMAL, 0);
	//////////////////////////////////////////////////////////////

	return FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////
/// Condition Compare functions
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn int compareIntOdd(int value)
 * @brief 정수가 홀수인지 비교하는 함수
 * @param value 비교할 정수
 * @return 홀수이면 YES, 아니면 NO 반환
 */
int compareIntOdd(int value)
{
	return (value % 2 != 0) ? YES : NO;
}

/**
 * @fn int compareIntEven(int value)
 * @brief 정수가 짝수인지 비교하는 함수
 * @param value 비교할 정수
 * @return 짝수이면 YES, 아니면 NO 반환
 */
int compareIntEven(int value)
{
	return (value % 2 == 0) ? YES : NO;
}

/**
 * @fn int compareIntPositive(int value)
 * @brief 정수가 양수인지 비교하는 함수
 * @param value 비교할 정수
 * @return 양수이면 YES, 아니면 NO 반환
 */
int compareIntPositive(int value)
{
	return (value > 0) ? YES : NO;
}

/**
 * @fn int compareIntNegative(int value)
 * @brief 정수가 음수인지 비교하는 함수
 * @param value 비교할 정수
 * @return 음수이면 YES, 아니면 NO 반환
 */
int compareIntNegative(int value)
{
	return (value < 0) ? YES : NO;
}

/**
 * @fn int compareIntZero(int value)
 * @brief 정수가 0 인지 비교하는 함수
 * @param value 비교할 정수
 * @return 0 이면 YES, 아니면 NO 반환
 */
int compareIntZero(int value)
{
	return (value == 0) ? YES : NO;
}


