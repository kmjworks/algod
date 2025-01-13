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
int insertNewObject(HeaderD** pStruct7, char* pNewID, int NewCode);
Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD);
Node* CreateBinaryTree(HeaderD* pStruct7);
void TreeTraversal(Node* pTree);
Node* DeleteTreeNode(Node* pTree, unsigned long int Code);

time_t raw_time;




int main(void) {

	HeaderD* pStruct = GetStruct7(7, OBJECTS_NUM);
	HeaderD** test = &pStruct;

	Node* binary_tree = CreateBinaryTree(pStruct);

	TreeTraversal(binary_tree);

	char newid0[] = "Axbfgb"; 
	insertNewObject(test, newid0, 9431231); 

	binary_tree = CreateBinaryTree(*test); 
	TreeTraversal(binary_tree);

	
		
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
	Object7* removedobj = RemoveExistingObject(test, idToRemove);
	char idToRemove2[] = "Aasers";
	Object7* removedobj2 = RemoveExistingObject(test, idToRemove2);
	char idToRemove3[] = "DoesntExist";

	PrintObjects(pStruct);
	Object7* removedobj3 = RemoveExistingObject(test, idToRemove3);



	




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


// Generate a binary tree from all the objects in HeaderD
Node* CreateBinaryTree(HeaderD* pStruct7) {
	// If it's an empty list, return NULL
	if (pStruct7 == NULL) {
		return NULL;
	}

	// allocate a node for the tree's root
	Node* root = (Node*)malloc(sizeof(Node));
	if (root == NULL) {
		return NULL;
	}

	// root node is the first object 
	root->pObject = pStruct7->pObject;
	root->pLeft = root->pRight = NULL;

	Stack* stack = NULL;


	// start tree creation with the next header - already accounted for the first one in the root node
	HeaderD* current_header = pStruct7->pNext;



	while (current_header || stack) {
		// go through each header and push the header's first object onto the stack, then move onto the next header
		while (current_header) {
			
			Stack* new_stack = (Stack*)malloc(sizeof(Stack));
			if (new_stack == NULL) {
				return root; // or handle error
			}


			// the fist object of the header goes on top of the stack
			new_stack->pObject = current_header->pObject;
			new_stack->pNext = stack;
			stack = new_stack;

			current_header = current_header->pNext;
		}

		// If there aren't any headers anymore, pop headers from the stack 
		if (stack) {
			Stack* temp_helper = stack;
			void* current_object = temp_helper->pObject;

			stack = stack->pNext;
			free(temp_helper);

			// Create a new tree node
			Node* new_node = (Node*)malloc(sizeof(Node));
			if (new_node == NULL) {
				return root; 
			}
			new_node->pObject = current_object;
			new_node->pLeft = new_node->pRight = NULL;

			// insertion by alphabetical sorting (object id)
			Node* parent_node = root;
			Node* current_node = root;
			char* current_object_id = ((Object7*)current_object)->pID;

			while (current_node) {
				parent_node = current_node;
				char* parent_object_id = ((Object7*)parent_node->pObject)->pID;

				if (strcmp(current_object_id, parent_object_id) < 0) {
					current_node = current_node->pLeft;
				}
				else {
					current_node = current_node->pRight;
				}
			}
			// Attach the new node
			char* parent_object_id = ((Object7*)parent_node->pObject)->pID;
			if (strcmp(current_object_id, parent_object_id) < 0) {
				parent_node->pLeft = new_node;
			}
			else {
				parent_node->pRight = new_node;
			}
			

			// process the rest of the list for the object that we popped from the stack if there are any
			// so if the object has a pointer to the next object, push that onto the stack
			if (((Object7*)current_object)->pNext) {
				Stack* new_stack = (Stack*)malloc(sizeof(Stack));
				if (new_stack == NULL) {
					return root;
				}
				new_stack->pObject = ((Object7*)current_object)->pNext;
				new_stack->pNext = stack;
				stack = new_stack;
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
	/*
	   Iterator for keeping count of the inputs so that node objects can be labeled
	   accordingly and printed out in a more readable format in case I need to debug anything
	*/
	int output_counter = 1;

	//  While the stack is not empty, traverse through the nodes
	while (current_node || stack) {

		// Travel to the leftmost node
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

		// Pop from the stack
		if (stack) {
			
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


Node* DeleteTreeNode(Node* pTree, unsigned long int Code) {
	if (pTree == NULL) {
		return pTree; 
	}

	Node* root = pTree;
	Node* parent_node = NULL;
	Node* current_node = root;

	/*
	   Search the tree until a matching object with the code is found or until there are no objects left to search for
	*/
	while (current_node && ((Object7*)current_node->pObject)->Code != NULL) {
		parent_node = current_node;
		if (((Object7*)current_node->pObject)->Code > Code) {
			current_node = current_node->pLeft;
		}
		else {
			current_node = current_node->pRight;
		}
	}

	if (current_node == NULL) {
		return root;
	}

	/*
	   This handles a scenario where we find the node but it has no children nodes
	 */

	if (current_node->pLeft == NULL && current_node->pRight == NULL) {
		if (current_node != root) {
			// In this case its pretty simple, just detach the node from the parent node
			if (parent_node->pLeft == current_node) {
				parent_node->pLeft = NULL;
			}
			else {
				parent_node->pRight = NULL;
			}
			free(current_node);
		}
		else
		{
			free(root);
			pTree = NULL; // empty tree if root is removed
		}
	}

	/*
	   In this scenario, the node has only one child node
	*/

	else if (current_node->pLeft == NULL || current_node->pRight == NULL) {
		Node* child = current_node->pLeft ? current_node->pLeft : current_node->pRight;

		if (current_node != root) {
			if (parent_node->pLeft == current_node) {
				parent_node->pLeft = child;
			}
			else {
				parent_node->pRight = child;
			}
			free(current_node);
		}
		else {
			free(root);
			pTree = child;
		}
	}

	/*
	Moving on we need to handle a scenario
	where the node which needs to be deleted has two children nodes
	*/

	else {
		// Find the smallest node in the right subtree 
		Node* successor_parent_node = current_node;
		Node* successor_node = current_node->pRight;


		while (successor_node->pLeft) {
			successor_parent_node = successor_node;
			successor_node = successor_node->pLeft;
		}

		if (successor_parent_node != current_node) {
			successor_parent_node->pLeft = successor_node->pRight;
		}
		else {
			successor_parent_node->pRight = successor_node->pRight;
		}

		// move the successor's pointer to the current node's pointer eventually replacing it
		current_node->pObject = successor_node->pObject;
		free(successor_node);
	}

	return pTree;
}