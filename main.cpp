#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include "grx-dbus.h"
#include <QList>

QString buscaEquipos(const QString &arg)
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

QString GrxArp::busca_equipos(const QString &arg)
{
       return buscaEquipos( ARP_SCAN +arg);
}

QString GrxArp::busca_equipo_terminados_en(const QString i,const QString &arg )
{

    return buscaEquipos( ARP_SCAN + arg + " -G " + i);
}

/*QList<QVariant> GrxArp::ips_nodos()
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
*/

QList<QVariant> GrxArp::ips_nodos(){

       return ipsNodos();
}



QString GrxArp::busca_ips_nodos()
{
        QString consulta;
        QProcess process;
        QString vector;
        consulta.append("select ip from NODOS");
        QSqlQuery consultar;
        if (consultar.exec(consulta)){
            while (consultar.next()){
                   vector.append(consultar.value(0).toString()+" ");
            }
         qDebug() << vector;
         process.start("/usr/sbin/arp-scan -q -x -g "+vector);
         process.waitForFinished(-1);
        }
        else{
            qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
        }
return process.readAllStandardOutput();
}

QList<QVariant> GrxArp::consulta_sql(const QString &arg,int i)
{
    QSqlQuery consultar;
    QList<QVariant> vector;
    if (consultar.exec(arg)){
       while (consultar.next()){
          vector.append(consultar.value(i).toString());
       }
    }else{
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
    }

return vector;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (!createConnection()){
        fprintf(stderr, "No puedo conectarme a la base de datos.\n");
        return 1;
        }
    if (!QDBusConnection::systemBus().isConnected()) {
            fprintf(stderr, "No puedo conectarme al bus de sistema.\n");
            return 1;
        }
        if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
            fprintf(stderr, "%s\n",
                    qPrintable(QDBusConnection::systemBus().lastError().message()));
            return 1;
        }
        GrxArp grx_arp;
        QDBusConnection::systemBus().registerObject("/", &grx_arp, QDBusConnection::ExportAllSlots);

    return a.exec();
}
