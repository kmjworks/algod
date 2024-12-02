#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>



#pragma warning(disable:4996)
#define OBJECTS_NUM 35

void PrintObjects(HeaderD* pStruct7);
int insertNewObject(HeaderD** pStruct7, char *pNewID, int NewCode); 
Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD); 

time_t raw_time; 


int main(void) {

	HeaderD* pStruct = GetStruct7(7, OBJECTS_NUM);
	HeaderD** test = &pStruct; 

	try {
		char newid[] = "Dxer";
		insertNewObject(test, newid, 345241);

		char newid2[] = "Dbsdf";
		insertNewObject(test, newid2, 341241);

		char newid3[] = "Dzdsa";
		insertNewObject(test, newid3, 36732241);

		char newid4[] = "Dkas";
		insertNewObject(test, newid4, 323343241);

		char newid5[] = "Aasers";
		insertNewObject(test, newid5, 3232413);

		char newid6[] = "Wuasx";
		insertNewObject(test, newid6, 32655324);


		char newid7[] = "Wa";
		insertNewObject(test, newid7, 32773241);

		char newid8[] = "Zweas";
		insertNewObject(test, newid8, 32334241);

		char newid9[] = "Zasdf";
		insertNewObject(test, newid9, 32325641);

		//char newid10[] = "wksad";
		//insertNewObject(test, newid10, 32225641);

		//char newid11[] = "Wa";
		//insertNewObject(test, newid11, 32125641);

		//char newid12[] = "W8serad";
		//insertNewObject(test, newid12, 32525641);

		char newid13[] = "W_serad";
		insertNewObject(test, newid13, 32525641);

		PrintObjects(pStruct);
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Caught exception : " << e.what() << std::flush; 
	}
	
	
	return 0;
}


 void PrintObjects(HeaderD* pStruct7) {
	Object7* t; 
	int i = 1;
	
	while (pStruct7) {
		printf("Header for : %c\n", pStruct7->cBegin); 
		t = (Object7*)pStruct7->pObject; 
		while (t) {
			printf("%d) %s %lu %02d %s %04d\n", i, t->pID, t->Code, t->pDate2->Day, t->pDate2->Month, t->pDate2->Year);
			t = t->pNext; 
			i++; 
		}
		pStruct7 = pStruct7->pNext; 
	}
} 

 bool checkIfDuplicateExists(HeaderD* current_header, const char* pNewID) {
	 Object7* current_object = (Object7*)current_header->pObject; 
	 while (current_object != NULL) {
		 if (strcmp(current_object->pID, pNewID) == 0) {
			 return true; 
		 }
		 current_object = current_object->pNext; 
	 }
	 return false; 
 }

 int insertNewObject(HeaderD** pStruct7, char* pNewID, int NewCode) {

	 if (*pNewID < 'A' || *pNewID > 'Z' || NewCode <= 0 || strlen(pNewID) < 2) {
		 std::ostringstream errorMsg; 
		 errorMsg << "Invalid parameters - pNewID : " << pNewID << " - NewCode: " << NewCode; 
		 throw std::runtime_error(errorMsg.str()); 
	 }

	 for (size_t i = 0; i < strlen(pNewID); ++i) {
		 if (!std::isalpha(pNewID[i])) {
			 throw std::runtime_error("Invalid pNewID, contains non-alphabetic character : " 
									  + std::string(pNewID));
		 }
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
		 if (!new_header) {
			 throw std::runtime_error("Memory allocation failed"); 
		 }
		 new_header->cBegin = *pNewID; // Set the new starting character of the list
		 new_header->pObject = NULL; 
		 new_header->pNext = current_header; 
		 new_header->pPrior = previous_header; 

		 if (previous_header != NULL) {
			 previous_header->pNext = new_header; 
		 } 
		 else
		 {
			 *pStruct7 = new_header; 
		 }

		 current_header = new_header; // Current header points to the newly created header
		 printf("Created new HeaderD for : %c\n", *pNewID); 

	 }

	 if (checkIfDuplicateExists(current_header, pNewID)) {
		 std::ostringstream errorMsg;
		 errorMsg << "Object already exists - pNewID : " << pNewID; 
		 throw std::runtime_error(errorMsg.str());
	 }


	 // Insert object into the list 
	 Object7* new_obj = (Object7*)malloc(sizeof(Object7)); 
	 if (!new_obj) {
		 throw std::runtime_error("malloc() failed"); 
	 }
	 new_obj->Code = NewCode; 
	 new_obj->pDate2 = (Date2*)malloc(sizeof(Date2));
	 GetDate2(time(&raw_time), new_obj->pDate2); 
	 new_obj->pID = (char*)malloc(strlen(pNewID) + 1); // Account for '\0' operator
	 if (!new_obj->pID) {
		 throw std::runtime_error("Memory allocation failed for ID");
	 }
	 strcpy(new_obj->pID, pNewID); 
	 new_obj->pNext = NULL; 

	 // If the list of objects is empty
	 if (current_header->pObject == NULL) {
		 current_header->pObject = new_obj; 
		 printf("Inserted %s first object in list %c \n", pNewID, *pNewID); 
		 return 1; // inserted
	 }

	 Object7* current_obj = (Object7*)current_header->pObject;
	 Object7* previous_obj = NULL; 

	 while (current_obj != NULL && strcmp(current_obj->pID, pNewID) < 0) {
		 previous_obj = current_obj; 
		 current_obj = current_obj->pNext; 
	 }

	 // Insert new object
	 if (previous_obj == NULL) {
		 new_obj->pNext = current_obj; 
		 current_header->pObject = new_obj; 
	 }
	 else {
		 // Insert at the correct position (middle or end) 
		 previous_obj->pNext = new_obj;
		 new_obj->pNext = current_obj; 
	 }

	 printf("Inserted %s into list %c\n", pNewID, *pNewID); 

	 return 1; // insertion succeeded
 }

 Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD) {
	

	 return 0; 
 }