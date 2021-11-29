#ifndef MODEL_H
#define MODEL_H

#include "Node.h"

class Model : public Node
{
public:
    Model(QString name);

    const QString &name() const;

    bool colored() const;
    void setColored(bool newColored);

    const QVector3D &color() const;
    void setColor(float r, float g, float b);

private:
    QString mName;
    QVector3D mColor;
    bool mColored;
};

#endif // MODEL_H
