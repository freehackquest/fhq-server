#ifndef MEMORY_CACHE_SERVERSETTINGS_H
#define MEMORY_CACHE_SERVERSETTINGS_H

#include <iserver.h>
#include <serversetthelper.h>

class MemoryCacheServerSettings : public IMemoryCache {
	public:
		// ICache
		virtual QString name();
		
		// class
		MemoryCacheServerSettings(IWebSocketServer *pWebSocketServer);
		
		QString getSettString(QString name);
		void setSettString(QString sName, QString sValue);
		int getSettInteger(QString sName);
		void setSettInteger(QString sName, int nValue);
		bool getSettBoolean(QString sName);
		void setSettBoolean(QString sName, bool bValue);

        QJsonArray toJsonObject();

	private:

        void addNewSetting(ServerSettHelper* pServerSettHelper);
        void initSettingDatabase(ServerSettHelper* pServerSettHelper);
        void updateSettingDatabase(ServerSettHelper* pServerSettHelper);

		IWebSocketServer *m_pWebSocketServer;
        QMap<QString, ServerSettHelper*> m_mapSettings;
		
		QMutex m_mtxServerSettings;
        QString TAG;
};

#endif // MEMORY_CACHE_SERVERSETTINGS_H
