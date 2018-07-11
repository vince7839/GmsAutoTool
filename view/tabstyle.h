#ifndef TABSTYLE_H
#define TABSTYLE_H
#include<QProxyStyle>

class TabStyle : public QProxyStyle
{
public:
    TabStyle();
protected:
    void drawControl(ControlElement, const QStyleOption *
                     ,QPainter *, const QWidget *) const;

QSize sizeFromContents(ContentsType, const QStyleOption*
                       ,const QSize&, const QWidget*) const;
};

#endif // TABSTYLE_H
