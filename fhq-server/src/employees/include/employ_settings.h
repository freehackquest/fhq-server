#ifndef EMPLOY_SETTINGS_H
#define EMPLOY_SETTINGS_H

#include <employees.h>
#include <iserver.h>
#include <serversetthelper.h>

#include <QString>

class EmploySettings : public EmployBase {
public:
    EmploySettings();
    static std::string name() { return "EmploySettings"; }
    virtual bool init();

    void initSettings(IWebSocketServer *pWebSocketServer);
    QString getSettString(QString name);
    void setSettString(QString sName, QString sValue);
    QString getSettPassword(QString name);
    void setSettPassword(QString sName, QString sValue);
    int getSettInteger(QString sName);
    void setSettInteger(QString sName, int nValue);
    bool getSettBoolean(QString sName);
    void setSettBoolean(QString sName, bool bValue);

    bool hasSett(QString sName);
    QString getSettType(QString sName);

    nlohmann::json toJson();

private:

    void addNewSetting(ServerSettHelper* pServerSettHelper);
    void initSettingDatabase(ServerSettHelper* pServerSettHelper);
    void updateSettingDatabase(ServerSettHelper* pServerSettHelper);

    IWebSocketServer *m_pWebSocketServer;
    QMap<QString, ServerSettHelper*> m_mapSettings;

    QMutex m_mtxServerSettings;
    QString TAG;
};

#endif // EMPLOY_SETTINGS_H
