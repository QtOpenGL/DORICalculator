#ifndef CURSORPOSITIONWIDGET_H
#define CURSORPOSITIONWIDGET_H

#include <QGroupBox>
#include <QLabel>

class CursorPositionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit CursorPositionWidget(QWidget *parent = nullptr);

public slots:
    void onCursorPositionChanged(QPointF position);

signals:
    void dirty();

private:
    QLabel *mLabelCursorX;
    QLabel *mLabelCursorY;
};

#endif // CURSORPOSITIONWIDGET_H
