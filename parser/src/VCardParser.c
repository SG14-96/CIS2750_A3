#include "VCardParser.h"
#include "parsingUtilities.h"
// ************* Card parser fucntions - MUST be implemented ***************
VCardErrorCode createCard(char* fileName, Card** newCardObject){
	printf("FileName: %s\n",fileName);
    
    if(isCorrectFileExtension(fileName) != true){
        *newCardObject = NULL;
        return INV_FILE;
    }
    
    FILE * vcardFile = fopen(fileName,"rw+");
    if (vcardFile == NULL) {
        return INV_FILE;
    }
    
    *newCardObject = init_Card();
    
    if (*newCardObject == NULL) {
        return OTHER_ERROR;
    }
    
    //--------------------------Required Fields------
    bool beginPresent = false;
    bool versionIsCorrect = false;
    bool fnPresent = false;
    bool endPresent = false;
    //-----------------------------------------------
    int lineCounter=numberOfLines(&vcardFile);
    char lineBuffer[1000];
    char prevBuffer[1000];
    char * trimmmedLine = NULL;
    char * unfoldedLine = NULL;
    bool useUnfoldedLine = false;

    for(int i = 0; i <= lineCounter ;i++){
        fgets(lineBuffer,1000,vcardFile);
        /*
        if(!isLineValid(lineBuffer)){
           cleanUP(&vcardFile,newCardObject);
           return INV_PROP;
        }
        */
        if(lineBuffer[0] == '\t' || lineBuffer[0] == ' '){
            useUnfoldedLine = true;
            unfoldedLine = malloc(sizeof(char)*(strlen(prevBuffer)+strlen(lineBuffer)));
            strcpy(unfoldedLine,prevBuffer);
            strcat(unfoldedLine,lineBuffer);
            fgets(lineBuffer,1000,vcardFile);
        }else{
            useUnfoldedLine = false;
        }
        //-----parsing--------------------
        if(i > 0){
            if(useUnfoldedLine){
                trimmmedLine = trimLine(unfoldedLine);
            }else{
                trimmmedLine = trimLine(prevBuffer);
            }
            char paramsLine[1000];
            char valuesLine[1000];
            bool parseValues = false;
            int i = 0,j=0;
            
            while(trimmmedLine[i] != '\0'){
				if(trimmmedLine[i]==':' && parseValues == false){
					parseValues = true;
					paramsLine[i] = '\0';
					i++;
				}
				if(!parseValues){
					paramsLine[i] = trimmmedLine[i];
				}else{
					valuesLine[j] = trimmmedLine[i];
					j++;
				}
				i++;
			}
			valuesLine[j] = '\0';
			/*
            printf("%s -> %s\n",paramsLine,valuesLine);
            printf("-------------------------------------------\n");
            */
            if(strcmp("BEGIN",paramsLine)==0){beginPresent = true;}
            if(strcmp("END",paramsLine)==0) {endPresent = true;}

            if(strcmp("BEGIN",paramsLine)!=0 && strcmp("END",paramsLine)!=0){
                if (strcmp("FN",paramsLine)==0) {
                    (*newCardObject)->fn =  parsePropsParams(paramsLine,valuesLine);
                    fnPresent = true;
                }else if(strcmp("ANNIVERSARY",paramsLine)==0){
                    (*newCardObject)->birthday =  parseDateTime(valuesLine);
                }else if(strcmp("BDAY",paramsLine)==0){
                    (*newCardObject)->anniversary =  parseDateTime(valuesLine);
                }else if(strcmp("VERSION",paramsLine)==0){
                    versionIsCorrect = true;
                    if(strcmp("4.0",valuesLine)==0){
                        versionIsCorrect = true;
                    }
                }else{
                    insertBack((*newCardObject)->optionalProperties,(void*)parsePropsParams(paramsLine,valuesLine));
                }
            }
        }
        if(unfoldedLine != NULL){
            free(unfoldedLine);
        }
        if(trimmmedLine != NULL){
            free(trimmmedLine);
        }
        strcpy(prevBuffer,lineBuffer);
    }
    // ---- Error checkin--------------------------
    if ((fnPresent == false) || (beginPresent == false) || (endPresent == false) || (versionIsCorrect == false)) {
        cleanUP(&vcardFile,newCardObject);
        return INV_CARD;
    }
    //----If everything went gucci----------------
    fclose(vcardFile);
    return OK;
}
void deleteCard(Card* obj){
    if (obj != NULL) {
        freeList(obj->optionalProperties);
        deleteProperty(obj->fn);
        deleteDate(obj->birthday);
        deleteDate(obj->anniversary);
        free(obj);
    }
}
char* printCard(const Card* obj){
    if(obj == NULL){
        return NULL;
    }
    char * cardString = NULL;
    char * name = printProperty((void*)obj->fn);

    char *birthday;
    if(obj->birthday == NULL){
		birthday = malloc(sizeof(char)*5);
        strcpy(birthday,"'none");
    }else{
		birthday = printDate((void*)obj->birthday);
	}
    char *anniversary;
    if(obj->anniversary == NULL){
		anniversary = malloc(sizeof(char)*5);
        strcpy(anniversary,"none");
    }else{
		anniversary = printDate((void*)obj->anniversary);
	}
    char *props;
    if(obj->optionalProperties->head == 0){
		props = malloc(sizeof(char)*5);
        strcpy(props,"none");
    }else{
		props = toString(obj->optionalProperties);
	}
    
    
	int strLen = strlen(name)+strlen(birthday)+strlen(anniversary)+strlen(props)+150;
	cardString = malloc(sizeof(char)*(strLen));
	snprintf(cardString,strLen,"New Card:\n \t %s\n BIRTHDAY:%s\n \t ANNIVERSARY:%s\n \t PROPERTIES:%s\n",name,birthday,anniversary,props);
	free(name);
	free(birthday);
	free(anniversary);
	free(props);
    return cardString;
}
char* printError(VCardErrorCode err){
    char * errorString = malloc(sizeof(char)*250);
    switch (err){
        case OK:
            sprintf(errorString,"VCard created succesfully.\n");
            break;
        case INV_FILE:
            sprintf(errorString,"There was an issue with the file. Either can't open or file does not exist.\n");
            break;
        case INV_CARD:
            sprintf(errorString,"The card is not valid. Could be missing tags ,required properties, or version mismatch.\n");
            break;
        case INV_PROP:
            sprintf(errorString,"A property inside the file is not correctly formatted.\n");
            break;
        case WRITE_ERROR:
        sprintf(errorString,"There was an issue writing to a new vcard file.\n");
            break;
        case OTHER_ERROR:
            sprintf(errorString,"Some other error occured.\n");
        default:
            sprintf(errorString,"Unknow Error\n");
            break;
    }
    return errorString;
}
// *************************************************************************

