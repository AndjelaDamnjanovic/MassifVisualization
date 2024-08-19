#include "heaptreenode.h"

HeapTreeNode::HeapTreeNode()
    : m_numOfChildren(0)
    , m_allocatedBytes(0)
    , m_address("")
    , m_functionName("")
    , m_fileName("")
    , m_lineNum(0)
    , m_level(0)
    , m_parent(nullptr)
{

}

HeapTreeNode::~HeapTreeNode()
{

}

quint64 HeapTreeNode::getAllocatedBytes() const
{
    return m_allocatedBytes;
}

void HeapTreeNode::setAllocatedBytes(const quint64 *allocatedBytes)
{
    m_allocatedBytes = *allocatedBytes;
}

std::string HeapTreeNode::getAddress() const
{
    return m_address;
}

void HeapTreeNode::setAddress(const std::string *addr)
{
    m_address = *addr;
}

std::string HeapTreeNode::getFunctionName() const
{
    return m_functionName;
}

void HeapTreeNode::setFunctionName(const std::string *functionName)
{
    m_functionName = *functionName;
}

std::string HeapTreeNode::getFileName() const
{
    return m_fileName;
}

void HeapTreeNode::setFileName(const std::string *filename)
{
    m_fileName = *filename;
}

uint HeapTreeNode::getLineNum() const
{
    return m_lineNum;
}

void HeapTreeNode::setLineNum(const uint *lineNum)
{
    m_lineNum = *lineNum;
}

uint HeapTreeNode::getLevel() const
{
    return m_level;
}

void HeapTreeNode::setLevel(const uint *level)
{
    m_level = *level;
}

HeapTreeNode *HeapTreeNode::getParent() const
{
    return m_parent;
}

void HeapTreeNode::setParent(HeapTreeNode *parent)
{
    m_parent = parent;
}

QVector<HeapTreeNode *> HeapTreeNode::getChildren() const
{
    return m_children;
}

void HeapTreeNode::setChildren(const QVector<HeapTreeNode *> *children)
{
    for(auto child : *children)
        addChild(child);
}

void HeapTreeNode::addChild(HeapTreeNode *child)
{
    m_children.append(child);
}

uint HeapTreeNode::getNumOfChildren() const
{
    return m_numOfChildren;
}

void HeapTreeNode::setNumOfChildern(const uint *num)
{
    m_numOfChildren = *num;
}

