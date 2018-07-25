#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);
    virtual void updateContent() = 0;
signals:

public slots:
};

#endif // BASEWIDGET_H