// ************* List helper functions - MUST be implemented *************** 
void deleteProperty(void* toBeDeleted){
    if (toBeDeleted == NULL) {
        return;
    }
    Property * prop = (Property*)toBeDeleted;
    free(prop->name);
    if(prop->group != NULL) free(prop->group);
    freeList(prop->parameters);
    freeList(prop->values);
    free(prop);
}
int compareProperties(const void* first,const void* second){
    return 0;
}
char* printProperty(void* toBePrinted){
    if (toBePrinted == NULL) {
        return NULL;
    }
    int totalStrLen = 0;
    Property * prop = (Property*)toBePrinted;
    char * name = prop->name;
    char * group = prop->group;
    char * paramtersString = NULL;
    char * valuesString = NULL;
    char * propertyString = NULL;
    if(group != NULL && strcmp("\0",group)!=0){
        totalStrLen = totalStrLen + 1000;
    }
    if(prop->parameters != NULL){
        paramtersString = toString(prop->parameters);
        totalStrLen = totalStrLen + strlen(paramtersString);
    }
    if(prop->values != NULL){
        valuesString = toString(prop->values);
        totalStrLen = totalStrLen + strlen(valuesString);
    }
    totalStrLen = totalStrLen + strlen(name);
 
    propertyString = malloc(sizeof(char)*(totalStrLen+1500));
    sprintf(propertyString,"%s;%s:%s",prop->name,paramtersString,valuesString);

    free(paramtersString);
    free(valuesString);
    return propertyString;
}

