#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QValidator>

namespace Ui {
class Widget;
}

class MsnParser;

enum DateRange {
    y_76_99 = 1976,
    y_00_23 = 2000,
    y_24_47 = 2024
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_radioButton1_toggled(bool toggle);
    void on_radioButton2_toggled(bool toggle);
    void on_radioButton3_toggled(bool toggle);
    void on_lineEdit_textChanged(const QString &text);

private:
    void checkAndSetDateRange();

private:
    MsnParser *parser;
    QValidator *validator;

private:
    Ui::Widget *ui;
};

class MsnParser : public QObject
{
    Q_OBJECT

private:
    QMap<QChar, int> table;
    QStringList monthTable;
    int dateRange;

private:
    void fillTables();

public:
    explicit MsnParser(QObject *parent = 0);
    ~MsnParser();

public:
    QString parseMsn(const QString &msn) const;
    void setDateRange(DateRange value);
};

#endif // WIDGET_H
