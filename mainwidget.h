#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include "mypushbutton.h"
#include "tscene.h"
#include "audioplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void paintEvent(QPaintEvent *);
    ChooseDifficultyMenu * chooseDifficultyMenu = NULL;

protected:
    Ui::MainWidget *ui;
    AudioPlayer * m_audioPlayer;

};
#endif // MAINWIDGET_H
