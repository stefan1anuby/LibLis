#include <iostream>
#include <string>

/*
TODO:
	adaugat empty list la meniu
	reparat delete sa nu mai dea eroare cand e pe ultimu nod
	terminat meniu
*/


class node {
public:
	std::string data;
	node* next;
	node* previous;
};

class List {
public:
	node* first;
	node* last;
	unsigned int length;
public:
	//List();
	void emptyList();
	void addNode(std::string, unsigned int);
	void printList(std::string dataType);
	void readFromFile(std::string dataType);
	void printListReverse();
	void deleteNode(unsigned int);
};

class ListSLL : public List {
private:
	List L;
public:
	ListSLL();
	//void printList();
};

class ListDLL : public List {
private: 
	List L;
public:
	ListDLL();
	//void printList();
};

class ListStack : public List {
private:
	List L;
public:
	ListStack();
	void push(std::string data);
	void pop();
	void printStack();
};

class ListQueue : public List {
private:
	List L;
public:
	ListQueue();
	void push(std::string data);
	void pop();
	void printQueue();
};



//int main()
//{
//	enum DataType { ll, dll, s, q };
//	DataType datatype;
//	std::string data;
//	unsigned int position;
//	List L;
//	//L.emptyList();
//	std::cout << "Select data type (l - linked list | d - doubly linked list | s - stack | q - queue | x - exit)\n";
//	char command;
//	std::cin >> command;
//	switch (command) {
//	case 'l':
//		std::cout << "selected linked list";
//		datatype = ll;
//		break;
//	case 'd':
//		std::cout << "selected doubly linked list";
//		datatype = dll;
//		break;
//	case 's':
//		std::cout << "selected stack";
//		datatype = s;
//		break;
//	case 'q':
//		std::cout << "selected queue";
//		datatype = q;
//		break;
//	}
//	while (true) {
//		switch (datatype) {
//		case ll:
//			std::cout << "\nAvailable commands: a - add node | d - delete node | p - print list | r - print list reverse | x - exit\n";
//			std::cin.ignore();
//			std::cin >> command;
//			switch (command) {
//			case 'a':
//				std::cout << "\nData to be added: ";
//				std::cin.ignore();
//				std::getline(std::cin, data);
//				std::cout << "\nNode position: ";
//				std::cin >> position;
//				L.addNode(data, position);
//				break;
//			case 'd':
//				std::cout << "\nNode to be deleted: ";
//				std::cin >> position;
//				L.deleteNode(position);
//				break;
//			case 'p':
//				L.printList();
//				break;
//			}
//		case 'r':
//			L.printListReverse();
//			break;
//		}
//	}
//
//	/*if (GetAsyncKeyState(0x4C) & 0x8000) {
//		std::cout << "Selected linked list";
//	}
//	if (GetAsyncKeyState(0x44) & 0x8000) {
//		std::cout << "Selected doubly linked list";
//	}
//	if (GetAsyncKeyState(0x53) & 0x8000) {
//		std::cout << "Selected stack";
//	}
//	if (GetAsyncKeyState(0x51) & 0x8000) {
//		std::cout << "Selected queue";
//	}*/
//	/*std::string input;
//	unsigned node_id;
//	std::cout << "Data to be added: ";
//	std::getline(std::cin, input);
//	std::cout << "Node position: ";
//	std::cin >> node_id;
//	addNode(L, input, node_id);
//	printList(L);*/
//	/*L.addNode("ASDF", 0);
//	L.addNode("ghjk", 1);
//	L.addNode("xzcv", 2);
//	L.addNode("bnm", 3);
//	L.addNode("154", 4);
//	L.addNode("722", 5);
//	L.addNode("15", 9);
//	L.deleteNode(4);
//	L.deleteNode(2);
//	L.deleteNode(0);
//	L.addNode("aloalao", 0);
//	L.addNode("837246", 4);
//	L.printList();
//	std::cout << L.last->data << " ";
//	std::cout << L.length << "\n";
//	L.printListReverse();
//	std::cout << L.last->data << " ";
//	std::cout << L.length << "\n";*/
//}