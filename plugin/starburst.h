#ifndef STARBURST_H
#define STARBURST_H

#include <QQuickItem>

class Starburst : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Starburst)

public:
    Starburst(QQuickItem *parent = nullptr);
    ~Starburst();
};

#endif // STARBURST_H
