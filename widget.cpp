#include "widget.h"
#include "ui_widget.h"

#include <QtGlobal>

#include <QDateTime>
#include <QMessageBox>
#include <QRegExp>
#include <QIcon>
#include <QDate>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEdit->setClearButtonEnabled(true);
#endif // QT_VERSION

    parser = new MsnParser(this);
    checkAndSetDateRange();

    // MSN RegExp
    QRegExp regExp("^[A-Za-z0-9]{4}[A-Za-z]{2}[A-Za-z0-9]{5}$");
    validator = new QRegExpValidator(regExp, this);
    ui->lineEdit->setValidator(validator);

    // Window Settings
    adjustSize();
    setFixedSize(size());
    setWindowIcon(QIcon(":/MSN-parser.png"));
}

void Widget::checkAndSetDateRange()
{
    int currentYear = QDateTime::currentDateTime().toString("yyyy").toInt();
    if (currentYear >= 1976 && currentYear <= 1999) {
        ui->radioButton1->setChecked(true);
    } else if (currentYear >= 2000 && currentYear <= 2023) {
        ui->radioButton2->setChecked(true);
    } else if (currentYear >= 2024 && currentYear <= 2047) {
        ui->radioButton3->setChecked(true);
    } else {
        QMessageBox::warning(this, tr("Wrong Date!"), tr("Please check Date and Time on your computer.\n"
                                                         "Please set Date Range manually."));
        ui->radioButton1->setChecked(true);
    }
}

void Widget::on_radioButton1_toggled(bool toggle)
{
    if (toggle) {
        parser->setDateRange(y_76_99);
        on_lineEdit_textChanged(ui->lineEdit->text());
    }
}

void Widget::on_radioButton2_toggled(bool toggle)
{
    if (toggle) {
        parser->setDateRange(y_00_23);
        on_lineEdit_textChanged(ui->lineEdit->text());
    }
}

void Widget::on_radioButton3_toggled(bool toggle)
{
    if (toggle) {
        parser->setDateRange(y_24_47);
        on_lineEdit_textChanged(ui->lineEdit->text());
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
    // Will be deleted on destroying parent object.
    // delete parser;
    // delete validator;
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
}

QString MsnParser::getHalf(bool qHalf) const
{
    return (qHalf) ? tr("First half of ") : tr("Last half of ");
}

QString MsnParser::parseMsn(const QString &msn) const
{
    QString answer = QString::number(table[msn[5]] / 2 + 1) + "-";      // Month
    answer += QString::number(table[msn[4]] + dateRange);               // Year
    return QDate::fromString(answer, "M-yyyy")                          // Date Format
            .toString(getHalf(table[msn[5]] % 2 == 0) + "MMMM yyyy");   // For "January" instead "1"
}

MsnParser::~MsnParser()
{
    /* Empty destructor */
}
