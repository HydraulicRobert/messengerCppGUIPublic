#include <iostream>
#include "datenbank.h"

datenbank::datenbank()
{
}
MYSQL* datenbank::getConn()
{
return conn;
}
void datenbank::setConn(MYSQL* conn)
{
    this->conn = conn;
}
bool datenbank::einloggen(std::string nutzername, std::string passwort, int nutzeridAbsender)
{
    std::string query = "SELECT * FROM nutzer WHERE nutzer.Name = '"+nutzername+"' AND Passwort = '"+passwort+"'";
    const char* q = query.c_str();
    int qstate = mysql_query(getConn(), q);
    if(!qstate)
        {
            res = mysql_store_result(getConn());
            while(row = mysql_fetch_row(res))
                {
                    if(row)
                    {
                        this->nutzername = row[0];
                        this->passwort = row[1];
                        this->nutzeridAbsender = std::stoi(row[2]);
                        istEingeloggt = true;
                        return true;
                    }
                }
        }
        return false;
}
bool datenbank::registrieren(std::string nutzername, std::string passwort)
{
    std::string name = nutzername;
    std::string query = "insert into nutzer(Name, Passwort) values('"+name+"', '"+passwort+"')";
    std::string pruefeDoppeltenLogin = "SELECT Name FROM nutzer WHERE Name = '"+name+"'";
    const char* q = pruefeDoppeltenLogin.c_str();
    int qstate = mysql_query(conn, q);
    int doppenPruefZahl = 0;
    if(!qstate)
    {
            res = mysql_store_result(conn);
            while(row = mysql_fetch_row(res))
            {
                if(row[0] == name)
                {
                    std::cout<<"benutzer existiert bereits. ";
                    return false;
                }
            }
            if(doppenPruefZahl == 0)
            {
                const char* q2 = query.c_str();
                int qstate2 = mysql_query(conn, q2);
                if(!qstate)
                {
                    res = mysql_store_result(conn);
                }
                std::cout<<"erfolgreich registriert!";
                return true;
            }

    }
    doppenPruefZahl = 0;
    return true;
}
bool datenbank::vectorFuellenNutzerSuche()
{
    std::string query = "SELECT * FROM nutzer";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);
    if(!qstate)
        {
            res = mysql_store_result(getConn());
            while(row = mysql_fetch_row(res))
                {
                    if(row)
                    {
                        platzhalter.push_back("Nutzer: ");
                        platzhalter.push_back(row[0]);
                        platzhalter.push_back("\r\n\r");
                        platzhalter.push_back("ID: ");
                        platzhalter.push_back(row[2]);
                        platzhalter.push_back("\r\n\r");
                    }
                }
        }
    return true;
}
std::string datenbank::vectorAusgabeStringFunktion()
{
    std::string vectorAusgabeString;
    for(int i = 0; i < platzhalter.size(); i++)
    {
        vectorAusgabeString = vectorAusgabeString+platzhalter[i];
    }
    return vectorAusgabeString;
}
bool datenbank::nachrichtSenden (int nutzerID, std::string nachricht)
{
    time_t timet = std::time(nullptr);
    std::string nutzerIDString, nutzeridAbsenderString, timee;
    nutzeridAbsenderString      = std::to_string(nutzerID);
    nutzerIDString              = std::to_string(nutzeridAbsender);
    timee                        = std::to_string(timet);
    std::string query           = "insert into empfangenenachricht(AbsenderID, EmpfaengerID, Nachricht, Zeit) values('"+nutzeridAbsenderString+"','"+nutzerIDString+"','"+nachricht+"','"+timee+"')";
    const char* q = query.c_str();
    int qstate;
    qstate = mysql_query(conn, q);
    std::cout<<qstate;
    return true;
}
std::string datenbank::zeitstempelZuDatum(std::string zeit)
{
            time_t zeit2 = std::stoi(zeit);
            struct tm * dt;
            char buffer [30];
            dt = localtime(&zeit2);
            strftime(buffer, sizeof(buffer), "Am %d.%m.%Y um %H:%M:%S Uhr", dt);
            return std::string(buffer);
}
bool datenbank::vectorFuellenNachrichten()
{
    std::string query           = "select empfangenenachricht.Nachricht, empfangenenachricht.AbsenderID, empfangenenachricht.Zeit, nutzer.Name from empfangenenachricht left join nutzer on empfangenenachricht.AbsenderID = nutzer.NutzerID where empfangenenachricht.EmpfaengerID = '"+std::to_string(nutzeridAbsender)+"'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);
    std::cout<<query;
    if(!qstate)
        {
            res = mysql_store_result(getConn());
            while(row = mysql_fetch_row(res))
                {
                    if(row)
                    {
                        platzhalter.push_back("Von: ");
                        if(row[3])
                        {
                            platzhalter.push_back(row[3]);
                        }else
                        {
                            platzhalter.push_back("unbekannt");
                        }
                        platzhalter.push_back(" um: ");
                        platzhalter.push_back(zeitstempelZuDatum(row[2]));
                        platzhalter.push_back("\r\n\r");
                        platzhalter.push_back(row[0]);
                        platzhalter.push_back("\r\n\r");
                        platzhalter.push_back("\r\n\r");
                    }
                }
        }
    return true;
}
int datenbank::getNutzerSucheVectorSize()
{
    return platzhalter.size();
}
bool datenbank::getIstEingeloggt()
{
    return istEingeloggt;
}
int datenbank::getAbsenderID()
{
    return nutzeridAbsender;
}
void datenbank::platzhalterLeeren()
{
    platzhalter.clear();
}