void deleteParameter(void* toBeDeleted){
    if (toBeDeleted == NULL) {
        return;
    }
    Parameter * param = (Parameter*)toBeDeleted;
    free(param);
}
int compareParameters(const void* first,const void* second){
    return 0;
}
char* printParameter(void* toBePrinted){
    if (toBePrinted == NULL) {
        return NULL;
    }
    Parameter * param = (Parameter*)toBePrinted;
    char * paramString = malloc(sizeof(char)*(strlen(param->value)+strlen(param->value)+10));
    sprintf(paramString,"%s=%s",param->name,param->value);
    return paramString;
}

void deleteValue(void* toBeDeleted){
    if (toBeDeleted == NULL) {
        return;
    }
    char * value = (char *)toBeDeleted;
    free(value);
}
int compareValues(const void* first,const void* second){
    return 0;
}
char* printValue(void* toBePrinted){
    if (toBePrinted == NULL) {
        return NULL;
    }
    char * value = (char *)toBePrinted;
    char * string = malloc(sizeof(char)*(strlen(value)+5));
    sprintf(string,"%s",value);
    return string;
}

void deleteDate(void* toBeDeleted){
    if (toBeDeleted == NULL) {
        return;
    }
    DateTime * date = (DateTime*)toBeDeleted;
    free(date);
}
int compareDates(const void* first,const void* second){
    return 0;
}
char* printDate(void* toBePrinted){
    if (toBePrinted == NULL) {
        return NULL;
    }
    DateTime * date = (DateTime*)toBePrinted;
    int stringLength = 0;
    char * dateString = NULL;
    
    if (date->isText) {
        stringLength = strlen(date->text)+10;
        dateString = malloc(sizeof(char)*stringLength);
        sprintf(dateString,"%s",date->text);
    }else{
        if (strcmp("\0",date->date)==0) {
            stringLength = strlen(date->time)+10;
            dateString = malloc(sizeof(char)*stringLength);
            if (date->UTC) {
                sprintf(dateString,"%sZ",date->time);
            }else{
                sprintf(dateString,"%s",date->time);
            }
        }else if(strcmp("\0",date->time)==0){
            stringLength = strlen(date->date)+10;
            dateString = malloc(sizeof(char)*stringLength);
            sprintf(dateString,"%s",date->date);
        }else{
            stringLength = strlen(date->time)+strlen(date->date)+10;
            dateString = malloc(sizeof(char)*stringLength);
            if(date->UTC){
                sprintf(dateString,"%sT%sZ",date->date,date->time);
            }else{
                sprintf(dateString,"%sT%s",date->date,date->time);
            }
        }
    }
    return dateString;
}
// **************************************************************************

/** Function to writing a Card object into a file in iCard format.
 *@pre Card object exists, and is not NULL.
        fileName is not NULL, has the correct extension
 *@post Card has not been modified in any way, and a file representing the
        Card contents in vCard format has been created
 *@return the error code indicating success or the error encountered when traversing the Card
 *@param obj - a pointer to a Card struct
		 fileName - the name of the output file
 **/
VCardErrorCode writeCard(const char* fileName, const Card* obj){
    if(fileName == NULL || !isCorrectFileExtension(fileName) || strcmp("\0",fileName)==0){
        return WRITE_ERROR;
    }
    if(obj == NULL) return INV_CARD;
    FILE * writeFile = fopen(fileName,"w+");
    if(writeFile == NULL){
		return WRITE_ERROR;
	}
    fprintf(writeFile,"BEGIN:VCARD\r\n");
    fprintf(writeFile,"VERSION:4.0\r\n");
    char * nameString = printProperty((void*)obj->fn);
    fprintf(writeFile,"%s\r\n",nameString);
    free(nameString);
    if(obj->birthday != NULL){
        char * birthdayString = printDate((void*)obj->birthday);
        fprintf(writeFile,"BDAY:%s\r\n",birthdayString);
        free(birthdayString);
    }
    if(obj->anniversary != NULL){
        char * anniversaryString = printDate((void*)obj->anniversary);
        if(anniversaryString != NULL){
			fprintf(writeFile,"ANNIVERSARY:%s\r\n",anniversaryString);
			free(anniversaryString);
		}
    }
    
    if(obj->optionalProperties != NULL && obj->optionalProperties->length > 0){
        Node * currentNode = obj->optionalProperties->head;
        while(currentNode != NULL){
            char * propString = printProperty(currentNode->data);
            if(propString != NULL){
				fprintf(writeFile,"%s\r\n",propString);
				free(propString);
			}
            currentNode = currentNode->next;
        }
    }
    
    fprintf(writeFile,"END:VCARD\r\n");
    fclose(writeFile);
    return OK;
}



