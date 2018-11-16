#include "parsingUtilities.h"

char * validProps[] = {
	"SOURCE",
	"KIND",
	"XML",
	"FN",
	"NICKNAME",
	"PHOTO",
	"BDAY",
	"ANNIVERSARY",
	"GENDER",
	"ADR",
	"TEL",
	"EMAIL",
	"IMPP",
	"LANG",
	"TZ",
	"GEO",
	"TITLE",
	"ROLE",
	"LOGO",
	"ORG",
	"MEMBER",
	"RELATED",
	"CATEGORIES",
	"NOTE",
	"PRODID",
	"REV",
	"SOUND",
	"UID",
	"CLIENTPIDMAP",
	"URL",
	"KEY",
	"FBURL",
	"CALDRUIR",
	"CALURI"
};
char* occursMaxOnce[]={
	"N",
	"KIND",
	"BDAY",
	"ANNIVERSARY",
	"GENDER",
	"PRODID",
	"REV",
	"UID",
};

/*
    +-------------+--------------------------------------------------+
    | Cardinality | Meaning                                          |
    +-------------+--------------------------------------------------+
    |      1      | Exactly one instance per vCard MUST be present.  |
    |      *1     | Exactly one instance per vCard MAY be present.   |
    |      1*     | One or more instances per vCard MUST be present. |
    |      *      | One or more instances per vCard MAY be present.  |
    +-------------+--------------------------------------------------+
    Required Properties:
    FN
    VERSION
*/


char* trimLine(char * line){
    //printf("Pre trimmed line: %s",line);
    int lineLen = strlen(line);
    char * trimmedLine = malloc(sizeof(char)*(lineLen+1));
    int j = 0;
    for(int i = 0;i<lineLen;i++){
        if(line[i]!='\n' && line[i]!='\r' && line[i]!='\t'){
			if(line[i]==';' && line[i+1]==';'){
				trimmedLine[j] = line[i];
				j++;
				trimmedLine[j] = ' ';
			}else{
				trimmedLine[j] = line[i];
			}
            j++;
        }
    }
    trimmedLine[j] = '\0';
    //printf("Post reimmed line: %s\n",trimmedLine);
    return trimmedLine;
}

bool isLineValid(char *lineBuffer){
    bool hasColon = false;
    if(lineBuffer[strlen(lineBuffer)-1] == '\n' && lineBuffer[strlen(lineBuffer)-2] == '\r'){   
        for(int i = 0; i < strlen(lineBuffer); i++){
            if (lineBuffer[i] == '=' && !isalnum(lineBuffer[i+1])){
                //printf("check: %c\n",lineBuffer[i]);
                if(lineBuffer[i+1] != '-' || lineBuffer[i+1] != ';') return false;
            }
            if(lineBuffer[i] == ':' && !isalnum(lineBuffer[i+1])){
                //printf("check: %c\n",lineBuffer[i]);
                if(lineBuffer[i+1] != '-' || lineBuffer[i+1] != ';') return false;
            }
            if(lineBuffer[i] == ':') hasColon = true;
        }
    }
    if(hasColon){
        return true;
    }else{
        return false;
    }
}

Property * parsePropsParams(char * lineWithParams, char * lineWithValue){
    if (lineWithParams == NULL || lineWithValue == NULL) {
        return NULL;
    }
    
    Property *newProp = init_Property();
    //------Parse and Store parameters and their values-------------------------
    char * token = NULL;
    int counter = 0;
    token = strtok(lineWithParams,";");
    while(token != NULL){
        if (counter == 0) {
            //parse name of property.
            (newProp)->name = malloc(sizeof(char)*(strlen(token)+100));
            strcpy((newProp)->name,token);
        }else{
            Parameter * newParam = init_Paramter(token);
            //parse the parameters
            char * secToken = NULL;
            secToken = strtok(token,"=");
            //printf("paramName:%s\n",secToken);
            strcpy(newParam->name,secToken);
            secToken = strtok(NULL,"=");
            //printf("paramValue:%s\n",secToken);
            strcpy(newParam->value,secToken);
            insertBack((newProp)->parameters,(void*)newParam);
        }
        counter++;
        token = strtok(NULL,";");
    }
    //----------Parse values from this property---------------------------------------
    char * tok = NULL;
    tok = strtok(lineWithValue,",;");
    while(tok != NULL){
		//printf("token values: %s\n",tok);
        char * newValue = malloc(sizeof(char)*(strlen(tok)+100));
        if(strcmp(" ",tok)==0){
			strcpy(newValue,"\0");
		}else{
			strcpy(newValue,tok);	
        }
        insertBack((newProp)->values,(void*)newValue);
        tok = strtok(NULL,",;");
    }
    return newProp;
}

