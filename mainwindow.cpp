#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QLabel>
#include <QProgressBar>
#include <QSpinBox>
#include <QFontComboBox>
#include <QMessageBox>

void MainWindow::iniUI()
{
    fLabCurFile = new QLabel;
    fLabCurFile->setMidLineWidth(150);
    fLabCurFile->setText("当前文件：");
    ui->statusbar->addWidget(fLabCurFile);

    progressBar = new QProgressBar;
    progressBar->setMinimum(5);
    progressBar->setMaximum(50);
    progressBar->setValue(ui->textEdit->font().pointSize());
    ui->statusbar->addWidget(progressBar);

    spinFontSize = new QSpinBox;
    spinFontSize->setMinimum(5);
    spinFontSize->setMaximum(50);
    ui->toolBar->addWidget(new QLabel("字体大小"));
    ui->toolBar->addWidget(spinFontSize);

    comboFont = new QFontComboBox;
    ui->toolBar->addWidget(new QLabel("字体"));
    ui->toolBar->addWidget(comboFont);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniUI(); // 注意大小写
    setCentralWidget(ui->textEdit);

    // 初始化当前文件路径
    currentFile.clear();

    // 1. 创建"新建"动作并设置属性
    QAction *actNew = new QAction(QIcon(":/images/images/new2.bmp"), tr("&New"), this);
    actNew->setShortcut(QKeySequence::New);
    actNew->setText(tr("新建"));
    actNew->setStatusTip(tr("Create a new file"));

    // 2. 将动作添加到菜单栏的"文件"菜单
    ui->menu->addAction(actNew);

    // 3. 将动作添加到工具栏
    ui->toolBar->addAction(actNew);

    // 4. 连接信号与槽
    connect(actNew, &QAction::triggered, this, &MainWindow::newFile);

    if (ui->actAbout) { // 安全检查
        ui->toolBar->insertAction(ui->actAbout, actNew); // 将 actNew 插入到 ui->actionOpen 之前
    }
}

void MainWindow::on_actAbout_triggered()
{
    // 使用 QMessageBox 创建一个信息对话框
    QMessageBox::about(this, // 父窗口设置为当前主窗口
                       tr("关于"), // 对话框标题
                       tr("开发人员信息：赵钰深")); // 对话框内容
}

// 自定义的新建文件槽函数
void MainWindow::newFile()
{
    if (maybeSave()) {
        ui->textEdit->clear(); // 使用textEdit而不是plainTextEdit
        currentFile.clear();
        setWindowModified(false);
        setWindowTitle("Untitled[*] - MyApp");

        // 更新状态栏显示
        fLabCurFile->setText("当前文件：新文件");
    }
}

// 检查保存函数
bool MainWindow::maybeSave()
{
    if (!isWindowModified()) {
        return true;
    }

    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"),
                                                                 tr("文档已被修改。\n是否保存更改?"),
                                                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

// 保存函数实现
bool MainWindow::save()
{
    // 这里需要实现实际的文件保存逻辑
    // 暂时返回true表示保存成功
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}
