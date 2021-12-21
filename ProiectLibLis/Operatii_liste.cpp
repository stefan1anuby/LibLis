#include "Operatii_liste.h";

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
void Stack::push(std::string data) {
	L.addNode(data, 0);
}
void Queue::push(std::string data) {
	L.addNode(data, L.length);
}

void List::deleteNode(unsigned int node_id) {
	if (node_id > length && first == NULL)
		node_id = 0;
	unsigned int current_node = 0;
	node* iterator = first;
	node* prev = NULL;
	node* temp = iterator;
	if (temp != NULL && node_id == 0) {
		temp->next->previous = NULL;
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
void Stack::pop() {
	L.deleteNode(0);
}
void Queue::pop() {
	L.deleteNode(0);
}

void List::printList() {
	node* iterator = first;
	unsigned int current_node = 0;
	while (iterator != NULL) {
		std::cout << "[" << current_node << "]" << iterator->data << "\n";
		iterator = iterator->next;
		current_node++;
	}
}

void List::printListReverse() {
	node* iterator = last;
	unsigned int current_node = length - 1;
	while (iterator != NULL) {
		std::cout << "[" << current_node << "]" << iterator->data << "\n";
		iterator = iterator->previous;
		current_node--;
	}
}