/** Function to writing a Card object into a file in iCard format.
 *@pre Card object exists, and is not NULL.
 *@post Card has not been modified in any way, and a file representing the
        Card contents in vCard format has been created
 *@return the error code indicating success or the error encountered when validating the Card
 *@param obj - a pointer to a Card struct
 **/
VCardErrorCode validateCard(const Card* obj){
    if(obj == NULL) return INV_CARD;
    if(obj->fn == NULL) return INV_PROP;
    if(strcmp(obj->fn->name,"\0")==0) return INV_CARD;
    if(obj -> birthday != NULL){
		if(obj->birthday->isText == true && strcmp("\0",obj->birthday->text)==0)return INV_DT;
	}
	if(obj->anniversary != NULL){
		if(obj->anniversary->isText == true && strcmp("\0",obj->anniversary->text)==0)return INV_DT;
	}
    
    Node * propsNode = obj->optionalProperties->head;
    while(propsNode!= NULL){
		Property * p = (Property*) propsNode->data;
		if(p == NULL) return INV_PROP;
		if(p->name == NULL || strcmp(p->name,"\0")==0) return INV_PROP;
		if(p->group == NULL) return INV_PROP;
		if(p->values == NULL || p->values->length == 0) return INV_PROP;
		if(strcmp("VERSION",p->name)==0){
			return INV_CARD;
		}else if(strcmp("BDAY",p->name)==0 || strcmp("ANNIVERSARY",p->name)==0){
			return INV_DT;
		}else{
			if(!isValidProp(p->name)){ 
				return INV_PROP;
			}else{
				int occurences = numOfOccurences(obj->optionalProperties,p->name);
				if(oneTooManyOccurences(occurences,p->name)){return INV_PROP;}
			}
		}
		propsNode=propsNode->next;
	}
    return OK;
}


/** Function for converting a list of strings into a JSON string
 *@pre List exists, is not null, and is valid
 *@post List has not been modified in any way, and a JSON string has been created
 *@return newly allocated JSON string.  May be NULL.
 *@param strList - a pointer to an Individual struct
 **/
char* strListToJSON(const List* strList){
    if(strList == NULL || strList->length <= 0) return NULL;
    char * JSONString = malloc(sizeof(char)*5);
    strcpy(JSONString,"[");
    Node * currentNode = strList->head;
    int i = 0;
    while(currentNode != NULL){
		char* currDescr = strList->printData(currentNode->data);
		int newLen = strlen(JSONString)+50+strlen(currDescr);
		JSONString = (char*)realloc(JSONString, newLen);
        if(i != 0){
            strcat(JSONString,",\0");
        }
        strcat(JSONString,"\"\0");
		strcat(JSONString, currDescr);
        strcat(JSONString,"\"\0");
		
		free(currDescr);
		currentNode = currentNode->next;
	}
    strcat(JSONString,"\0");
    return JSONString;
}


/** Function for creating an List of strings from an JSON string
 *@pre String is not null, and is valid
 *@post String has not been modified in any way, and a List has been created
 *@return a newly allocated List.  May be NULL.
 *@param str - a pointer to a JSON string
 **/
List* JSONtoStrList(const char* str){
    
    if(str == NULL || strlen(str) == 0) return NULL;
    List * returnList = initializeList(NULL,deleteStr,NULL);
    List * jsonList = JSONParserArray(str);

    Node * current = jsonList->head;
    while(current != NULL){
        char * toCopy = (char *) current->data;
        char * newAloc = malloc(sizeof(char)*strlen(toCopy));
        insertBack(returnList,(void*)newAloc);
        current = current->next;
    }
    clearList(jsonList);
    return returnList;
}


/** Function for converting a Property struct into a JSON string
 *@pre Property exists, is not null, and is valid
 *@post Property has not been modified in any way, and a JSON string has been created
 *@return newly allocated JSON string.  May be NULL.
 *@param strList - a pointer to a Property struct
 **/
