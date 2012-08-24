#pragma once

#include <QTcpSocket>
#include <QNetworkProxy>
#include "BCMgr.h"
#include "LIConfig.h"
class NetworkProxyMgr
{
public:
	static NetworkProxyMgr *inst()
	{
		static NetworkProxyMgr instance;
		return &instance;
	}

	void UpdateProxyFromIni()
	{
		LIConfig * cfg = g_pbcmgr->getConfig();
		QNetworkProxy::ProxyType proxyType = QNetworkProxy::NoProxy;
		QString ptype = cfg->getValue("Network/ProxyType", "不使用代理").toString();
		if (ptype == "不使用代理")
		{
			proxyType = QNetworkProxy::NoProxy;

		}
		else if (ptype == "HTTP代理")
		{
			proxyType = QNetworkProxy::HttpProxy;

		}
		else if (ptype == "SOCKS5代理")
		{
			proxyType = QNetworkProxy::Socks5Proxy;
		}

		UpdateProxy(proxyType, cfg->getValue("Network/Host", "").toString(), cfg->getValue("Network/Port", "").toInt(), cfg->getValue("Network/UserName", "").toString(), cfg->getValue("Network/Password", "").toString());
	}


	bool Test(QNetworkProxy::ProxyType type, const QString & hostName, quint16 port ,const QString & user, const QString & password)
	{
		QNetworkProxy proxy;
		proxy.setType(type);
		proxy.setHostName(hostName);
		proxy.setPort(port);
		proxy.setUser(user);
		proxy.setPassword(password);
		QTcpSocket ts;
		ts.setProxy(proxy);
		ts.connectToHost("www.langisle.com", 80);
		return ts.waitForConnected(3000);
	}
private:
	NetworkProxyMgr(void);
	~NetworkProxyMgr(void);

	void UpdateProxy(QNetworkProxy::ProxyType type, const QString & hostName, quint16 port ,const QString & user, const QString & password)
	{
		QNetworkProxy proxy;
		proxy.setType(type);
		proxy.setHostName(hostName);
		proxy.setPort(port);
		proxy.setUser(user);
		proxy.setPassword(password);
		QNetworkProxy::setApplicationProxy(proxy);
	}

};
