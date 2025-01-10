#include "stdio.h"
#include "stdlib.h"
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
Node* CreateBinaryTree(HeaderD* pStruct7); 
void TreeTraversal(Node* pTree);

time_t raw_time; 


int main(void) {

	HeaderD* pStruct = GetStruct7(7, OBJECTS_NUM);
	HeaderD** test = &pStruct;

	Node* binary_tree = CreateBinaryTree(pStruct);
	TreeTraversal(binary_tree);

	/*
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

		char newid10[] = "wksad";
		insertNewObject(test, newid10, 32225641); // Returns error, invalid format

		char newid11[] = "Wa";
		insertNewObject(test, newid11, 32125641); // ID Already exists - returns error

		char newid12[] = "W8serad";
		insertNewObject(test, newid12, 32525641); // Invalid non-alphabetic character 8 - returns error

		char newid13[] = "W_serad";
		insertNewObject(test, newid13, 32525641); // Invalid non-alphabetic character _ - returns error

		
		
		PrintObjects(pStruct);
		char idToRemove[] = "Zweas";
		Object7* removedObject = RemoveExistingObject(test, idToRemove);
		char idToRemove2[] = "Aasers";
		Object7* removedObject2 = RemoveExistingObject(test, idToRemove2);

		char idToRemove3[] = "DoesntExist";

		PrintObjects(pStruct);
		Object7* removedObject3 = RemoveExistingObject(test, idToRemove3);
		
	
		
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Caught exception : " << e.what() << std::flush; 
	}
	*/

	
	
	
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
 /* 
	Each malloc() call is checked whether a valid pointer is returned or not. 
	Although this is a C program some utilities such as throws and runtime_errors are used
	which would be bulkier to implement in pure C

	Core functionality is implemented in pure C (data types)
 */
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
		 previous_header = current_header; // Set the previous header list to point to the current header
		 current_header = current_header->pNext; // Skip to the next header list
	 }

	 // If doesn't exist, create a new HeaderD node
	 if (current_header == NULL || current_header->cBegin != *pNewID) {
		 HeaderD* new_header = (HeaderD*)malloc(sizeof(HeaderD)); 
		 if (!new_header) throw std::runtime_error("Memory allocation failed"); 
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
	 if (!new_obj->pDate2) throw std::runtime_error("Memory allocation failed for pDate2"); 
	 GetDate2(time(&raw_time), new_obj->pDate2); 

	 new_obj->pID = (char*)malloc(strlen(pNewID) + 1); // Account for '\0' operator
	 if (!new_obj->pID) throw std::runtime_error("Memory allocation failed for ID");
	 
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
	 if (!pStruct7 || !*pStruct7) {
		 throw std::runtime_error("List is empty or the provided pointer is invalid"); 
	}

	 HeaderD* current_header = *pStruct7; 
	 Object7* previous_obj = NULL; 

	 while (current_header) {
		 Object7* current_obj = (Object7*)current_header->pObject; // First object 
		 previous_obj = NULL; // Reset previous object to NULL 

		 while (current_obj) {
			 if (strcmp(current_obj->pID, pExistingD) == 0) {
				 // Object located, remove it
				 if (previous_obj == NULL) {
					 current_header->pObject = current_obj->pNext; // Remove first object from list
				 }
				 else 
				 {
					 previous_obj->pNext = current_obj->pNext; 
				 }

				 printf("Removed object with pID : %s from list %c\n", pExistingD, current_header->cBegin); 
				 return current_obj; 
			 }
			 previous_obj = current_obj; 
			 current_obj = current_obj->pNext; // Move to the next object in the given list
		 }

		 current_header = current_header->pNext;  // Same for the header
	 }

	 // If the object was not found 
	 std::ostringstream errorMsg; 
	 errorMsg << "Error : object with pID : " << pExistingD << " not found..."; 
	 throw std::runtime_error(errorMsg.str()); 

	 return 0; 
 }

 Node* CreateBinaryTree(HeaderD* pStruct7) {
	 // If it's an empty list, return NULL
	 if (pStruct7 == NULL) {
		 return NULL; 
	 }

	 Node* root = (Node*)malloc(sizeof(Node));
	 if (root == NULL) {
		 return NULL; 
	 }

	 root->pObject = pStruct7->pObject; 
	 root->pLeft = root->pRight = NULL; 

	 Stack* stack = NULL;
	 HeaderD* current_header = pStruct7->pNext;

	 while (current_header || stack) {
		 while (current_header) {
			 Stack *new_stack = (Stack*)malloc(sizeof(Stack));
			 if (new_stack == NULL) {
				 return root; 
			 }

			 new_stack->pObject = current_header->pObject;
			 new_stack->pNext = stack;

			 stack = new_stack;
			 current_header = current_header->pNext;
		 }

		 // Pop the top-most object and process it 
		 if (stack) {
			 Stack* temp_helper = stack; 
			 void *current_object = temp_helper->pObject;

			 stack = stack->pNext;

			 free(temp_helper);

			 Node* new_node = (Node*)malloc(sizeof(Node));
			 if (new_node == NULL) {
				 return root; 
			 }

			 new_node->pObject = current_object;
			 new_node->pLeft = new_node->pRight = NULL;

			 // Node insertion 
			 Node* parent_node = root; 
			 Node* current_node = root; 

			 int current_object_code = ((Object7*)current_object)->Code;

			 while (current_node) {
				 parent_node = current_node; 

				 int parent_object_code = ((Object7*)current_node->pObject)->Code;

				 if(current_object_code < parent_object_code) {
					 current_node = current_node->pLeft; 
				 }
				 else {
					 current_node = current_node->pRight; 
				 }
			 }

			 int parent_object_code = ((Object7*)parent_node->pObject)->Code;
			 if(current_object_code < parent_object_code) {
				 parent_node->pLeft = new_node; 
			 }
			 else {
				 parent_node->pRight = new_node; 
			 }	
		 }
	 }

	 return root; 

 }

 void TreeTraversal(Node* pTree) {
	 if (pTree == NULL) {
		 return;
	 }

	 Stack* stack = NULL;

	 Node* current_node = pTree;
	 int output_counter = 1; // Output numbering iterator

	 //  While the stack is not empty, traverse through the nodes
	 while (current_node || stack) {
		 while (current_node) {
			 Stack* new_node = (Stack*)malloc(sizeof(Stack));
			 if (new_node == NULL) {
				 return;
			 }

			 new_node->pObject = current_node;
			 new_node->pNext = stack;
			 stack = new_node;

			 current_node = current_node->pLeft;
		 }

		 // Process the node that is the top-most node in the stack
		 if (stack) {
			 // Pop it :D 
			 Stack* temp_helper = stack;
			 current_node = (Node*)temp_helper->pObject;
			 stack = stack->pNext;

			 // Print the object info just as in the first function PrintObjects
			 Object7* current_object = (Object7*)current_node->pObject;
			 printf("%d) %s %lu %02d %s %04d\n", output_counter, current_object->pID, current_object->Code, current_object->pDate2->Day, current_object->pDate2->Month, current_object->pDate2->Year);
			 ++output_counter;

			 current_node = current_node->pRight;
			 free(temp_helper); 
		 }
	 }
 }
