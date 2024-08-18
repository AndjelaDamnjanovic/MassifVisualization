#include "snapshot.h"

Snapshot::Snapshot()
    : m_snapshotIndex(0)
    , m_timeI(0)
    , m_totalHeapB(0)
    , m_usefulHeapB(0)
    , m_extraHeapB(0)
    , m_stacksB(0)
    , m_snapshotType(SnapshotType::EMPTY)

{

}

Snapshot::~Snapshot()
{

}

uint Snapshot::getSnapshotIndex() const
{
    return m_snapshotIndex;
}

void Snapshot::setSnapshotIndex(const uint *index)
{
    m_snapshotIndex = *index;
}

quint64 Snapshot::getTime() const
{
    return m_timeI;
}

void Snapshot::setTime(const quint64 *timeI)
{
    m_timeI = *timeI;
}

quint64 Snapshot::getTotalHeapB() const
{
    return m_totalHeapB;
}

void Snapshot::setTotalHeapB(const quint64 *totalB)
{
    m_totalHeapB = *totalB;
}

quint64 Snapshot::getUsefulHeapB() const
{
    return m_usefulHeapB;
}

void Snapshot::setUsefulHeapB(const quint64 *usefulB)
{
    m_usefulHeapB = *usefulB;
}

quint64 Snapshot::getExtraHeapB() const
{
    return m_extraHeapB;
}

void Snapshot::setExtraHeapB(const quint64 *extraB)
{
    m_extraHeapB = *extraB;
}

quint64 Snapshot::getStacks() const
{
    return m_stacksB;
}

void Snapshot::setStacks(const quint64 *stackB)
{
    m_stacksB = *stackB;
}

SnapshotType Snapshot::getSnapshotType() const
{
    return m_snapshotType;
}

void Snapshot::setSnapshotType(const SnapshotType snapshotType)
{
    m_snapshotType = snapshotType;
}
