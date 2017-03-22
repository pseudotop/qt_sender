#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

class QTableWidget;
class QTableWidgetItem;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void on_btnBrowse_clicked();
    void on_btnApply_clicked();

private:
    void createFilesTable(QTableWidget *table);
    void showFiles(const QStringList &files);

    Ui::Widget *ui;
    QUdpSocket *udpSocket;

    QDir currentDir;
};

#endif // WIDGET_H
