#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_phpFpmStatus(new phpFpmStatus(this))
{
    ui->setupUi(this);
    connect(m_phpFpmStatus,SIGNAL(gotStatusData()),
            this,SLOT(refreshStatus()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_refreshbutton_clicked()
{
    m_phpFpmStatus->setStatusUrl(QUrl(ui->addressbar->text() + "?full&xml"));
    m_phpFpmStatus->refresh();
}

void MainWindow::refreshPoolStatus()
{
    QString infoStr = "";
    qDebug() << m_phpFpmStatus->poolStatus();
    QVariantMap statusMap = m_phpFpmStatus->poolStatus();
    for(QVariantMap::const_iterator iter = statusMap.begin(); iter != statusMap.end(); ++iter) {
        QString key = iter.key();
        QString val = iter.value().toString();
        infoStr += "\n\r";
        infoStr += key + ": ";
        infoStr += val;
    }
    ui->infoLabel->setText(infoStr);
}

void MainWindow::refreshStatus()
{
    refreshPoolStatus();
    refreshProcesses();
}


void MainWindow::refreshProcesses()
{
    QTableWidget * procTablet = ui->procTable;
    QList<QVariantMap *> processes = m_phpFpmStatus->processes();

    procTablet->setRowCount(processes.size());

//    procTablet->setColumnWidth(0,300);



    procTablet->setEditTriggers(QAbstractItemView::NoEditTriggers);
    procTablet->setSelectionBehavior(QAbstractItemView::SelectRows);
    procTablet->setSelectionMode(QAbstractItemView::SingleSelection);
    procTablet->setShowGrid(true);
    procTablet->setSortingEnabled(true);




    QVariantMap *firstProc = processes.first();
    QStringList tableHeaders;

    for(QVariantMap::const_iterator iter = firstProc->begin(); iter != firstProc->end(); ++iter) {
        QString key = iter.key();
        tableHeaders << key;
    }
    procTablet->setColumnCount(tableHeaders.size());
    procTablet->setHorizontalHeaderLabels(tableHeaders);

    int index = 0;
    foreach (QVariantMap *p, processes) {
        for (int i = 0; i < tableHeaders.size();i++){
            // create item
            if (!procTablet->item(index,i)){
                procTablet->setItem(index,i,new QTableWidgetItem());
            }
            // iinsert stuff
            procTablet->item(index,i)->setText(p->value(tableHeaders.at(i)).toString());
        }

        index++;
    }

}
