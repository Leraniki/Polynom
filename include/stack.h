#ifndef __List_H__
#define __List_H__

#include <iostream>

using namespace std;


template<typename T>
class List
{
protected:
	struct Node {
		T data;
		Node* next;
		Node(const T& value, Node* next_node = nullptr) : data(value), next(next_node) {}
	};
	Node* first;

public:
	List() : first(nullptr) {}

	List(int n, T deflt = T()) {
		if (n == 0)
			throw 1;
		first = new Node(deflt, nullptr);
		Node* current = first;
		for (int i = 0; i < n - 1; i++) {
			Node* tmp = new Node(deflt, nullptr);
			current->next = tmp;
			current = current->next;
		}

	}
	List(const List& other) {
		if (!other.first) {
			first == nullptr;
			throw 1;
		}
		this-> first = new Node(other.first->data, nullptr);
		Node* current = first;
		Node* ocurrent = other.first;
		while (ocurrent->next) {
			current->next = new Node(ocurrent->next->data, nullptr);
			current = current->next;
			ocurrent = ocurrent->next;
		}

	}

	void clear() {
		while (first) {
			Node* tmp = first;
			first = first->next;
			delete tmp;
		}
		first = nullptr; 
	}
	
	~List() {
		clear();
	}

	List& operator=(const List& other) {
		if (this == &other)
			return *this;
		clear();
		if (other.first) {
			first = new Node(other.first->data);
			Node* current = first;
			Node* other_curr = other.first->next;
			while (other_curr) {
				current->next = new Node(other_curr->data);
				current = current->next;
				other_curr = other_curr->next;
			}
		}
		return *this;
	}

	void print() {
		Node* current = first;
		while (current != nullptr) {
			std::cout << current->data << " ";
			current = current->next;
		}
	}

	size_t size() { //O(n)
		int size = 0;
		Node* current = first;
		while (current) {
			size++;
			current = current->next;
		}
		return size;
	}

	Node* insert(T value, Node* prev) {
		Node* tmp = new Node(T(), nullptr);
		tmp->next = prev->next;
		tmp->data = value;
		prev->next = tmp;
		return tmp;
	}

	Node* insert_front(T value) {
		Node* tmp = new Node(T(), nullptr);
		tmp->next = first;
		tmp->data = value;
		first = tmp;
		return first;

	}

	Node* erase(Node* prev) {
		Node* tmp = prev->next;
		if (tmp == nullptr) 
			throw 1;

		prev->next = tmp->next;
		delete tmp;
		return prev->next;
	}

	Node* erase_front() {
		Node* tmp = first;
		if (!first)
			throw 1;
		first = tmp->next;
		delete tmp;
		return first;
	}

	T& operator[](int index) { //O(n)
		if (index < 0)
			throw 1;
		Node* curr = first;
		for (int i = 0; i < index; i++) {
			if (!curr) {
				throw 1;
			}
			curr = curr->next;
		}
			
		return curr->data;
	}


	Node* find(T value) {
		Node* current = first;
		while (current != nullptr) {
			if (current->data == value) {
				return current;
			}
			current = current->next;
		}
		return nullptr;
	}

	Node* get_first() {
		return first;
	}

	//задача
	void del_mid_elem() {
		if (first == nullptr || first->next == nullptr) {
			return; 
		}

		Node* slow = first;
		Node* fast = first;
		Node* prev = nullptr;

		while (fast != nullptr && fast->next != nullptr) {
			fast = fast->next->next;
			prev = slow;
			slow = slow->next;
		}

		

		if (prev != nullptr) {
			prev->next = slow->next;
			delete slow;

			if (fast != nullptr) {
				slow = prev->next;
				if (slow != nullptr) { 
					prev->next = slow->next;
					delete slow;
				}
			}
		}
		else {
			
			first = slow->next;
			delete slow;
		}
	}

	//Iterator
	class Iterator {
	protected:
		Node* curr;
	public:
		Iterator(Node* node) : curr(node) {}

		Iterator& operator++() {
			curr = curr->next;
			return *this;
		}

		Iterator operator++(int) {
			Iterator copy = *this;
			curr = curr->next;
			return copy;
		}

		T& operator* () {
			return curr->data;
		}

		T& operator->() {
			return &(curr->data);
		}

		friend bool operator !=(const Iterator& it1, const Iterator& it2) {
			return it1.curr != it2.curr;
		}

		
	};
	
	Iterator begin() {
		return Iterator(first);

	}

	Iterator end() {
		return Iterator(nullptr);

	}
	

};

#endif