DateTime * parseDateTime(char * date_timeString){
    
    DateTime *newDateTime = init_DateTime(date_timeString);
    if (isDateString(date_timeString)) {
        (newDateTime)->isText = true;
        strncpy((newDateTime)->text,date_timeString,7);
    }else{
         (newDateTime)->isText = false;
        if(!stringContainsDate(date_timeString) && stringContainsTime(date_timeString)) {
            strcpy((newDateTime)->time,date_timeString);
            strcpy((newDateTime)->date," ");
            if ((newDateTime)->time[strlen((newDateTime)->time)-1] == 'Z') {
                newDateTime->UTC = true;
            }else{
                newDateTime->UTC = false;
            }
        }else if(stringContainsDate(date_timeString) && !stringContainsTime(date_timeString)){
            strcpy((newDateTime)->date,date_timeString);
        }else{
            char * token = NULL;
            token = strtok(date_timeString,"T");
            strcpy((newDateTime)->date,token);
            token = strtok(NULL,"T");
            strncpy((newDateTime)->time,token,7);
            if ((newDateTime)->time[7] == 'Z') {
                (newDateTime)->UTC = true;
            }else{
                (newDateTime)->UTC = false;
            }
        }
    }
    return newDateTime;
}

bool stringContainsTime(char * string){
    int i = 0;
    while(string[i] != '\0'){
        if (string[i] == 'T') {
            return true;
        }
        i++;
    }
    return false;
}
bool stringContainsDate(char * string){
    if (string[0]=='T') {
       return false;
    }else{
        return true;
    }
}
bool isDateString(char * string){
    if (isalpha(string[0])&&isalpha(string[1])) {
        return true;
    }else{
        return false;
    } 
}


DateTime * init_DateTime(char * date_timeString){
    DateTime * newDateTime = malloc(sizeof(DateTime)+sizeof(char)*strlen(date_timeString));
    (newDateTime)->UTC = false;
    (newDateTime)->isText = false;
    strcpy((newDateTime)->date,"\0");
    strcpy((newDateTime)->time,"\0");
    strcpy(newDateTime->text,"\0");
    return newDateTime;
}

Parameter * init_Paramter(char * token){
    Parameter * newParam = malloc(sizeof(Parameter)+sizeof(char)*strlen(token));
    strcpy(newParam->name,"\0");
    strcpy(newParam->value,"\0");
    return newParam;
}

Property * init_Property(void){
    Property *newProp = malloc(sizeof(Property));
    newProp->name = NULL;
    newProp->group = NULL;
    (newProp)->parameters = initializeList(printParameter,deleteParameter,compareParameters);
    (newProp)->values = initializeList(printValue,deleteValue,compareValues);
    return newProp;
}

Card * init_Card(void){
    Card *newCardObject = calloc(sizeof(Card), 1);
    newCardObject->fn = NULL;
    newCardObject->birthday = NULL;
    newCardObject->anniversary = NULL;
    newCardObject->optionalProperties = initializeList(&printProperty,&deleteProperty,&compareProperties);
    return newCardObject;
}

void cleanUP(FILE ** inputFile,Card** card){
    fclose(*inputFile);
    deleteCard(*card);
    *card = NULL;
}

bool isCorrectFileExtension(const char * fileString){
    char * point;
    if((point = strrchr(fileString,'.')) != NULL ) {
        if(strcmp(point,".vcf") == 0) {
            return true;
        }else if(strcmp(point,".vcard") == 0){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

int numberOfLines(FILE**file){
    int i = 0;
    char lineBuffer[1000];
    while(fgets(lineBuffer,1000,*file)!=NULL){
        if(lineBuffer[0] == '\t' || lineBuffer[0] == ' '){
            i--;
        }
        i++;
    }
    fseek(*file,0,SEEK_SET);
    return i;
}

void deleteStr(void * toDelete){
    char * deleteThis = (char*)toDelete;
    free(deleteThis);
}

List* JSONParserArray(const char * JSONString){
    List * listOfValues = initializeList(NULL,deleteStr,NULL);
    const char delims[5] = ",:{}";
    char * token = strtok((char *)JSONString,delims);

    while(token != NULL){
        char * newStr = malloc(sizeof(char)*(strlen(token)+3));
        if(strcmp("\"\"",token)==0){
            strcpy(newStr,"\0");
        }else{
            if(token[0] == '\"'){
                strncpy(newStr,token+1,strlen(token)-2);
            }else{
                strcpy(newStr,token);
            }
        }
        insertBack(listOfValues,(void*)newStr);
        token = strtok(NULL,delims);
    }

    return listOfValues;
}

void * getDataFromList(List * list,int index){
    Node * currentNode = list->head;
    for(int i = 0; i == index; i++){
        currentNode = currentNode->next;
    }
    return currentNode->data;
}

bool isValidProp(char * cardProp){
	for(int i = 0; i < 36; i++){
		if(strcmp(cardProp,validProps[i])==0){
			return true;
		}
	}
	return false;
}

int numOfOccurences(List * propsList, char * propName){
	int numOccurences = 0;
	Node * currentNode = propsList->head;
	while(currentNode != NULL){
		Property * p = (Property*)currentNode->data;
		if(strcmp(p->name,propName)==0){
			numOccurences++;
		}
		currentNode = currentNode->next;
	}
	return numOccurences;
}

bool oneTooManyOccurences(int numOccurences,char * propName){
	for(int i = 0; i < 8; i ++){
		if(strcmp(propName,occursMaxOnce[i])==0){
			if(numOccurences > 1) return true;
		}
	}
	return false;
}
