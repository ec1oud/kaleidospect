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
#include "starburst.h"

#include <QtMath>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/qsgnode.h>
#include <QtQuick/QSGSimpleMaterialShader>

class StarburstShader : public QSGSimpleMaterialShader<Starburst::StarburstShaderParams>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(StarburstShader, Starburst::StarburstShaderParams)
public:
    StarburstShader() {
        setShaderSourceFile(QOpenGLShader::Vertex, ":/shaders/starburst.vsh");
        setShaderSourceFile(QOpenGLShader::Fragment, ":/shaders/starburst.fsh");
    }

    QList<QByteArray> attributes() const { return QList<QByteArray>() << "vertexCoord"; }

    void updateState(const Starburst::StarburstShaderParams *p, const Starburst::StarburstShaderParams *) {
        program()->setUniformValue(m_uColor, p->color);
        program()->setUniformValue(m_uMatrix, p->pmvMatrix);
        program()->setUniformValue(m_uOpacity, GLfloat(p->opacity));
    }

    void resolveUniforms() {
        m_uColor = program()->uniformLocation("color");
        m_uOpacity = program()->uniformLocation(uniformOpacityName());
        m_uMatrix = program()->uniformLocation("pmvMatrix");
    }

private:
    int m_uColor;
    int m_uOpacity;
    int m_uMatrix;
};

struct PointAlpha {
    float r, theta, t;
    void set(float nr, float nth, float nt) {
        r = nr; theta = nth; t = nt;
    }
};

static const QSGGeometry::AttributeSet &attributes_PointAlpha()
{
    static QSGGeometry::Attribute data[] = {
        QSGGeometry::Attribute::create(0, 3, GL_FLOAT),
    };
    static QSGGeometry::AttributeSet attrs = { 1, 3 * sizeof(float), data };
    return attrs;
}

Starburst::Starburst(QQuickItem *parent)
  : QQuickItem(parent)
{
    m_material = StarburstShader::createMaterial();
    m_material->setFlag(QSGMaterial::Blending);
    m_material->setFlag(QSGMaterial::RequiresFullMatrix);
    setFlag(ItemHasContents, true);
}

void Starburst::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged();
}

void Starburst::setLevels(QJSValue levels)
{
    m_levels = levels;
    update();
    emit levelsChanged();
}

void Starburst::setGain(qreal gain)
{
    if (qFuzzyCompare(m_gain, gain))
        return;

    m_gain = gain;
    emit gainChanged();
}

void Starburst::setHoleRadius(qreal holeRadius)
{
    if (qFuzzyCompare(m_holeRadius, holeRadius))
        return;

    m_holeRadius = holeRadius;
    emit holeRadiusChanged();
}

void Starburst::itemChange(QQuickItem::ItemChange ch, const QQuickItem::ItemChangeData &data)
{
    QQuickItem::itemChange(ch, data);
    update();
}

QSGNode *Starburst::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    int levelsCount = m_levels.property("length").toInt();
    QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);
    if (node) {
        m_geometry = node->geometry();
        m_geometry->allocate(levelsCount * 8);
    } else {
        node = new QSGGeometryNode;
        m_geometry = new QSGGeometry(attributes_PointAlpha(), levelsCount * 6);
        node->setGeometry(m_geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        node->setMaterial(m_material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    // m_geometry->setDrawingMode(GL_TRIANGLE_STRIP); (default)

    QMatrix4x4 matrix;
    matrix.scale(width() / 2, height() / 2);
    matrix.translate(1.0, 1.0);
    m_material->state()->pmvMatrix = matrix;
    m_material->state()->color = m_color;
    m_material->state()->opacity = opacity();

    PointAlpha *vertices = static_cast<PointAlpha *>(m_geometry->vertexData());
    qreal rayAngle = M_PI * 2 / levelsCount;
    qreal outerRadius = width() / 2;
    qreal innerRadiusR = m_holeRadius / outerRadius;
    qreal innerThickness = 2.5;
    qreal outerThickness = 7.5;
    qreal innerHalfWidthTheta = qAtan(innerThickness / m_holeRadius / 2);
    qreal outerHalfWidthTheta = qAtan(outerThickness / outerRadius / 2);
    for (int i = 0; i < levelsCount; ++i) {
        qreal theta = rayAngle * i;
        float level = m_levels.property(i).toNumber() * m_gain;
        int vi = i * 6;

        // r, theta, t
        vertices[vi    ].set(innerRadiusR, theta + innerHalfWidthTheta, 0);
        vertices[vi + 1].set(level, theta + outerHalfWidthTheta, 0);
        vertices[vi + 3].set(level, theta - outerHalfWidthTheta, 1);
        vertices[vi + 2].set(innerRadiusR, theta - innerHalfWidthTheta, 1);
        vertices[vi + 4].set(innerRadiusR, theta - innerHalfWidthTheta, 1);

        theta += rayAngle + innerHalfWidthTheta;

        vertices[vi + 5].set(innerRadiusR, theta, 1);
    }

    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial | QSGNode::DirtyForceUpdate);
    return node;
}
