/*Names: Kira Cibak and Maria Jose Perez
Description: For this project, we implemented the validateExpression, evaluateExpression, and covertToFloat functions. We also implemented the destructor.*/

#include "linked_calc.hpp"

template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory
template <typename T>
LinkedCalc<T>::~LinkedCalc() {

    Node<T>* curr=head;

    while(curr != nullptr){ //iterates through the list and deletes each node
        Node<T>* nxt=curr->next;
        delete curr;
        curr=nxt;
    }
}

// Function to insert a new node at the end of the linked list
template <typename T>
void LinkedCalc<T>::insert(const T& value) {

    Node<T>* newNode = new Node<T>(value);

    if (head == nullptr) {
        head = newNode;
    } 
    
    else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Helper function to determine if a character is a digit
template <typename T>
bool LinkedCalc<T>::isDigit(const T& c) {
    return (c >= '0' && c <= '9') ;
}

// Helper function to convert a sequence of digits to a float
template <typename T>
float LinkedCalc<T>::convertToFloat(Node<T>*& head) {

    if (head == nullptr) {
        return 0.0f; // handle the case of an empty list
    }

    // First, determine the length of the linked list
    int length = 0;

    Node<T>* current = head;

    while (current != nullptr) { //iterates through the list and counts the number of nodes
        length++;
        current = current->next;
    }

    // Allocate a char array to store the number string
    char* numberStr = new char[length + 1]; // +1 for null terminator
    current = head;
    int index = 0; // index to keep track of the current position in the char array

    // Traverse the linked list and copy characters to the char array
    while (current != nullptr) {

        numberStr[index++] = current->data;
        current = current->next;
    }

    numberStr[index] = '\0'; // Null-terminate the char array

    // Convert the char array to a floating point number
    char* end;
    float result = std::strtof(numberStr, &end);//converts the char array to a float, stopping at the operator

    // Clean up the allocated char array
    delete[] numberStr;

    return result;
}


template <typename T>
bool LinkedCalc<T>::validateExpression() { //checks if the expression is valid

    Node<T>* curr=head; //sets current to first node

    if(head==nullptr) //if list is empty
        return false;
    
    while(curr){ //iterates through the list until the end

        if(isDigit(curr->data)){ //checks if current node is a digit

            curr=curr->next; 
            continue; // moves to next iteration
        }

        if(curr->data=='+' || curr->data=='-' || curr->data=='*' || curr->data=='/' || curr->data=='.'){ //checks if current node is an operator or a decimal
            if(curr->next==nullptr || curr->next->data=='+' || curr->next->data=='-' || curr->next->data=='*' || curr->next->data=='/' || curr->next->data=='.'){ //checks for consecutive decimal points/consecutive operators/no digits following an operator
                return false;
            }

            curr=curr->next; //moves to next node
            continue; //moves to next iteration
        }

        else
            return false;
    }

    return true; //returns true if all conditions are met
}


// Function to evaluate the expression represented by the linked list
template <typename T>
float LinkedCalc<T>::evaluateExpression() {

    Node<T>* curr = head; //sets current to first node

    if(head == nullptr) //if list is empty
        return 0;

    float out=0;

    char swc = '+'; //assigns first val to out in switch case

    while(curr){ //iterates through the list until the end

        if(isDigit(curr->data)){

            Node<T>* nextOp=curr->next; //sets nextOp to next node

            while(nextOp && (isDigit(nextOp->data) || nextOp->data=='.')){ //checks if next node is a digit or a decimal to keep track of operators
                nextOp=nextOp->next;
            }

            float val=convertToFloat(curr); //converts the current list to a float
            
            switch(swc){ //switch case to perform operations
                case '+':
                    out += val;
                    break;
                case '-':
                    out -= val;
                    break;
                case '*':
                    out *= val;
                    break;
                case '/':
                    out /= val;
                    break;
                default:
                    break;
            }

            curr=nextOp; //sets current to nextOp
        }

        else if(curr->data=='+' || curr->data=='-' || curr->data=='*' || curr->data=='/'){ //checks if current node is an operator
            swc = curr->data; //sets the swc variable to the next operator
            curr = curr->next; //moves to next node
        }
    }

    return out;
}