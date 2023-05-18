#ifndef nodehpp
#define nodehpp

#include <vector>
#include <string>

class Edge;

class Node{
    public:
        int examID;

        int colour; //Essentially the chosen period 
        
        std::vector<int> * possibleColours;

        int numberOfStudents;
        
        int degree;

        std::vector<Edge *> conflicts;

        Node(int examID, int numberOfStudents);

        std::string getExamAndPeriod();

        Edge * isConflicted(int conflictedNode);

        void addEdge(Edge * edge);

        std::vector<Node *> * getUncolouredNeighboursWithEntOver1();

        std::vector<Node *> * getUncolouredNeighbours();

        void removeColourFromEntropy(int colour);

        int getEntropy();

        void setFirstNum();
  
        void debugOutput();

        void setEntropy(int ent);


};

#endif