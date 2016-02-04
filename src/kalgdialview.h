#ifndef KALGDIALVIEW_H
#define KALGDIALVIEW_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class KTextBrowser;
class QLabel;
class QTimer;
class QTimerEvent;
class QTime;
class QString;

class KProcIO;

/**
@author Nikos Zarkadas
*/
class KAlgDialView : public QWidget
{
    Q_OBJECT
public:
	KAlgDialView(QWidget *parent = 0, const char *name = 0,WFlags fl = 0 );

    ~KAlgDialView();
	void colorMessage(QColor , QString);
	QFrame* frame4;
	KTextBrowser* m_textInfo;
	QLabel* textLabel2;
	QLabel* m_timeShow;
	QLabel* textLabel3;
	QLabel* m_speedInfo;
	QLabel* label4;
	QLabel* m_ibytesInfo;
	QLabel* textLabel5;
	QLabel* m_obytesInfo;
	QLabel* textLabel6;
	QLabel* m_tbytesInfo;
	QLabel* textLabel1;
	QLabel* m_ipacketsInfo;
	QLabel* textLabel3_2;
	QLabel* m_opacketsInfo;

	char *var_run_pid;
	bool isConnected;
	
	protected:
		QVBoxLayout* KAlgDialViewBaseLayout;
		QVBoxLayout* frame4Layout;
		QHBoxLayout* layout7;
		QHBoxLayout* layout9;
		QHBoxLayout* layout10;
		QHBoxLayout* layout11;
		QHBoxLayout* layout12;
		QHBoxLayout* layout7_2;
		QHBoxLayout* layout8;
		
	private:
		int hour;
		int min;
		int sec;
		QTime *elapsed_time;
		int _timerid;
		int try_con;
		QTimer *con_timer;
		QString _shell_cmd;
		QString _connect_cmd;
		QString _provider;
		QString _disconnect_cmd;
		pid_t _connect_cmd_id;
		pid_t _disconnect_cmd_id;
protected:
    void	timerEvent( QTimerEvent * );
    void showInfo();
	protected slots:
		virtual void languageChange();

	private:
		QPixmap image0;
		KProcIO *proc_io;
public slots:
    virtual void disconnectSlot();
    virtual void connectSlot();
    void connect_timer_slot();
    void writeProcInfo(KProcIO *);
	signals:
		void quit_app();
		void beginDocking();
		void pppDisconnect(); //Occur when user press the disconnect button.
private:
    
};

#endif
