#ifndef wfchpp
#define wfchpp

#include <vector>
#include <stack>
#include <iostream>

#include "../Graph/graph.hpp"
#include "../Graph/node.hpp"
#include "../Graph/edge.hpp"

Node * biggestConflictNode(std::vector<Edge *> * edges){
    int biggestConflict = 0;
    int index = 0;
    for(int i = 1; i < edges->size(); ++i){
        if(edges->at(i)->numberOfConflicts > biggestConflict){
            index = i;
            biggestConflict = edges->at(i)->numberOfConflicts;
        }
    }

    if(edges->at(index)->node1->degree >= edges->at(index)->node2->degree){
        return edges->at(index)->node1;
    }
    else{
        return edges->at(index)->node2;
    }
}

Node * biggestDegreeNode(std::vector<Node *> * nodes){
    int biggestDegree = 0;
    int index = 0;
    for(int i = 1; i < nodes->size(); ++i){
        if(nodes->at(i)->degree > biggestDegree){
            biggestDegree = nodes->at(i)->degree;
            index = i;
        }
    }

    return nodes->at(index);
}

bool itemInStack(std::stack<Node *> stack, Node * node){
    while(!stack.empty()){
        if(node == stack.top())return true;
        else stack.pop();
    }
    return false;
}

void propagate(Graph * graph, Node * node){
    std::stack<Node *> nodeStack = std::stack<Node *>();
    nodeStack.push(node);

    while(!nodeStack.empty()){
        Node * nodeFromStack = nodeStack.top();
        nodeStack.pop();

        std::vector<Node *> * nodeNeighbours = nodeFromStack->getUncolouredNeighbours();
        for(int j = 0; j < nodeNeighbours->size(); ++j){
            Node * currentNeighbour = nodeNeighbours->at(j);
            currentNeighbour->removeColourFromEntropy(nodeFromStack->colour);

            if(currentNeighbour->getEntropy() > 1){
                if(currentNeighbour->getEntropy() == 1 && !itemInStack(nodeStack, currentNeighbour)){
                    currentNeighbour->setFirstNum();
                    nodeStack.push(currentNeighbour);
                }
            }
        }
    }

}

void collapse(Node * n1){
    n1->setFirstNum();
    //std::cout << "NODE " << n1->examID << " Collapsed with colour " << n1->colour <<std::endl;
}

bool graphIncomplete(Graph * graph, bool * zeroEnt){
    bool stillUncoloured = false;

    for(int i = 0; i < graph->nodes->size(); ++i){
        if(graph->nodes->at(i)->getEntropy() == 0){
            *zeroEnt = true;
            return false;
        }
        if(graph->nodes->at(i)->colour == -1)stillUncoloured = true;
    }
    return stillUncoloured;
    
}

Node * observe(Graph * graph){
    int lowestEntropy = 1000000;
    int index = 0;
    bool anyZeroEnts = false;
    for(int i = 0; i < graph->numberOfExams; ++i){
        Node * currentNode = graph->nodes->at(i);
        if(currentNode->colour == -1 && currentNode->getEntropy() < lowestEntropy){
            if(currentNode->getEntropy() == 0){
                anyZeroEnts = true;
                //std::cout << "ZERRO ENT" << std::endl;
            }
            lowestEntropy = currentNode->getEntropy();
            index = i;
        }
    }

    if(anyZeroEnts)return nullptr;
    
    return graph->nodes->at(index);

}

bool WFC(Graph * graph, Node * startNode){

    bool zeroEntropy = false;

    Node * currentNode;

    if(startNode == nullptr){
        currentNode = biggestDegreeNode(graph->nodes);
    } else {
        currentNode = startNode;
    }
    collapse(currentNode);
    
    propagate(graph, currentNode);
    currentNode = observe(graph);
    
    
    while(graphIncomplete(graph, &zeroEntropy)){
        collapse(currentNode);
        propagate(graph, currentNode);
        currentNode = observe(graph);
    }

    if(zeroEntropy)return false;
    
    return true;
}

int WFCAll(Graph * graph, int currentCheck){
    for(int i = 0; i < graph->nodes->size(); ++i){
        if(graph->nodes->at(i)->conflicts.size() == 0)continue;

        graph->resetGraph(currentCheck);
        bool done = WFC(graph, graph->nodes->at(i));
        if(done && graph->getChromaticNumber() <= currentCheck){
            graph->saveGraphNums();
            std::cout << "New chrom number = " << graph->getChromaticNumber() << std::endl;
            return graph->getChromaticNumber();
        }
        
    }
    return currentCheck + 1;
}



#endif