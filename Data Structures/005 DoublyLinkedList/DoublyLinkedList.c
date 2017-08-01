#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct StudentStruct {
	struct StudentStruct* prev;
	struct StudentStruct* next;
	DListNode list_node; //common header
	int id;
	char name[100];
	int age;
	char address[2000];
};

typedef struct StudentStruct Student;

void Student_Init(Student* s) {
	s->id = 0;
	s->name[0] = 0;
	s->age = 0;
	s->address[0] = 0;
	s->list_node.prev = NULL;
	s->list_node.next = NULL;
}

void Student_InitWithId(Student* s, int id, char* name) {
	Student_Init(s);
	s->id = id;
	strcpy(s->name, name);
}

Student* Student_NewWithId(int id, char* name) {
	Student* s = (Student*)malloc(sizeof(Student));
	Student_InitWithId(s, id, name);
	return s;
}

typedef struct {
	DListNode* head;
	DListNode* tail;
	int count;
} DList;


void DList_Init(DList* ls) {
	ls->head = NULL;
	ls->tail = NULL;
	ls->count = 0;
}

void DList_InsertToHead(DList* ls, DListNode* s) {
	assert(!s->prev && !s->next);

	s->prev = NULL;
	s->next = ls->head; //1

	if (s->next) {
		s->next->prev = s;
	}
	
	if (!ls->head) {
		ls->tail = s; //update tail as well, if the list is empty
	}

	ls->head = s;
	ls->count++;
}

void DList_Insert(DList* ls, DListNode* s, DListNode* after) {
	assert(!s->prev && !s->next);

	if (!after) {
		DList_InsertToHead(ls, s);
		return;
	}

	s->prev = after; //1
	s->next = after->next; //2

	if (after->next) {
		after->next->prev = s; //3
	} else {
		ls->tail = s; //3
	}

	after->next = s; //4
	
	ls->count++;
}

void DList_AppendToTail(DList* ls, DListNode* s);
void DList_Append(DList* ls, DListNode* s, DListNode* before);

void DList_Release(DList* ls) {
	DListNode* next;
	for (DListNode* p = ls->head; p; p = next) {
		next = p->next;
		free(p);
	}
	ls->count = 0;
}

typedef DList StudentList;

void StudentList_Insert(StudentList* ls, Student* s, Student* after) {
	DListNode* a = NULL;
	if (after)
		a = after->list_node;
	DList_Insert((DList*)ls, &s->list_node, a);
}

void StudentList_Init(StudentList* ls) {
	DList_Init((DList*)ls);
}

void StudentList_Release(StudentList* ls) {
	DList_Release((DList*)ls);
}

void StudentList_Print(DList* ls) {
	printf("\nprint list count=%d\n", ls->count);
	for (DListNode* p = ls->head; p; p = p->next) {
		Student* s = (Student*)p;
		printf("  %d: %s\n", s->id, s->name);
	}
	printf("\n");
}


int main() {
	StudentList ls;
	StudentList_Init(&ls);

	Student* john = Student_NewWithId(10, "John");
	StudentList_Insert(&ls, john, NULL);

	Student* mary = Student_NewWithId(44, "Mary");
	StudentList_Insert(&ls, mary, NULL);

	Student* tom = Student_NewWithId(6, "Tom");
	StudentList_Insert(&ls, tom, NULL);

	StudentList_Print(&ls);

	Student* bob = Student_NewWithId(36, "Bob");
	StudentList_Insert(&ls, bob, john);

	StudentList_Print(&ls);

	StudentList_Release(&ls);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}