char* propToJSON(const Property* prop){
    if(prop==NULL) return NULL;
    Property * nonConst = (Property*)prop;
    char tempStr[1000];
    char * propGroup = nonConst->group;
    char * propName = nonConst->name;
    Node * valuesList = nonConst->values->head;
    char * JSONString = malloc(sizeof(char)*5);
    strcat(JSONString,"{");
    if(propGroup != NULL && strcmp(propGroup,"\0")!=0){
        int newLen = strlen(JSONString)+1000+strlen(propGroup);
		JSONString = (char*)realloc(JSONString, newLen);
        sprintf(tempStr,"\"group\":\"%s\"",propGroup);
        strcat(JSONString,tempStr);
    }else{
        int newLen = strlen(JSONString)+1000;
		JSONString = (char*)realloc(JSONString, newLen);
        sprintf(tempStr,"\"group\":\"""\"");
        strcat(JSONString,tempStr);
    }
    strcat(JSONString,",");

    int newLen = strlen(JSONString)+1000+strlen(propName);
	JSONString = (char*)realloc(JSONString, newLen);
    sprintf(tempStr,"\"name\":\"%s\",values:[",propName);
    strcat(JSONString,tempStr);

    if(nonConst->values->length > 0){
		int i = 1;
		while(valuesList != NULL){
			char * valueString = (char*)valuesList->data;
			JSONString = realloc(JSONString,strlen(valueString)+1000);
            if(i > 1 && i <= nonConst->values->length){
				strcat(JSONString,",");
			}
			sprintf(tempStr,"\"%s\"",valueString);
            strcat(JSONString,tempStr);
			
			i++;
			valuesList=valuesList->next;
		}
		strcat(JSONString,"]");
	}

    strcat(JSONString,"}");
    return JSONString;
}


/** Function for creating a Property struct from an JSON string
 *@pre String is not null, and is valid
 *@post String has not been modified in any way, and a Property struct has been created
 *@return a newly allocated Property.  May be NULL.
 *@param str - a pointer to a JSON string
 **/
Property* JSONtoProp(const char* str){
    
    if(str==NULL||strcmp("\0",str)==0) return NULL;
    Property * newPROP = init_Property();
    List* JSONMap = JSONParserArray((char*)str);
    strcpy(newPROP->group,(char*)(JSONMap->head->next->data));
    strcpy(newPROP->name,(char*)(JSONMap->head->next->next->next->data));
    Node * current = (JSONMap->head->next->next->next->next->next);
    while(current){
        char * toCopy = (char*) current->data;
		char * newEntry = malloc(sizeof(char)*strlen(toCopy)+1);
        strcpy(newEntry,toCopy);
        insertBack(newPROP->values,(void*)newEntry);
		current= current->next;
	}
	clearList(JSONMap);
    return newPROP;
    
}


/** Function for converting a DateTime struct into a JSON string
 *@pre DateTime exists, is not null, and is valid
 *@post DateTime has not been modified in any way, and a JSON string has been created
 *@return newly allocated JSON string.  May be NULL.
 *@param strList - a pointer to a DateTime struct
 **/
char* dtToJSON(const DateTime* prop){
    if(prop == NULL)return NULL;
    DateTime * nonConstProp = (DateTime*)prop;
    char * JSONDate = malloc(sizeof(char)*50);
    if(nonConstProp->isText){
        JSONDate = realloc(JSONDate,sizeof(char)*strlen(nonConstProp->text)+500);
        sprintf(JSONDate,"{\"isText\":true,\"date\":\"\",\"time\":\"\",\"text\":\"%s\",\"isUTC\":false}",nonConstProp->text);
    }else{
        JSONDate = realloc(JSONDate,sizeof(char)*(strlen(nonConstProp->date)+strlen(nonConstProp->time)+500));
        char utc[10];
        if(nonConstProp->UTC){
            strcpy(utc,"true");
        }else{
            strcpy(utc,"false");
        }
        sprintf(JSONDate,"{\"isText\":false,\"date\":\"%s\",\"time\":\"%s\",\"text\":\"\",\"isUTC\":%s}",nonConstProp->date,nonConstProp->time,utc);
    }
    return JSONDate;
}


/** Function for creating a DateTime struct from an JSON string
 *@pre String is not null, and is valid
 *@post String has not been modified in any way, and a DateTime struct has been created
 *@return a newly allocated DateTime.  May be NULL.
 *@param str - a pointer to a JSON string
 **/
