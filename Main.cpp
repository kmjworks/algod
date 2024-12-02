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

 

 int insertNewObject(HeaderD** pStruct7, char* pNewID, int NewCode) {

	 if (*pNewID < 'A' || *pNewID > 'Z' || NewCode <= 0 || sizeof(pNewID) < 3) {
		 return 0;
	 }

	 HeaderD* current_header = *pStruct7;
	 HeaderD* previous_header = NULL;

	 // Traverse through the nodes to determine whether the list already exists
	 while (current_header != NULL && current_header->cBegin < *pNewID) {
		 previous_header = current_header; 
		 current_header = current_header->pNext; 
	 }

	 // If doesn't exist, create a new HeaderD node
	 if (current_header == NULL || current_header->cBegin != *pNewID) {
		 HeaderD* new_header = (HeaderD*)malloc(sizeof(HeaderD)); 
		 new_header->cBegin = *pNewID; // Set the new starting character of the list
		 new_header->pObject = NULL; 
		 new_header->pNext = current_header; 
		 new_header->pPrior = previous_header; 

		 if (previous_header != NULL) {
			 previous_header->pNext = new_header; 
		 }

		 if (previous_header == NULL) {
			 *pStruct7 = new_header; 
		 }

		 current_header = new_header; // Current header points to the newly created header

	 }


	 // Insert object into the list 
	 Object7* new_obj = (Object7*)malloc(sizeof(Object7)); 
	 new_obj->Code = NewCode; 
	 new_obj->pDate2 = (Date2*)malloc(sizeof(Date2));
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