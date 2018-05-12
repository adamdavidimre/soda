#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "interface/IGraph.h"
#include "data/Node.h"

#ifndef IDAG_H
#define IDAG_H

using namespace std;

namespace soda
{
    class IDAG
    {
        public:
            virtual IndexType nodeCount() = 0;
            virtual IndexType edgeCount() = 0;
            virtual void clear() = 0;
            virtual Node* addNode(const String& n) = 0;
            virtual Node* addNode(const IndexType i) = 0;
            virtual Node* addChild(const IndexType parentNodeId, const String& n) = 0;
            virtual void addEdge(const IndexType parentNodeId, const IndexType childNodeId) = 0;
            virtual void addEdge(Node* parent, Node* child) = 0;
            virtual bool isValid(IndexType root) = 0;
            virtual String getNodeValue(Node* node) = 0;
            virtual String getNodeValue(IndexType nodeId) = 0;
            virtual String getValue(IndexType elementId) = 0;
            virtual vector<IndexType>& getEdges(const IndexType& i) = 0;
            virtual vector<IndexType>* getDFS(IndexType i) = 0;
            virtual vector<IndexType>* getBFS(IndexType i) = 0;
            virtual vector<list<IndexType>*>* convertToNodeIdChains() = 0;
            virtual void save(io::CBinaryIO* out) const = 0;
            virtual void load(io::CSoDAio* in) = 0;
            virtual void save(const char* filename) const = 0;
            virtual void load(const char* filename) = 0;
            virtual void loadJson(const String& path) = 0;
    };
}

#endif
