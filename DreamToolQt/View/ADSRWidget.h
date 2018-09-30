#pragma once

#include <QWidget>

class ADSRWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ADSRWidget(QWidget *parent = nullptr);

signals:

public slots:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent* event) override;
};
