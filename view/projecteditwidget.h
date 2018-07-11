#ifndef PROJECTEDITWIDGET_H
#define PROJECTEDITWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectEditWidget;
}

class ProjectEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectEditWidget(QWidget *parent = 0);
    ~ProjectEditWidget();

private:
    Ui::ProjectEditWidget *ui;
};

#endif // PROJECTEDITWIDGET_H
