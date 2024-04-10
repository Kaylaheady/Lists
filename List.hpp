#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <initializer_list>
#include "List.h"


template<typename T>
class List {
private:
    struct Node {
        T data;
        Node *prev;
        Node *next;
        Node(const T & d = T{}, Node *p = nullptr, Node *n = nullptr)
                : data{ d }, prev{ p }, next{ n } {}
    };

    Node *head;
    Node *tail;
    size_t theSize;


public:
    List() : head(nullptr), tail(nullptr), theSize(0) {}


    List(const List &rhs) {
        head = nullptr; // Initialize head for the new list

        Node* temp = rhs.head; // Start from the head of the other list
        while (temp != nullptr) {
            push_back(temp->data); // Copy each element
            temp = temp->next;
        }
    }


    List(List &&rhs) noexcept {
        head = rhs.head;
        rhs.head = nullptr;
    }
    List(int num, const T &val = T()) {
        head = nullptr;
        for (int i = 0; i < num; ++i)
            push_back(val);
    }


    List(typename List<T>::const_iterator start, typename List<T>::const_iterator end) {
        head = nullptr;
        for (auto it = start; it != end; ++it)
            push_back(*it);
    }

    List& operator=(std::initializer_list<T> iList) {
        while (head != nullptr) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        theSize = 0;


        for (const auto& element : iList) {
            push_back(element);
        }

        return *this;
    }
    size_t size() const {
        return theSize;
    }

    bool empty() const {
        return theSize == 0;
    }
    void clear() {
        while (head != nullptr) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        theSize = 0;
    }
    void reverse() {
        Node *current = head;
        Node *temp = nullptr;
        while (current != nullptr) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        if (temp != nullptr) {
            head = temp->prev;
        }
    }
    T& front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return head->data;
    }


    T& back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return tail->data;
    }
    void push_front(const T& value) {
        Node *newNode = new Node(value, nullptr, head);
        if (head != nullptr) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
        theSize++;
    }


    void push_front(T&& value) {
        Node *newNode = new Node(std::move(value), nullptr, head);
        if (head != nullptr) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
        theSize++;
    }


    void push_back(const T& value) {
        Node *newNode = new Node(value, tail, nullptr);
        if (tail != nullptr) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
        theSize++;
    }


    void push_back(T&& value) {
        Node *newNode = new Node(std::move(value), tail, nullptr);
        if (tail != nullptr) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
        theSize++;
    }


    void pop_front() {
        if (head != nullptr) {
            Node *temp = head;
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            delete temp;
            theSize--;
        }
    }


    void pop_back() {
        if (tail != nullptr) {
            Node *temp = tail;
            tail = tail->prev;
            if (tail != nullptr) {
                tail->next = nullptr;
            } else {
                head = nullptr;
            }
            delete temp;
            theSize--;
        }
    }


    void remove(const T& val) {
        if (empty()) {
            throw std::logic_error("List is empty, cannot remove elements");
        }

        Node* current = head;
        while (current != nullptr) {
            if (current->data == val) {
                Node* temp = current;
                if (current->prev != nullptr) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current->next != nullptr) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
                current = current->next;
                delete temp;
                theSize--;
            } else {
                current = current->next;
            }
        }
    }

    void print(std::ostream &os, char ofc = ' ') const {
        Node *current = head;
        while (current != nullptr) {
            os << current->data << ofc;
            current = current->next;
        }
    }
    iterator begin() {
        return iterator(head);
    }

    const_iterator begin() const {
        return const_iterator(head);
    }

    iterator end() {
        return iterator(tail->next);
    }

    const_iterator end() const {
    iterator insert(iterator itr, const T & val) {
        Node *p = itr.current;
        if (p == nullptr)
            return end();

        Node *newNode = new Node(val, p->prev, p);
        if (p->prev != nullptr)
            p->prev->next = newNode;
        else
            head = newNode;
        p->prev = newNode;
        theSize++;
        return iterator(newNode);
    }

    iterator insert(iterator itr, T && val) {
        Node *p = itr.current;
        if (p == nullptr)
            return end();

        Node *newNode = new Node(std::move(val), p->prev, p);
        if (p->prev != nullptr)
            p->prev->next = newNode;
        else
            head = newNode;
        p->prev = newNode;
        theSize++;
        return iterator(newNode);
    }
    iterator erase(iterator itr) {
        Node *p = itr.current;
        if (p == nullptr)
            return end();

        iterator retVal(p->next);
        if (p->prev != nullptr)
            p->prev->next = p->next;
        else
            head = p->next;
        if (p->next != nullptr)
            p->next->prev = p->prev;
        else
            tail = p->prev;
        delete p;
        theSize--;
        return retVal;
    }

    iterator erase(iterator start, iterator end) {
        Node *p = start.current;
        if (p == nullptr || end.current == nullptr || start == end)
            return end();

        if (p->prev != nullptr)
            p->prev->next = end.current;
        else
            head = end.current;
        end.current->prev = p->prev;

        while (p != end.current) {
            Node *temp = p;
            p = p->next;
            delete temp;
            theSize--;
        }
        return end;
    }
    List& operator=(const List& rhs) {
        if (this != &rhs) { // Check for self-assignment
            // Clear the current contents of the list
            clear();

            // Copy the elements from rhs
            for (const auto& element : rhs) {
                push_back(element);
            }
        }
        return *this;
    }

    List& operator=(List&& rhs) noexcept {
        if (this != &rhs) {

            clear();


            head = rhs.head;
            tail = rhs.tail;
            listSize = rhs.listSize;


            rhs.head = nullptr;
            rhs.tail = nullptr;
            rhs.listSize = 0;
        }
        return *this;
    }


};
template<typename T>
bool operator==(const List<T> & lhs, const List<T> & rhs) {
    // Check if the lists have the same size
    if (lhs.size() != rhs.size()) {
        return false;
    }
    template<typename T>
    bool operator!=(const List<T> & lhs, const List<T> & rhs) {
        return !(lhs == rhs);
    }

    // Iterate through the lists and compare elements
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (lhs_iter != lhs.end() && rhs_iter != rhs.end()) {
        if (*lhs_iter != *rhs_iter) {
            return false;  // Elements at corresponding positions are not equal
        }
        ++lhs_iter;
        ++rhs_iter;
    }

    // If both iterators reach the end, all elements are equal
    return true;
}



