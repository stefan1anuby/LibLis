#define _CRT_SECURE_NO_WARNINGS

// Vlad: functiile de prelucrare a valorilor din liste
#include "Operatii_liste.h";

// creare / golire lista
void List::emptyList() {
	/*first = NULL;
	last = NULL;
	length = 0;*/
	while (first != NULL)
		deleteNode(0);
}
ListSLL::ListSLL() {
	L.emptyList();
}
ListDLL::ListDLL() {
	L.emptyList();
}
ListStack::ListStack() {
	L.emptyList();
}
ListQueue::ListQueue() {
	L.emptyList();
}

//adaugare noduri in lista
void List::addNode(std::string data, unsigned int node_id) {
	node* newnode = new node;
	newnode->data = data;
	if (node_id > length && first == NULL)
		node_id = 0;
	if (node_id <= 0) {
		if (first == NULL && last == NULL) {
			newnode->next = NULL;
			newnode->previous = NULL;
			first = newnode;
			last = newnode;
			length = 1;
		}
		else {
			first->previous = newnode;
			newnode->next = first;
			first = newnode;
			length++;
		}
	}
	else {
		/*if (node_id == (L.length + 1)) {
			std::cout << "here";
			newnode->next = NULL;
			L.last = newnode;
			L.length++;
		}*/
		unsigned int current_node = 0;
		node* iterator = first;
		node* prev = NULL;
		while (iterator != NULL && current_node != node_id) {
			prev = iterator;
			iterator = iterator->next;
			current_node++;
		}
		/*if (prev == NULL) {
			prev = iterator;
		}*/
		newnode->previous = prev;
		prev->next = newnode;
		newnode->next = iterator;
		if (iterator != NULL)
			iterator->previous = newnode;
		if (node_id >= length) {
			newnode->next = NULL;
			last = newnode;
		}
		/*if (iterator->next == NULL) {
		L.last = iterator;
		}*/
		length++;
	}
}
void ListStack::push(std::string data) {
	L.addNode(data, 0);
}
void ListQueue::push(std::string data) {
	L.addNode(data, L.length);
}

//stergere noduri din lista
void List::deleteNode(unsigned int node_id) {
	if (node_id > length && first == NULL)
		node_id = 0;
	unsigned int current_node = 0;
	node* iterator = first;
	node* prev = NULL;
	node* temp = iterator;
	if (temp != NULL && node_id == 0) {
		if (temp->next != NULL)
			temp->next->previous = NULL;
		else
			last = temp->next;
		first = temp->next;
		delete temp;
		length--;
	}
	else {
		while (temp != NULL && current_node != node_id) {
			prev = temp;
			temp = temp->next;
			current_node++;
		}
		if (temp == NULL) return;
		prev->next = temp->next;
		if (temp->next != NULL)
			temp->next->previous = prev;
		else {
			temp->previous = prev;
			last = temp->previous;
		}
		delete temp;
		length--;
	}
}
void ListStack::pop() {
	L.deleteNode(0);
}
void ListQueue::pop() {
	L.deleteNode(0);
}

//scrie listele in fisier
void List::printList(std::string dataType) {
	FILE* output;
	node* iterator = first;
	std::string filePath;
	char* charfilePath;
	unsigned int current_node = 0;

	if (dataType == "sll")	filePath = "./sll.txt";
	if (dataType == "dll")	filePath = "./dll.txt";
	if (dataType == "s")	filePath = "./stack.txt";
	if (dataType == "q")	filePath = "./queue.txt";
	
	charfilePath = &filePath[0];
	output = fopen(charfilePath, "w");

	while (iterator != NULL) {
		//stringSize = iterator->data.length();
		char* buffer;
		buffer = &iterator->data[0];
		fputs(buffer, output);
		fputs(",", output);
		//std::cout << "[" << current_node << "]" << iterator->data << "\n";
		iterator = iterator->next;
		current_node++;
	}

	fclose(output);
}

//void ListSLL::printList() {
//	L.printList("sll");
//}
//void ListDLL::printList() {
//	L.printList("dll");
//}
void ListStack::printStack() {
	L.printList("s");
}
void ListQueue::printQueue() {
	L.printList("q");
}

//citeste listele din fisier
void List::readFromFile(std::string dataType) {
	FILE* input;
	int read;
	std::string value, filePath;
	char* charFilePath;
	
	if (dataType == "sll")	filePath = "./sll.txt";
	if (dataType == "dll")	filePath = "./dll.txt";
	if (dataType == "s")	filePath = "./stack.txt";
	if (dataType == "q")	filePath = "./queue.txt";

	charFilePath = &filePath[0];
	input = fopen(charFilePath, "r");

	if (input == NULL)	{ std::cout << "file does not exist"; return; }

	do {
		read = fgetc(input);
		if (read != ',') value += read;
		else {
			addNode(value, length);
			//std::cout << value << " ";
			value = "";
		}
	} while (read != EOF);
	fclose(input);
}

void List::printListReverse() {
	node* iterator = last;
	unsigned int current_node = length - 1;
	if (iterator == NULL) std::cout << "List is empty.";
	while (iterator != NULL) {
		std::cout << "[" << current_node << "]" << iterator->data << "\n";
		iterator = iterator->previous;
		current_node--;
	}
}