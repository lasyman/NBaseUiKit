#include "nbasesearchbutton.h"
#include "ui_search_button.h"

NBaseSearchButton::NBaseSearchButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NBaseSearchButton)
{
    ui->setupUi(this);
    connect(ui->btnSearch, &QPushButton::clicked, this, &NBaseSearchButton::onSearchBtnClicked);
    connect(ui->lneSearch, &QLineEdit::returnPressed, ui->btnSearch, &QPushButton::click);
    connect(ui->lneSearch, &QLineEdit::editingFinished, this, &NBaseSearchButton::onSearchEditFinished);
}

NBaseSearchButton::~NBaseSearchButton()
{
    delete ui;
}

void NBaseSearchButton::onSearchBtnClicked()
{
    const QString &strKey = ui->lneSearch->text().trimmed();
    emit sigSearchKey(strKey);
}

void NBaseSearchButton::onSearchEditFinished()
{
    if (ui->lneSearch->text().isEmpty())
        emit sigSearchClear();
}
