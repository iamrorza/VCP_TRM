#include <string>
#include <vector>
#include <ctime>

#include "Graph/graph.hpp"
#include "inputReader.hpp"
#include "WFC/WFCController.hpp"


void benchMark(std::string url){
    int start = clock();
    Graph * graph = readTextFile(url);
    WFCController(graph);

    int stop = clock();
    delete graph;
    std::cout << (stop-start)/double(CLOCKS_PER_SEC) * 1000 << std::endl;
}

int main(){


    for(int i = 11; i <= 11; ++i){
        Graph * graph = readTextFile("testdata/testdata" + std::to_string(11) + ".txt");
        WFCController(graph);
        std::cout << "CHROM NUMBER " << graph->getChromaticNumber() << std::endl;
        std::cout << "------------------" << std::endl;
        delete graph;
    }
    
    return 0;
}
