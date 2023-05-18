#ifndef wfccontrollerhpp
#define wfccontrollerhpp

#include <iostream>

#include "../Graph/graph.hpp"
#include "WFC.hpp"

void WFCController(Graph * graph){
    
    graph->biggestDegreeAmount();
    int checking = 999;
    int lastChecked = 1000;
    bool isChecked = false;

    graph->resetGraph(checking);
    
    while(!isChecked){
        
        bool successful = WFC(graph, nullptr);
        
        if(successful){
            graph->saveGraphNums();
            
            //get chromatic number
            int chromNumber = graph->getChromaticNumber();
            //std::cout << "Successful VCP check on number " << checking << " with chromatics number of " << chromNumber << std::endl;
            lastChecked = chromNumber;
            //subtract one from chromatic number
            checking = chromNumber - 1;
            //reset graphs
            graph->resetGraph(checking);

        }
        else{
            //std::cout << "ChromNumber 1 " << graph->getChromaticNumber() << std::endl;

            graph->saveGraphNums();
            graph->resetGraph(lastChecked - 1);
            
            int amount = WFCAll(graph, lastChecked - 1);

            if(amount >= lastChecked){
                isChecked = true;
                break;
            } 
        }
    }

    //std::cout << "Found the chromatic number of graph as " << graph->getChromaticNumber() << ". The biggest degree of this graph is " << graph->biggestDegree << std::endl;
    //std::cout << std::endl;

    //std::cout << graph->outputString() << std::endl;
}

#endif