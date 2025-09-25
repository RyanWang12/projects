#include "node.h"

#include <iostream>
// overloaded constructor to take in the next & prev pointer along with the this user's info
Node::Node(Node*next, Node*prev, User userInfo):
            next(next),
            prev(prev),
            userInfo(userInfo){}

// pointer to store user info & initialize pointers to null
Node::Node(User userInfo): 
          next(nullptr),
          prev(nullptr),
          userInfo(userInfo){}

