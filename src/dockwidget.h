#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <qpixmap.h>
#include <ksystemtray.h>

class PPPStats;
class kzdial;
class KAction;
class KStdAction;

/**
@author Nikos Zarkadas
*/
class DockWidget : public KSystemTray
{
    Q_OBJECT
public:
    DockWidget(QWidget* parent, const char* name, PPPStats *st);
    ~DockWidget();
protected:
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent* )
    {}
	void enterEvent(QEvent *);
private slots:
    void toggle_window_state();
    void mousePressEvent(QMouseEvent *e);

public slots:
    void paintIcon(int);
    void take_stats();
    void stop_stats();
	
public:
    static DockWidget *dock_widget;

private:
    int toggleID;
    PPPStats *stats;
	QString el_time;

    KPopupMenu *popup_m;

    QPixmap dock_none_pixmap;
    QPixmap dock_left_pixmap;
    QPixmap dock_right_pixmap;
    QPixmap dock_both_pixmap;
};

#endif
