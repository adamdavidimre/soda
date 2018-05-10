#include "data/CTree.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"
#include "data/Node.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CTree::CTree():
        m_codeElements(new CIDManager()),
        m_structure(new vector<vector<IndexType>>()),
        m_nodes(new vector<Node*>())
    {}

    CTree::~CTree()
    {
        delete m_codeElements;
        m_codeElements = NULL;
        delete m_structure;
        m_structure = NULL;
        delete m_nodes;
        m_nodes = NULL;
    }

    void CTree::clear()
    {
        m_codeElements->clear();
        m_structure->clear();
        m_nodes->clear(); //for each element
    }

    IndexType CTree::nodeCount()
    {
        return m_nodes->size();
    }

    IndexType CTree::edgeCount()
    {
        IndexType count = 0;

        for(vector<vector<IndexType>>::iterator it = m_structure->begin(); it != m_structure->end(); it++) 
        {
            count+= it->size();
        }

        return count;
    }

    vector<IndexType>& CTree::getEdges(const IndexType& i)
    {
        return m_structure->at(i);
    }

    void CTree::addEdge(Node* parent, Node* child)
    {
        addEdge(parent->m_id, child->m_id);
    }

    Node* CTree::addNode(const IndexType i)
    {
        IndexType newIndex =  m_nodes->size() == 0 ? 0 : m_nodes->back()->m_id+1;
        Node* n = new Node(newIndex, i);

        m_nodes->push_back(n);
        m_structure->push_back(vector<IndexType>());

        return n;
    }

    Node* CTree::addNode(const String& n)
    {
        m_codeElements->add(n);

        IndexType i = m_codeElements->getID(n);

        return addNode(i);
    }

    Node* CTree::addChild(const IndexType parentId, const String& n)
    {
        Node* newNode = addNode(n);
        m_structure->at(parentId).push_back(newNode->m_id);

        return newNode;
    }

    String CTree::getNodeValue(Node* node)
    {
        return m_codeElements->getValue(node->m_elementId);
    }

    bool CTree::isValid()
    {
        return CBFS(*m_structure).isValid(0);
    }

    void CTree::addEdge(const IndexType parentId, const IndexType childId)
    {
        bool exsistingEdge = std::find(m_structure->at(childId).begin(), m_structure->at(childId).end(), parentId) != m_structure->at(childId).end();
        if(exsistingEdge)
        {
            return;
        }

        m_structure->at(parentId).push_back(childId);

        bool isValid = this->isValid();

        if (!isValid)
        {
            m_structure->at(parentId).pop_back();
            throw logic_error("This edge cannot be applied. It creates a circle.");
        }
    }

    vector<IndexType>* CTree::getDFS(IndexType i)
    {
        return CDFS(*m_structure).getDFS(i);
    }

    vector<IndexType>* CTree::getBFS(IndexType i)
    {
        return CBFS(*m_structure).getBFS(i);
    }

    void CTree::save(io::CBinaryIO *out) const
    {
        //IDManager
        m_codeElements->save(out, io::CSoDAio::IDMANAGER);

        //Tree
        out->writeUInt4(m_chunkId);

        IndexType length = sizeof(IndexType);

        for(vector<vector<IndexType>>::const_iterator rit = m_structure->begin() ; rit != m_structure->end(); ++rit )
        {
            length += sizeof(IndexType);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                length += sizeof(IndexType);
            }
        }

        length += m_nodes->size() * sizeof(Node);

        out->writeULongLong8(length);

        //Count of nodes
        IndexType nodesCount = m_nodes->size();
        out->writeULongLong8(nodesCount);

        //Nodes
        for(vector<Node*>::const_iterator it = m_nodes->begin() ; it != m_nodes->end(); ++it )
        {
            out->writeULongLong8((*it)->m_id);
            out->writeULongLong8((*it)->m_elementId);
        }

        //Edges
        for(vector<vector<IndexType>>::const_iterator rit = m_structure->begin() ; rit != m_structure->end(); ++rit )
        {
            IndexType edgeCount = rit->size();
            out->writeULongLong8(edgeCount);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                out->writeULongLong8(*it);
            }
        }
    }

    void CTree::load(io::CSoDAio *in)
    {
        m_codeElements->clear();
        m_structure->clear();
        m_nodes->clear();

        while(in->nextChunkID()) {
            auto chunkId = in->getChunkID();

            if(chunkId == io::CSoDAio::IDMANAGER) 
            {
                m_codeElements->load(in);
            }

            if(m_chunkId == chunkId){
                IndexType nodeCount = in->readULongLong8();

                for(IndexType i = 0; i < nodeCount; ++i)
                {
                    IndexType mId = in->readULongLong8();
                    IndexType mElementId = in->readULongLong8();

                    m_nodes->push_back(new Node(mId, mElementId));
                }

                for (IndexType e = 0; e < nodeCount; ++e) 
                {
                    IndexType edgeCount = in->readULongLong8();

                    vector<IndexType> edges;
                    
                    for (IndexType i = 0; i < edgeCount; ++i) 
                    {
                        IndexType edge = in->readULongLong8();
                        
                        edges.push_back(edge);
                    }

                    m_structure->push_back(edges);
                }
            }
        }
    }

    void CTree::save(const char * filename) const
    {
        io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
        save(out);
        delete out;
    }

    void CTree::load(const char * filename)
    {
        io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
        load(in);
        delete in;
    }

    void CTree::loadJson(const String& path)
    {
        CJsonReader *reader  = new CJsonReader(path);

        //Read IDManager elements
        vector<CJsonReader> nodes = reader->getPropertyVectorFromProperty("nodes"); 
        vector<CJsonReader> edges = reader->getPropertyVectorFromProperty("edges"); 

        for(IndexType i = 0; i <  nodes.size(); i++)
        {
            IndexType nodeId = nodes[i].getIntFromProperty("id");
            String value = nodes[i].getStringFromProperty("value");

            this->addNode(value);
        }

        for(IndexType i = 0; i <  edges.size(); i++)
        {
            IndexType nodeId = edges[i].getIntFromProperty("nodeId");
            vector<IndexType> values = edges[i].getIntVectorFromProperty("values");

            for(vector<IndexType>::iterator v = values.begin(); v < values.end(); v++)
            {
                this->addEdge(nodeId, *v);
            }
        }

        delete reader;
        reader = 0;
    }
}
