#ifndef _PARSING_UTILITIES_
#define _PARSING_UTILITIES_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "VCardParser.h"
#include "LinkedListAPI.h"

//---------data checking---------------------
bool isLineValid(char *lineBuffer);
char* trimLine(char * line);
//--------------propertyFunctions
Property * parsePropsParams(char * lineWithParams, char * lineWithValue);
//-------dateTime functions
DateTime * parseDateTime(char * date_timeString);
bool stringContainsTime(char * string);
bool stringContainsDate(char * string);
bool isDateString(char * string);
//------------Init functions-----------
DateTime * init_DateTime(char * date_timeString);
Parameter * init_Paramter(char * token);
Property * init_Property(void);
Card * init_Card(void);
//------------Extra-------------------
void cleanUP(FILE ** inputFile,Card** card);
bool isCorrectFileExtension(const char * fileString);
int numberOfLines(FILE**file);
//------------------JSON Utility Functions----------
void deleteStr(void * toDelete);
List* JSONParserArray(const char * JSONString);
void * getDataFromList(List * list,int index);
bool isValidProp(char * cardProp);
int numOfOccurences(List * propsList, char * propName);
bool oneTooManyOccurences(int numOccurences,char * propName);
#endif
