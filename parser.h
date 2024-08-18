#ifndef PARSER_H
#define PARSER_H

#include "snapshot.h"

#include <iostream>
#include <QVector>
#include <QFile>
#include <qglobal.h>

class Parser
{
public:
    // razmisli kad zavrsis sa implementacijom moze li ovde *
    Parser(std::string);
    ~Parser();

    QVector<Snapshot*> getSnapshots() const;
    QVector<quint64> getTimes() const;
    QVector<quint64> getTotalB() const;

    QVector<std::string> getMassifArgs() const;
    std::string getCommand() const;
    std::string getTimeUnit() const;

    bool isValidMassifFile() const;
    void parseFile();

private:
    std::string m_filename;
    // za pasriranje snapshotova
    QVector<Snapshot*> m_snapshots;
    //vektor vremena za plotovanje
    QVector<quint64> m_timesI;
    //vektor potrosnje memorije za plotovanje
    QVector<quint64> m_totalB;

    // za parsiranje preambule
    QVector<std::string> m_massifArgs;
    //jedinica u kojoj se izrazava vreme, za plotovanje bitno
    std::string m_timeUnit;
    //na koji izvrsni fajl treba da se primeni Massif
    std::string m_command;

    quint64 m_memHeapB;
    quint64 m_memHeapExtraB;
    quint64 m_stacks;
    //memHeapB/(memHeapB + memExtraB + stacks)
    qreal m_precentage;


    void parsePreamble(std::ifstream &);
    void parseDescription(const std::string &);
    void parseCommand(const std::string &);
    void parseTimeUnit(const std::string &);

    std::string trim(const std::string &);

    // provera da li je prosledjeni fajl zaista dobar massif fajl
    bool m_isMassifFile = true;
};

#endif // PARSER_H
