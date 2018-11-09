/****************************************************************************
**
** Copyright (C) 2018 Shawn Rutledge
**
** This file is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation
** and appearing in the file LICENSE included in the packaging
** of this file.
**
** This code is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
****************************************************************************/
#ifndef STARBURST_H
#define STARBURST_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QSGSimpleMaterial>

class QSGGeometry;
class QOpenGLShaderProgram;

class Starburst : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QJSValue levels READ levels WRITE setLevels NOTIFY levelsChanged)
    Q_PROPERTY(qreal gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(qreal holeRadius READ holeRadius WRITE setHoleRadius NOTIFY holeRadiusChanged)

public:
    Starburst(QQuickItem *parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(QColor color);

    QJSValue levels() const { return m_levels; }
    void setLevels(QJSValue levels);

    qreal gain() const { return m_gain; }
    void setGain(qreal gain);

    qreal holeRadius() const { return m_holeRadius; }
    void setHoleRadius(qreal holeRadius);

signals:
    void colorChanged();
    void levelsChanged();
    void gainChanged();
    void holeRadiusChanged();

public:
    struct StarburstShaderParams
    {
        QMatrix4x4 pmvMatrix;
        QColor color;
        qreal opacity;

        int compare(const StarburstShaderParams *p) const {
            return p->color.rgba() - color.rgba();
        }
    };

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

protected:
    virtual void itemChange(ItemChange, const ItemChangeData &) override;

private:
    QOpenGLShaderProgram *m_program = nullptr;
    QSGGeometry *m_geometry = nullptr;
    QSGSimpleMaterial<StarburstShaderParams> *m_material = nullptr;
    QColor m_color = QColor(Qt::cyan);
    QJSValue m_levels;
    int m_levelsCount = 40;
    qreal m_gain = 50;
    qreal m_holeRadius = 15;
};

#endif // STARBURST_H
