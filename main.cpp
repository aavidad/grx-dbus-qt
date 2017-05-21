#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include "grx-dbus.h"
#include <QList>

QString arp_scan(const QString &arg)
{
       QProcess process;
       process.start(arg);
       process.waitForFinished(-1);
       return process.readAllStandardOutput();
}

QList<QVariant> ipsNodos()
{
       QString consulta;
       QList<QVariant> vector;
       consulta.append("select ip from NODOS");
       QSqlQuery consultar;
       if (consultar.exec(consulta)){
           while (consultar.next()){
               vector.append(consultar.value(0).toString());
           }
       }
       else{
           qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
       }
       return vector;
}

QString ipsNodosString()
{
       QString vector;
       QSqlQuery consultar;
       if (consultar.exec("select ip from NODOS")){
           while (consultar.next()){
               vector.append(consultar.value(0).toString()+" ");
           }
       }
       else{
           qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
       }
       return vector;
}

QString GrxArp::busca_router(const QString &ip)
{
       return arp_scan( ARP_SCAN + ip);
}

QString GrxArp::crea_conexion (const QString &conexion)
{

   return conexion;
}

QString GrxArp::busca_router_muestra_nombre(const QString &ips)
{
       QString arp;
       arp = arp_scan(ARP_SCAN + ips);
       QStringList nodoList = arp.split('\n');
       QString nombre_nodo;
       for (int i = 0; i < nodoList.size(); ++i){
           nombre_nodo.append(consulta_nombre(nodoList.value(i).split("\t").first())+" ");
           }
return nombre_nodo;
}

QList<QVariant> GrxArp::ip_nodos(){

       return ipsNodos();
}

QString GrxArp::ip_nodos_string(){

    return ipsNodosString();

}

QString GrxArp::busca_todos_routers_nombre()
{
       return busca_router_muestra_nombre(ipsNodosString());

}

QString GrxArp::busca_todos_routers()
{
        QString vector;
        QString arp;
        vector=ipsNodosString();
        arp = arp_scan(ARP_SCAN + vector);
return arp;
}

QList<QVariant> GrxArp::consulta_sql(const QString &arg,int columna)
{
    QSqlQuery consultar;
    QList<QVariant> vector;
    if (consultar.exec(arg)){
       while (consultar.next()){
          vector.append(consultar.value(columna).toString());
       }
    } else {
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
    }

return vector;
}

QString GrxArp::consulta_nombre (const QString &ip){
    QSqlQuery consultar;
    QString resultado;
    consultar.prepare(QString("select nombre from NODOS where ip = :ip"));
    consultar.bindValue(":ip", ip);
    if (consultar.exec() and consultar.first()){
            resultado.append(consultar.value(0).toString());

    } else {
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
    }
return resultado;
}

QString GrxArp::consulta_ip (const QString &nombre){
    QSqlQuery consultar;
    QString resultado;
    consultar.prepare(QString("select ip from NODOS where nombre = :nombre"));
    consultar.bindValue(":nombre", nombre);
    if (consultar.exec() and consultar.first()){
            resultado.append(consultar.value(0).toString());

    } else {
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
    }
return resultado;
}

QString GrxArp::busca_nodo_por_ip(const QString &nodo)
{
    QString arp;
    arp = arp_scan(ARP_SCAN + nodo);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"
    return nodoList.first();//con esto cogemos solo la ip


}

bool GrxArp::esta_nodo_por_ip(const QString &nodo)
{
    QString arp;
    arp = arp_scan(ARP_SCAN + nodo);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"

    if (nodo == nodoList.first()){
        return true;
    } else {
        return false;
    }

}

QString GrxArp::busca_nodo_por_nombre(const QString &nodo)
{
    QString arp;
    QString ip_nodo;
    ip_nodo = consulta_ip(nodo);
    qDebug() << ip_nodo;
    arp = arp_scan(ARP_SCAN + ip_nodo);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"
    return nodoList.first();//con esto cogemos solo la ip


}

void GrxArp::mensaje(const QString &mensaje){
    qDebug()<< "ha pasado antes"<< mensaje;
}

bool GrxArp::esta_nodo_por_nombre(const QString &nodo)
{
    QString arp;
    QString ip_nodo;
    ip_nodo = consulta_ip(nodo);
    arp = arp_scan(ARP_SCAN + ip_nodo);
    QStringList nodoList = arp.split('\n').first().split('\t');
    if (ip_nodo == nodoList.first()){
        return true;
    } else {
        return false;
    }
}

bool GrxArp::esta_veleta(){
    QProcess process;
    process.start("ping -c1 -w1 8.8.8.8");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
    return !process.exitCode();
}

void GrxArp::temporizador(){
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(esta_veleta()));
    timer->start(1000);
}

QList<QVariant> GrxArp::consulta_sql_todo(const QString &arg)
{
    int col,i;
    QSqlQuery consultar;
    QList<QVariant> vector;
    if (consultar.exec(arg)){
       QSqlRecord record = consultar.record();
       col=record.count();
       while (consultar.next()){
          for (i=0; i< col;i++){
                vector.append(consultar.value(i).toString());
          }

       }
    }else{
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
    }

return vector;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (!crea_conexionSQLite()){
        fprintf(stderr, "No puedo conectarme a la base de datos.\n");
        return 1;
        }
    if (!QDBusConnection::systemBus().isConnected()) {
            fprintf(stderr, "No puedo conectarme al bus del sistema.\n");
            return 1;
        }
    if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
            fprintf(stderr, "%s\n",
                    qPrintable(QDBusConnection::systemBus().lastError().message()));
            return 1;
        }
    GrxArp grx_arp;
    grx_arp.temporizador();
    QDBusConnection::systemBus().registerObject("/", &grx_arp, QDBusConnection::ExportAllSlots);
    QDBusConnection::systemBus().registerObject("/senales", &grx_arp,QDBusConnection::ExportAllSignals);
    return a.exec();
}
