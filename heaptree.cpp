#include "heaptree.h"

HeapTree::HeapTree()
    : m_percentage(0)
    , m_allocatedBytes(0)
    , m_address("")
    , m_functionName("")
    , m_fileName("")
    , m_lineNum(0)
{

}

HeapTree::~HeapTree()
{

}

qreal HeapTree::getPercentage() const
{
    return m_percentage;
}

void HeapTree::setPercentage(const qreal *percentage)
{
    m_percentage = *percentage;
}

quint64 HeapTree::getAllocatedBytes() const
{
    return m_allocatedBytes;
}

void HeapTree::setAllocatedBytes(const quint64 *allocatedBytes)
{
    m_allocatedBytes = *allocatedBytes;
}

std::string HeapTree::getAddress() const
{
    return m_address;
}

void HeapTree::setAddress(const std::string *addr)
{
    m_address = *addr;
}

std::string HeapTree::getFunctionName() const
{
    return m_functionName;
}

void HeapTree::setFunctionName(const std::string *functionName)
{
    m_functionName = *functionName;
}

std::string HeapTree::getFileName() const
{
    return m_fileName;
}

void HeapTree::setFileName(const std::string *filename)
{
    m_fileName = *filename;
}

uint HeapTree::getLineNum() const
{
    return m_lineNum;
}

void HeapTree::setLineNum(const uint *lineNum)
{
    m_lineNum = *lineNum;
}
