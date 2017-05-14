#ifndef GRX_DBUS
#define GRX_DBUS
#define SERVICE_NAME    "grx.dbus.service"

#include <QtCore/QObject>

class GrxArp: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE QString busca_lista(const QString &arg);
    Q_SCRIPTABLE QString busca_equipos(const QString &arg);
    Q_SCRIPTABLE QString busca_equipo(const QString &arg);

};

#endif
