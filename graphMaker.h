#ifndef EXAM_GRAPHMAKER_H
#define EXAM_GRAPHMAKER_H

#include <iostream>
#include <fstream>
#include "vessel_t.h"
#include "graphviz/gvc.h"

class graphMaker {
private:
public:
    /// Converts a vessel to a digraph, ready to be pasted into http://www.webgraphviz.com/ -RQ2
    static void vesselToDigraph(vessel_t vessel, const std::string& fileName) {
        auto relativeFileName = "../output/" + fileName + ".txt";
        std::ofstream file;
        file.open(relativeFileName);
        file << std::scientific;
        file << vessel.toDigraph();
        file.flush();
        file.close();

        GVC_t *gvc = gvContext();
        Agraph_t *agraph = agmemread(vessel.toDigraph().c_str());
        gvLayout(gvc, agraph, "dot");
        relativeFileName = "../output/" + fileName + ".png";
        gvRenderFilename(gvc, agraph, "png", relativeFileName.c_str());
        gvFreeLayout(gvc, agraph);
        agclose(agraph);
    }
};

#endif //EXAM_GRAPHMAKER_H