DateTime* JSONtoDT(const char* str){
    
    if(str==NULL||strcmp("\0",str)==0) return NULL;
    List* jsonarray = JSONParserArray((char*)str);
    DateTime * newDT = init_DateTime((char*)str);
    if(strcmp("false",(char*)getDataFromList(jsonarray,1))==0){
        newDT->isText = false;
        strcpy(newDT->date,(char*)getDataFromList(jsonarray,3));
        strcpy(newDT->time,(char*)getDataFromList(jsonarray,5));
        if(strcmp("false",(char*)getDataFromList(jsonarray,10))==0){
            newDT->UTC = false;
        }else{
            newDT->UTC = true;
        }
    }else{
        newDT->isText = true;
        newDT->UTC = false;
        strcpy(newDT->text,(char*)getDataFromList(jsonarray,8));
    }
    clearList(jsonarray);
    return newDT;
   
}


/** Function for creating a Card struct from an JSON string
 *@pre String is not null, and is valid
 *@post String has not been modified in any way, and a Card struct has been created
 *@return a newly allocated Card.  May be NULL.
 *@param str - a pointer to a JSON string
 **/
Card* JSONtoCard(const char* str){
	if(str == NULL ||strcmp(str,"\0")==0) return NULL;
    char * strnonconst = (char*)str;
	Card * newCard = init_Card();
	List * JSONMap = JSONParserArray(strnonconst);
	newCard->fn = init_Property();
	strcpy(newCard->fn->name,(char*)getDataFromList(JSONMap,1));
	
	clearList(JSONMap);
	return newCard;
}


/** Function for adding an optional Property to a Card object
 *@pre both arguments are not NULL and valid
 *@post Property has not been modified in any way, and its address had been added to 
 *      Card's optionalProperties list
 *@return void
 *@param obj - a pointer to a Card struct
 *@param toBeAdded - a pointer to an Property struct
**/
void addProperty(Card* card, const Property* toBeAdded){
    if(card == NULL || toBeAdded == NULL) return;
	insertBack(card->optionalProperties,(void*)toBeAdded);
}


char * vCardSummary(char * filename){
    if(filename == NULL || strcmp(filename,"\0")==0) return NULL;
    Card * newCard = NULL;
    VCardErrorCode errorCode = createCard(filename,&newCard);
    if(errorCode != OK){
        return printError(errorCode);
    }else{
        char * nameJSON = propToJSON(newCard->fn);
        int numOfProperties = newCard->optionalProperties->length;
        int strLen = strlen(nameJSON) + 100;
        char * JSONcardSummary = malloc(sizeof(char)*strLen);
        sprintf(JSONcardSummary,"{\"filename\":%s,%s,\"numprops\":%d}",filename,nameJSON,numOfProperties);
        free(nameJSON);
        deleteCard(newCard);
        return JSONcardSummary;
    }
}

char * vCardDataJSON(char * filename){
    if(filename == NULL) return NULL;

    Card * openCard = NULL;
    VCardErrorCode err = createCard(filename,&openCard);
    char * JSONString = malloc(sizeof(char)*15);
    strcpy(JSONString,"{");
    char * fnString = propToJSON(openCard->fn);
    if(fnString != NULL){
        JSONString = realloc(JSONString,strlen(fnString)+1);
        strcat(JSONString,fnString);
        strcat(JSONString,",");
    }
    char * annJSON = dtToJSON(openCard->anniversary);
    if(annJSON != NULL){
        JSONString = realloc(JSONString,strlen(annJSON)+1);
        strcat(JSONString,annJSON);
        strcat(JSONString,",");
    }
    char * bdayJSON = dtToJSON(openCard->birthday);
    if(bdayJSON != NULL){
        JSONString = realloc(JSONString,strlen(bdayJSON)+1);
        strcat(JSONString,bdayJSON);
        strcat(JSONString,",");
    }
    int i = 1;
    Node * currPropNode = openCard->optionalProperties->head;
    while(currPropNode != NULL){
        Property * currProp = (Property*)currPropNode->data;
        char * propJSON = propToJSON(currProp);
        JSONString = realloc(JSONString,strlen(propJSON)+1);
        strcat(JSONString,propJSON);
        if(i < openCard->optionalProperties->length){
            strcat(JSONString,",");
        }
        currPropNode = currPropNode->next;
    }

    return JSONString;
}
