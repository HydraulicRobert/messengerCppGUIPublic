#ifndef DATENBANK_H
#endif  DATENBANK_H

#include "windows.h"
#include "mysql.h"
#include "string.h"
#include <vector>
#include <ctime>
#include <sstream>
class datenbank
{
protected:

private:
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    bool istEingeloggt;
    std::vector<std::string> platzhalter;
    std::string nutzername, passwort;
    int nutzeridAbsender;
public:
    datenbank();
    bool einloggen(std::string nutzername, std::string passwort, int nutzerIdAbsender);
    bool registrieren(std::string nutzername, std::string passwort);
    bool vectorFuellenNutzerSuche();
    std::string vectorAusgabeStringFunktion();
    bool nachrichtSenden(int nutzerID, std::string nachricht);
    std::string zeitstempelZuDatum(std::string zeit);
    bool vectorFuellenNachrichten();
    int getNutzerSucheVectorSize();
    MYSQL* getConn();
    bool getIstEingeloggt();
    void setConn(MYSQL* conn);
    int getAbsenderID();
    void platzhalterLeeren();
};
