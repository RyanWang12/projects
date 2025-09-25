#include "user.h"

class Node{
    public:
        //variable initialization
        Node* next;
        Node* prev;
        User userInfo;

        //overloaded constructors
        Node(Node*next, Node*prev, User userInfo);
        Node(User userInfo);
};