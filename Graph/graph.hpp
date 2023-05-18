#ifndef graphhpp
#define graphhpp


#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <cstdlib>

#include "node.hpp"
#include "edge.hpp"

//NODE--------------------------------------------------------------------------------------------------------NODE
Node::Node(int examID, int numberOfStudents){
    this->examID = examID;
    this->numberOfStudents = numberOfStudents;
    this->colour = -1; //Setting as -1 as it hasn't been set yet
    this->degree = 0;
    this->possibleColours = new std::vector<int>();
}

Edge * Node::isConflicted(int conflictedNode){
    
    for(int i = 0; i < conflicts.size(); ++i){
        if(this->conflicts.at(i)->containsNode(conflictedNode)){
            return this->conflicts.at(i);
        }
    }
    return nullptr;
}

std::string Node::getExamAndPeriod(){
    return std::to_string(this->examID) + " " + std::to_string(this->colour) + " (" + std::to_string(this->degree) + "); ";
}

void Node::addEdge(Edge * edge){
    this->conflicts.push_back(edge);
    this->degree = this->degree + 1;
}

std::vector<Node *> * Node::getUncolouredNeighboursWithEntOver1(){
    std::vector<Node *> * output = new std::vector<Node *>();

    for(int i = 0; i < this->conflicts.size(); ++i){
        Node * n2 = this->conflicts.at(i)->getOtherNode(this);
        if(n2->colour == -1 && n2->getEntropy() > 1){
            output->push_back(n2);
        }
    }
    return output;
}

void Node::removeColourFromEntropy(int colour){
    int index = -1;
    for(int i = 0; i < this->possibleColours->size(); ++i){
        if(this->possibleColours->at(i) == colour){
            index = i;
            break;
        }
    }

    if(index != -1){
        //std::cout << "Colour " << colour << " removed from node " << this->examID << std::endl;
        this->possibleColours->erase(this->possibleColours->begin() + index);
        this->possibleColours->shrink_to_fit();
    } else{
        //std::cout << "Node " << this->examID << " has already removed the colour " << colour << std::endl;
    }

}

std::vector<Node *> * Node::getUncolouredNeighbours(){
    std::vector<Node *> * output = new std::vector<Node *>();

    for(int i = 0; i < this->conflicts.size(); ++i){
        output->push_back(this->conflicts.at(i)->getOtherNode(this));
    }
    return output;
}


int Node::getEntropy(){
    //std::cout << "ENTROPY OF NODE " << this->examID << " IS " << this->possibleColours->size() << "\n";
    return this->possibleColours->size();
    
}

void Node::setEntropy(int ent){ 
    this->possibleColours->clear();
    this->possibleColours->shrink_to_fit();
    for(int j = 0; j < ent; ++j){
        this->possibleColours->push_back(j);
    }
}

void Node::setFirstNum(){
    this->colour = this->possibleColours->at(0);
}

void Node::debugOutput(){
    std::cout << this->possibleColours->size() << std::endl;
}
//EDGE--------------------------------------------------------------------------------------------------------EDGE

Edge::Edge(Node * n1, Node * n2){
    this->numberOfConflicts = 1;
    this->node1 = n1;
    this->node2 = n2;
}

bool Edge::containsNode(int nodeId){
    if(this->node1->examID == nodeId)return true;
    if(this->node2->examID == nodeId)return true;
    return false;
}
void Edge::addEdgesToNode(){
    this->node1->addEdge(this);
    this->node2->addEdge(this);
}

Node * Edge::getOtherNode(Node * n1){
    if(n1 == this->node1)return this->node2;
    else return this->node1;
}

void Edge::addNumberOfConflicts(){
    this->numberOfConflicts = this->numberOfConflicts + 1;   
}

//GRAPH--------------------------------------------------------------------------------------------------------GRAPH
class Graph{
    public:
        std::vector<Node *> * nodes;
        std::vector<Edge *> * edges;

        int numberOfStudents;
        int numberOfNoiseStudents;

        int numberOfPeriods;

        int numberOfExams;

        int biggestDegree;

        std::vector<int> savedGraph;

        Graph(){
            this->nodes = new std::vector<Node *>();
            this->edges = new std::vector<Edge *>();

            this->numberOfNoiseStudents = 0;
            this->numberOfStudents = 0;
            this->numberOfPeriods = 0;
            this->numberOfExams = 0;
        }

        void addEdge(int n1, int n2){
            Node * node1 = (this->nodes->at(n1-1));
            Node * node2 = (this->nodes->at(n2-1));

            Edge * e = new Edge(node1, node2);
            
            this->edges->push_back(e);
            e->addEdgesToNode();
        }

        void addNode(Node * node){
            this->nodes->push_back(node);
        }

        
        std::string outputString(){
            std::string output = "";
            for(int i = 0; i < this->nodes->size(); ++i){
                output+= this->nodes->at(i)->getExamAndPeriod();
            }
            return output;
        }

        int getChromaticNumber(){
            std::set<int> ints = std::set<int>();

            for(int i = 0; i < this->nodes->size(); ++i){
                ints.insert(this->nodes->at(i)->colour);
            }
            return ints.size();
        }

        void resetGraph(int entropy){
            
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->colour = -1;
            }
            this->setNodesEntropy(entropy);

        }

        void setNodesEntropy(int number){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->setEntropy(number);
            }
        }
        void debugNodes(){
            for(int i = 0; i < this->nodes->size(); ++i){
                this->nodes->at(i)->debugOutput();
            }
        }
        void deleteGraph(){}

        void biggestDegreeAmount(){
            int biggest = 0;
            for(int i = 0; i < this->nodes->size(); ++i){
                if(this->nodes->at(i)->degree > biggest){
                    biggest = this->nodes->at(i)->degree;
                }
            }
            this->biggestDegree = biggest;
            //std::cout << "Biggest Degree of graph is " << this->biggestDegree << std::endl;
        }


        int numOfConflicts(){
            return this->edges->size();
        }

        void saveGraphNums(){
            if(this->savedGraph.size() > 0)this->savedGraph.clear();

            for(int i = 0; i < this->nodes->size(); ++i){
                this->savedGraph.push_back(this->nodes->at(i)->colour);
            }
        }

        void loadGraph(){
            for(int i = 0 ; i < this->numberOfExams; ++i){
                this->nodes->at(i)->colour = this->savedGraph.at(i);
            }
        }
};


//FUNCTIONS TO DO WITH THE GRAPH
void addEdgesToConflictingClasses(std::vector<int> * conflictClasses, Graph * graph){
    int numConflicts = conflictClasses->size();
    for(int i = 0; i < numConflicts; ++i){
        for(int j = i + 1; j < numConflicts; ++j){

            Edge * conflictedEdge = graph->nodes->at(conflictClasses->at(i)-1)->isConflicted(conflictClasses->at(j));

            if(conflictedEdge == nullptr){
                graph->addEdge(conflictClasses->at(i),conflictClasses->at(j));
            }
            else {
                int currentConflicts = conflictedEdge->numberOfConflicts;
                conflictedEdge->numberOfConflicts = currentConflicts + 1;
            }
        }
    }
}

#endif