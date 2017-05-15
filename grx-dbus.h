#ifndef GRX_DBUS
#define GRX_DBUS
#define SERVICE_NAME    "grx.dbus.service"
#define BASE_DATOS "/var/lib/grx/conexiones.db"
#define ARP_SCAN "/usr/sbin/arp-scan -q -x -g -G 100 "
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QObject>
#include "QtDebug"
#include <QDBusVariant>
#include <QSqlRecord>

class GrxArp: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE QString busca_router(const QString &arg);
    Q_SCRIPTABLE QString busca_router_muestra_nombre(const QString &nombres);
    Q_SCRIPTABLE QList<QVariant> ip_nodos();
    Q_SCRIPTABLE QString ip_nodos_string();
    Q_SCRIPTABLE QList<QVariant> consulta_sql(const QString &arg,int i);
    Q_SCRIPTABLE QList<QVariant> consulta_sql_todo(const QString &arg);
    Q_SCRIPTABLE QString busca_todos_routers();
    Q_SCRIPTABLE QString consulta_nombre(const QString &ip);
    Q_SCRIPTABLE QString consulta_ip (const QString &nombre);
    Q_SCRIPTABLE QString busca_nodo_por_ip(const QString &nodo);
    Q_SCRIPTABLE QString busca_nodo_por_nombre(const QString &nodo);
    Q_SCRIPTABLE bool esta_nodo_por_nombre(const QString &nodo);
    Q_SCRIPTABLE bool esta_nodo_por_ip(const QString &nodo);

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
