#include "widget.h"
#include "ui_widget.h"

enum {
    absoluteFileNameRole = Qt::UserRole + 1
};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("3D Application");
    createFilesTable(ui->tableWidget);

}

Widget::~Widget()
{
    delete ui;
}
void Widget::createFilesTable(QTableWidget *table)
{
    table->setColumnCount(2);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList labels;
    labels <<tr("File Name")<<tr("Size");
    table->setHorizontalHeaderLabels(labels);
    table->verticalHeader()->hide();
    table->setShowGrid(false);
}

static void findRecursion(const QString &path, QStringList *result)
{
    QDir currentDir(path);
    QStringList filters;
    filters << "*.jpg";
    currentDir.setNameFilters(filters);
    if (currentDir.entryList().isEmpty()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","This directory contains no \"jpg\" files.");
        messageBox.setFixedSize(100,100);
        return;
    }
    const QString prefix =path+QLatin1Char('/');
    foreach (const QString &match, currentDir.entryList(QDir::Files | QDir::NoSymLinks)){
        result->append(prefix + match);
    }
}

void Widget::on_btnApply_clicked()
{
    QString i = ui->lineEdit->text();
    udpSocket = new QUdpSocket(this);
    QByteArray datagram = i.toUtf8();
    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 54321);
}


void Widget::on_btnBrowse_clicked()
{
    QString directory=QFileDialog::getExistingDirectory(
                this,
                tr("&Browse..."),
                "",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );
    if(!directory.isEmpty()){
        ui->lineEdit->setText(directory);
        QStringList files;
        findRecursion(directory, &files);
        showFiles(files);
        files.clear();
    }
}

void Widget::showFiles(const QStringList &files)
{
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < files.size(); ++i){
        const QString &fileName = files.at(i);
        const QString toolTip = QDir::toNativeSeparators(fileName);
        const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(fileName));
        const QString onlyfileName = QFileInfo(fileName).fileName();
        const qint64 size = QFileInfo(fileName).size();
        QTableWidgetItem *fileNameItem =new QTableWidgetItem(onlyfileName);
        fileNameItem->setData(absoluteFileNameRole, QVariant(fileName));
        fileNameItem->setToolTip(toolTip);
        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *sizeItem =new QTableWidgetItem(tr("%1 KB").arg(int((size+1023)/1024)));
        sizeItem->setData(absoluteFileNameRole, QVariant(fileName));
        sizeItem->setToolTip(toolTip);
        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);


        int row =ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, fileNameItem);
        ui->tableWidget->setItem(row, 1, sizeItem);
    }
}
