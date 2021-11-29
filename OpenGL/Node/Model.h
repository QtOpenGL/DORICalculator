#ifndef MODEL_H
#define MODEL_H

#include "Node.h"

class Model : public Node
{
public:
    Model(QString name);

    const QString &name() const;

private:
    QString mName;
};

#endif // MODEL_H
