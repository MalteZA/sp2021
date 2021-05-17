#ifndef EXAM_GRAPHMAKER_H
#define EXAM_GRAPHMAKER_H

#include <iostream>
#include <fstream>
#include "vessel_t.h"

class graphMaker {
private:
public:
    /// Converts a vessel to a digraph, ready to be pasted into http://www.webgraphviz.com/ -RQ2
    static void vesselToDigraph(vessel_t vessel, const std::string& fileName) {
        auto relativeFileName = "../../" + fileName + ".txt";
        std::ofstream file;
        file.open(relativeFileName);
        file << std::scientific;
        file << vessel.toDigraph();
        file.flush();
        file.close();
    }
};

#endif //EXAM_GRAPHMAKER_H
