#include <iostream>
#include <cstdlib>

using namespace std;

struct element
{
	int number;
	char *string;
	struct element *next;
	struct element *prev;
};


struct linkedList
{
	size_t size;
	element *head;
	element *tail;
};

linkedList* createList()
{
	linkedList *tmp = (linkedList*)malloc(sizeof(linkedList));
	tmp->size = 0;
	tmp->head = tmp->tail = NULL;
	return tmp;
}

void deleteList(linkedList **list)
{
	element *tmp = (*list)->head;
	element *next = NULL;
	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	free(*list);
	(*list) = NULL;
}

void pushFront(linkedList *list, int num, char *str)
{
	element *tmp = (element*)malloc(sizeof(element));
	if (tmp == NULL)
	{

	}
	else
	{
		tmp->number = num;
		tmp->string = str;
		tmp->next = list->head;
		tmp->prev = NULL;
		if (list->head)
		{
			list->head->prev = tmp;
		}
		list->head = tmp;
		if (list->tail == NULL)
		{
			list->tail = tmp;
		}
		list->size++;
	}
}

void pushBack(linkedList *list, int num, char *str)
{
	element *tmp = (element*)malloc(sizeof(element));
	if (tmp == NULL)
	{

	}
	else
	{
		tmp->number = num;
		tmp->string = str;
		tmp->prev = list->tail;
		tmp->next = NULL;
		if (list->tail)
		{
			list->tail->next = tmp;
		}
		list->tail = tmp;
		if (list->head == NULL)
		{
			list->head = tmp;
		}
		list->size++;
	}
}

void popFront(linkedList *list)
{
	element *prev;
	if (list->head == NULL)
	{

	}
	prev = list->head;
	list->head = list->head->next;
	if (list->head)
	{
		list->head->prev = NULL;
	}
	if (prev == list->tail)
	{
		list->tail = NULL;
	}
	free(prev);
	list->size--;
}

void popBack(linkedList *list)
{
	element *next;
	if (list->head == NULL)
	{

	}
	next = list->tail;
	list->tail = list->tail->prev;
	if (list->tail)
	{
		list->tail->next = NULL;
	}
	if (next == list->head)
	{
		list->head = NULL;
	}
	free(next);
	list->size--;
}

element* getNode(linkedList *list, size_t index)
{
	element *tmp = NULL;
	size_t i;
	if (index < list->size / 2)
	{
		i = 0;
		tmp = list->head;
		while (tmp && i < index)
		{
			tmp = tmp->next;
			i++;
		}
	}
	else
	{
		i = list->size - 1;
		tmp = list->tail;
		while (tmp && i > index)
		{
			tmp = tmp->prev;
			i--;
		}
	}
	return tmp;
}

void pushNode(linkedList *list, size_t index, int num, char * str)
{
	element *node = NULL;
	element *push = NULL;
	node = getNode(list, index);
	if (node == NULL)
	{

	}
	else
	{
		push = (element*)malloc(sizeof(element));
		push->number = num;
		push->string = str;
		push->prev = node;
		push->next = node->next;
		if (node->next)
		{
			node->next->prev = push;
		}
		node->next = push;
		if (!node->prev)
		{
			list->head = node;
		}
		if (!node->next)
		{
			list->tail = node;
		}
		list->size++;
	}
}

void popNode(linkedList *list, size_t index)
{
	element *node = NULL;
	node = getNode(list, index);
	if (node == NULL)
	{

	}
	else
	{
		if (node->prev)
		{
			node->prev->next = node->next;
		}
		if (node->next)
		{
			node->next->prev = node->prev;
		}
		if (!node->prev)
		{
			list->head = node->next;
		}
		if (!node->next)
		{
			list->tail = node->prev;
		}
		free(node);
		list->size--;
	}
}



int main()
{
	int a = 1;
	char sa[] = "GOOGLE";
	int b = 15;
	char sb[] = "HELLOWORLD";

	linkedList *list = createList();



	pushFront(list, a, sa);
	pushBack(list, b, sb);
	pushFront(list, a, sb);

	pushNode(list, 2, a, sa);
	popNode(list, 1);
	deleteList(&list);
	return 0;
}


