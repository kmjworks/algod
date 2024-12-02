#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"



#pragma warning(disable:4996)
#define OBJECTS_NUM 21

void PrintObjects(HeaderD* pStruct7);
int insertNewObject(HeaderD** pStruct7, char *pNewID, int NewCode); 
Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD); 

time_t raw_time; 


int main(void) {

	HeaderD* pStruct = GetStruct7(7, OBJECTS_NUM);
	HeaderD** test = &pStruct; 
	char newid[] = "Abcads";
	insertNewObject(test, newid, 341241);
	

	PrintObjects(pStruct); 
	
	
	return 0;
}


 void PrintObjects(HeaderD* pStruct7) {
	Object7* t; 
	int i = 1;
	int identifier_counter = 0; 

	while (pStruct7) {
		t = (Object7*)pStruct7->pObject; 
		while (t) {
			printf("%d) %s %lu %02d %s %04d\n", i, t->pID, t->Code, t->pDate2->Day, t->pDate2->Month, t->pDate2->Year);
			t = t->pNext; 
			++i; 
		}

		pStruct7 = pStruct7->pNext;
		
	}
} 

 

 int insertNewObject(HeaderD** pStruct7, char *pNewID, int NewCode) {

	 if (*pNewID < 'A' || *pNewID > 'Z' || NewCode <= 0 || sizeof(pNewID) < 3) {
		 return 0; 
	}

	 HeaderD* backup = *pStruct7; 
	 backup = (*pStruct7); 

	 if (*pNewID == 'A' && (*pStruct7)->cBegin != 'A') {
		
		 
		 HeaderD* new_head = (HeaderD*)malloc(sizeof(pStruct7)); 
		// new_head->cBegin = (char)malloc(sizeof(1)); 
		 new_head->cBegin = *pNewID; 
		 new_head->pNext = (*pStruct7);
		 new_head->pPrior = NULL; 
		 (*pStruct7)->pPrior = new_head;

		 Object7* new_obj = (Object7*)malloc(sizeof(Object7)); 
		 new_obj->Code = NewCode; 
		 new_obj->pDate2 = (Date2*)malloc(sizeof(Date2)); 
		 GetDate2(time(&raw_time), new_obj->pDate2); 
		 new_obj->pID = (char*)malloc(sizeof(pNewID)+1); // account for NULL terminator 
		 strcpy(new_obj->pID, pNewID);
		 new_head->pObject = new_obj; 

		 return (int)new_head; 
		 
	 }
	 else {
		 HeaderD* helper = (*pStruct7); 

		 while (helper) {
			 if (helper->cBegin == *pNewID) {
				 Object7* new_obj = (Object7*)malloc(sizeof(Object7)); 
				 Object7* helper_obj = (Object7*)(*pStruct7)->pObject;
				 while (helper_obj) {
					 for (int i = 1; i < sizeof(pNewID); ++i) {
						 if (helper_obj->pID[i] < pNewID[i]) {
							 helper->pObject = helper_obj->pNext;

						 }
						 else {
							 new_obj->pNext = (Object7*)helper->pObject;
							 helper->pObject = new_obj;
							 new_obj->pDate2 = (Date2*)malloc(sizeof(Date2)); 
							 GetDate2(time(&raw_time), new_obj->pDate2); 
							 new_obj->pID = (char*)malloc(sizeof(pNewID)); 
							 strcpy(new_obj->pID, pNewID); 
							 new_obj->Code = NewCode;
							 return 1; 

						 }
					 }
				 }
			 }
			 helper = helper->pNext;
		 }
		 
	 }
	 
	 (*pStruct7) = backup; 

	 while (backup) {
		 if (backup->cBegin < *pNewID) {
			 HeaderD* new_node = (HeaderD*)malloc(sizeof(pStruct7));
			 new_node->cBegin = NULL; 
			 new_node->cBegin = (char)malloc(sizeof(char));
			 new_node->cBegin = *pNewID; 
			 new_node->pObject = NULL; 
			 new_node->pPrior = NULL; 
			 new_node->pNext = NULL; 

			 new_node->pPrior = backup->pPrior; 
			 new_node->pNext = backup; 
			 backup->pPrior = new_node;

			 Object7* new_obj = (Object7*)malloc(sizeof(Object7)); 
			 new_obj->Code = NewCode; 
			 new_obj->Code = NewCode; 
			 new_obj->pDate2 = (Date2*)malloc(sizeof(Date2)); 
			 GetDate2(time(&raw_time), new_obj->pDate2); 
			 new_obj->pID = (char*)malloc(sizeof(pNewID));
			 strcpy(new_obj->pID, pNewID); 
			 new_obj->pNext = NULL; 
			 return 1; 
		 }
	 }
	 

	 return 0; 
 }

 Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD) {
	 uint32_t node_count, object_count; 
	 Object7* helper_obj;
	 HeaderD** helper_struct; 
	 object_count = 0, node_count = 0; 
	 
	 
	 while ((*pStruct7)) {
		 
		 helper_struct = pStruct7; 
		 helper_obj = (Object7*)(*pStruct7)->pObject; 
		 while (helper_obj) {
			 if (helper_obj->pID = pExistingD) {
				 if (node_count == 0) {
					 (*pStruct7) = (*pStruct7)->pNext; 
					 return (Object7*)(*helper_struct)->pObject; 
				 }
				 (*pStruct7) = (*pStruct7)->pNext; 
				 return helper_obj; 
			 }
			 helper_obj = helper_obj->pNext; 
		 }


		 (*pStruct7) = (*pStruct7)->pNext; 
		 ++node_count; 
	 }

	 return 0; 
 }