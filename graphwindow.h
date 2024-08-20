#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include "parser.h"
#include <iostream>
#include <QString>
#include <QPointF>
#include <QMap>

class QGraphicsScene;

QT_BEGIN_NAMESPACE
namespace Ui {
class GraphWindow;
}
QT_END_NAMESPACE

class GraphWindow : public QMainWindow {
    Q_OBJECT

public:
    GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();
    void SaveAsPic(const QString& m_ext);
    void drawGraph(std::string);
    void updateGraph();
    void drawNormalGraph(Parser *);
    void drawScatterPlot(Parser *);

signals:


private slots:
    void warning(QString s);
    void on_actionSaveAsPng_triggered();
    void on_actionSaveAsJpg_triggered();
    void on_actionClose_triggered();
    void on_actionSave_triggered();
    void on_openOne_triggered();
    void on_actionSaveAsPng2_triggered();
    void on_actionSaveAsJpg2_triggered();
    void on_actionClose2_triggered();
    void on_actionSave2_triggered();
    void on_pbSave_clicked();

private:
    Ui::GraphWindow* ui;
    std::string m_path = "";
    bool m_normalGraphChecked = true;
    bool m_scatterPlotChecked = false;

};
#endif  // GRAPHWINDOW_H

