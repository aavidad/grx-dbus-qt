#ifndef GRX_DBUS
#define GRX_DBUS
#define SERVICE_NAME    "grx.dbus.service"
#define BASE_DATOS "/var/lib/grx/conexiones.db"
#define ARP_SCAN "/usr/sbin/arp-scan -q -x -g "
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QObject>
#include "QtDebug"
#include <QDBusVariant>
#include <QProcess>

class GrxArp: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE QString busca_equipos(const QString &arg);
    Q_SCRIPTABLE QString busca_equipo_terminados_en(const QString i, const QString &arg);
    Q_SCRIPTABLE QList<QVariant> ips_nodos();
    Q_SCRIPTABLE QList<QVariant> consulta_sql(const QString &arg,int i);
    Q_SCRIPTABLE QString busca_ips_nodos();
private:
    QSqlDatabase db;
    QSqlQuery consultar;
};

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(BASE_DATOS);
    if (!db.open()) {
       qDebug() << ("No puedo abrir la base de datos\n");
       return false;
    }

   return true;
}


#endif
