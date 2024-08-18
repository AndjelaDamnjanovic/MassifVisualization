#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <qglobal.h>

enum class SnapshotType {
    EMPTY,
    DETAILED,
    PEAK
};

class Snapshot
{
public:
    Snapshot();
    ~Snapshot();

    uint getSnapshotIndex() const;
    void setSnapshotIndex(const uint *);

    quint64 getTime() const;
    void setTime(const quint64 *);

    quint64 getTotalHeapB() const;
    void setTotalHeapB(const quint64 *);

    quint64 getUsefulHeapB() const;
    void setUsefulHeapB(const quint64 *);

    quint64 getExtraHeapB() const;
    void setExtraHeapB(const quint64 *);

    quint64 getStacks() const;
    void setStacks(const quint64 *);

    SnapshotType getSnapshotType() const;
    void setSnapshotType(const SnapshotType);

private:
    // defaultna vrednost za maksimalan broj snapshotova je 100, pa iako se ova vrednost moze menjati, ne ocekuje se neki
    // preveiki broj -- stiga uint (podlozno izmenama ako zatreba)
    uint m_snapshotIndex;

    // za ostale vrednosti izabran je quint64 zbog velikog broje vrednosti koje moze da skladisti, unsigned jer nijedna vrednost
    // ne moze biti negativna
    quint64 m_timeI;
    quint64 m_totalHeapB;
    quint64 m_usefulHeapB;
    quint64 m_extraHeapB;
    quint64 m_stacksB;

    SnapshotType m_snapshotType;
};

#endif // SNAPSHOT_H