template<typename T>
class List<T>::iterator {
public:
    const_iterator() : current(nullptr) {}
    const T& operator*() const { return current->data; }
    const_iterator& operator++() { current = current->next; return *this; }
    const_iterator& operator--() { current = current->prev; return *this; }
    bool operator==(const const_iterator& rhs) const { return current == rhs.current; }
    bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }


protected:
    Node *current;
    const T & retrieve() const;
    const_iterator(Node *p);
    friend class List<T>;
};
template<typename T>
List<T>::~List() {
    // Delete all nodes in the list
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    // Set tail to nullptr to avoid dangling pointer
    tail = nullptr;
    // Reset list size to 0
    listSize = 0;
}

template<typename T>
class List<T>::const_iterator {
public:
    iterator();
    T & operator*();
    const T & operator*() const;

    iterator() : const_iterator() {}
    T& operator*() { return current->data; }
    iterator& operator++() { current = current->next; return *this; }
    iterator& operator--() { current = current->prev; return *this; }


protected:
    Node *current;
    iterator(Node *p);
    friend class List<T>;
    const T & retrieve() const;
};

public:
List();
List(const List & rhs);
List(List && rhs);
List(int num, const T & val = T());
List(const_iterator start, const_iterator end);
List(std::initializer_list<T> iList);
~List();

List & operator=(const List & rhs);
List & operator=(List && rhs);
List & operator=(std::initializer_list<T> iList);

int size() const;
bool empty() const;
void clear();
void reverse();
T & front();
const T & front() const;
T & back();
const T & back() const;
void push_front(const T & val);
void push_front(T && val);
void push_back(const T & val);
void push_back(T && val);
void pop_front();
void pop_back();
void remove(const T & val);

template<typename PREDICATE>
void remove_if(PREDICATE pred);
void print(std::ostream & os, char ofc = ' ') const;
iterator begin();
const_iterator begin() const;
iterator end();
const_iterator end() const;
iterator insert(iterator itr, const T & val);
iterator insert(iterator itr, T && val);
iterator erase(iterator itr);
iterator erase(iterator start, iterator end);

private:
int theSize;
Node *head;
Node *tail;


};

template<typename T>
const T& List<T>::const_iterator::operator*() const {
    return retrieve();
} //Implementation of operator*() for const_iterator
template<typename T>
T& List<T>::iterator::operator*() {
    return retrieve();
}//Implementation of operator*() for iterator

// Implementation of prefix increment operator++() for const_iterator
template<typename T>
typename List<T>::const_iterator& List<T>::const_iterator::operator++() {
    current = current->next;
    return *this;
}

template<typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int) {
    const_iterator old = *this;
    ++(*this);
    return old;
}
// Implementation of prefix decrement operator--() for const_iterator
template<typename T>
typename List<T>::const_iterator& List<T>::const_iterator::operator--() {
    current = current->prev;
    return *this;
}


template<typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int) {
    const_iterator old = *this;
    --(*this);
    return old;
}


template<typename T>
typename List<T>::iterator& List<T>::iterator::operator++() {
    current = current->next;
    return *this;
}


template<typename T>
typename List<T>::iterator List<T>::iterator::operator++(int) {
    iterator old = *this;
    ++(*this);
    return old;
}


template<typename T>
typename List<T>::iterator& List<T>::iterator::operator--() {
    current = current->prev;
    return *this;
}


template<typename T>
typename List<T>::iterator List<T>::iterator::operator--(int) {
    iterator old = *this;
    --(*this);
    return old;
}


template<typename T>
bool List<T>::const_iterator::operator==(const const_iterator & rhs) const {
    return current == rhs.current;
}


template<typename T>
bool List<T>::const_iterator::operator!=(const const_iterator & rhs) const {
    return !(*this == rhs);
}


template<typename T>
bool List<T>::iterator::operator==(const iterator & rhs) const {
    return current == rhs.current;
}


template<typename T>
bool List<T>::iterator::operator!=(const iterator & rhs) const {
    return !(*this == rhs);
}


template<typename T>
std::ostream & operator<<(std::ostream & os, const List<T> & l);

template<typename T>
typename List<T>::iterator List<T>::begin() {
    return iterator(head);
}

template<typename T>
typename List<T>::const_iterator List<T>::begin() const {
    return const_iterator(head);
}

template<typename T>
typename List<T>::iterator List<T>::end() {
    return iterator(tail->next);
}

template<typename T>
typename List<T>::const_iterator List<T>::end() const {
    return const_iterator(tail->next);
}


#endif