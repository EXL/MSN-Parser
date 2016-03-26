#include "widget.h"
#include "ui_widget.h"

#include <QDateTime>
#include <QMessageBox>
#include <QRegExp>

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    parser = new MsnParser(this);

    setFixedSize(size());

    checkAndSetDateRange();

    // MSN RegExp
    QRegExp regExp("^[A-Za-z0-9]{4}[A-Za-z]{2}[A-Za-z0-9]{5}$");
    validator = new QRegExpValidator(regExp, this);
    // ui->lineEdit->setMaxLength(11);
    ui->lineEdit->setValidator(validator);
}

void Widget::checkAndSetDateRange()
{
    QDateTime _date = QDateTime::currentDateTime();
    int _year = _date.toString("yyyy").toInt();

    if (_year >= 1976 && _year <= 1999) {
        ui->radioButton1->setChecked(true);
    } else if (_year >= 2000 && _year <= 2023) {
        ui->radioButton2->setChecked(true);
    } else if (_year >= 2024 && _year <= 2047) {
        ui->radioButton3->setChecked(true);
    } else {
        QMessageBox::warning(this, tr("Wrong Date!"), tr("Please check Date on your computer.\n"
                                                         "Please set Date Range manually."));
    }
}

void Widget::on_radioButton1_toggled(bool toggle)
{
    if (toggle) {
        parser->setDateRange(y_76_99);
        ui->lineEdit->clear();
    }
}

void Widget::on_radioButton2_toggled(bool toggle)
{
    if (toggle) {
        parser->setDateRange(y_00_23);
        ui->lineEdit->clear();
    }
}

void Widget::on_radioButton3_toggled(bool toggle)
{
    if (toggle) {
        parser->setDateRange(y_24_47);
        ui->lineEdit->clear();
    }
}

void Widget::on_lineEdit_textChanged(const QString &text)
{
    ui->lineEdit->setText(text.toUpper());

    if (text.length() > 5) {
        ui->labelDate->setText("<strong>" + parser->parseMsn(text) + "</strong>");
    } else {
        ui->labelDate->setText("<strong>" + tr("Need more characters") + "</strong>");
    }
}

Widget::~Widget()
{
    delete parser;
    delete validator;
    delete ui;
}

// ----------------------------------------------------- //

MsnParser::MsnParser(QObject *parent) :
    QObject(parent)
{
    dateRange = y_76_99;
    fillTables();
}

void MsnParser::setDateRange(DateRange value)
{
    dateRange = value;
}

void MsnParser::fillTables()
{
    int i = 0;
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (ch == 'I' || ch == 'O') {
            continue;
        } else {
            table.insert(ch, i);
            ++i;
        }
    }

    monthTable << "January" << "February" << "March"
               << "April" << "May" << "June"
               << "July" << "August" << "September"
               << "October" << "November" << "December";
}

QString MsnParser::parseMsn(const QString &msn) const
{
    QString answer = "";

    // Month half
    if (table[msn[5]] % 2 == 0) {
        answer += tr("First half of ");
    } else {
        answer += tr("Last half of ");
    }

    // Month
    answer += monthTable[table[msn[5]] / 2];

    // Year
    answer += " ";
    answer += QString::number(table[msn[4]] + dateRange);
    answer += ".";

    return answer;
}

MsnParser::~MsnParser()
{

}
