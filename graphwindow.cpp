#include "graphwindow.h"
#include "ui_graphwindow.h"
#include <QMainWindow>

#include <QJsonDocument>
#include <future>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <fstream>
#include <string>
#include <QString>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTextEdit>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <QShortcut>
#include <cmath>
#include <QTime>
#include <QIcon>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}


void GraphWindow::fillMap() {
    m_colors.insert("red", "#FF0000");
    m_colors.insert("light blue", "#ADD8E6");
    m_colors.insert("green", "#006400");
    m_colors.insert("yellow", "#FFFF00");
    m_colors.insert("orange", "#FFE5B4");
    m_colors.insert("purple", "#A020F0");
    m_colors.insert("brown", "#964B00");
    m_colors.insert("dark blue", "#00008B");
    m_colors.insert("pink", "#FFC0CB");

}

void GraphWindow::indexColors() {
    m_indices.insert("dark blue", 7);
    m_indices.insert("light blue", 1);
    m_indices.insert("pink", 8);
    m_indices.insert("brown", 6);
    m_indices.insert("purple", 5);
    m_indices.insert("orange", 4);
    m_indices.insert("yellow", 3);
    m_indices.insert("green", 2);
    m_indices.insert("red", 0);
}



void GraphWindow::SaveAsPic(const QString& m_ext){
    QString dir = QDir::homePath();
    QString name = "Untilted." + m_ext;
    QString fileType = m_ext.toUpper() + "(*." + m_ext.toUpper() + ")";
    QString fileName= QFileDialog::getSaveFileName(this, "Save image", dir + "/" + name, fileType, nullptr, QFileDialog::DontUseNativeDialog);
    if (!fileName.isNull()) {
        QPixmap pixMap = this->ui->graphicsView->grab();
        pixMap.save(fileName);
    }

}

void GraphWindow::on_actionSaveAsPng_triggered() {
    GraphWindow::SaveAsPic("png");
}


void GraphWindow::on_actionSaveAsJpg_triggered() {
    GraphWindow::SaveAsPic("jpeg");
}

void GraphWindow::on_actionOpen_triggered(){

    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", "GRAPH files (*.graph)", nullptr, QFileDialog::DontUseNativeDialog);
    std::string filename = file.toStdString();
    std::ifstream openFile;
    //m_path=filename;
    openFile.open(filename);

    if (!openFile.fail()){

    }

}

void GraphWindow::on_actionClose_triggered() {
    switch(QMessageBox::question(this, "Warning",
                                  "<FONT COLOR='#171717'>Do you want to save changes?</FONT>",

                                  QMessageBox::Save |
                                      QMessageBox::No |
                                      QMessageBox::Cancel |

                                      QMessageBox::Cancel) )
    {
    case QMessageBox::Save:
        emit on_actionSave_triggered();
        close();
        break;
    case QMessageBox::No:
        close();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
    return;
}

void GraphWindow::on_actionSave_triggered(){

}
