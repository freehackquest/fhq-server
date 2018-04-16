#include <log.h>

#include <iostream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

// Last log messages
std::deque<std::string> *g_LAST_LOG_MESSAGES = NULL;
std::mutex *g_LOG_MUTEX = NULL;


void Log::info(QString tag, QString msg){ // deprecated
    Log::add("INFO",tag.toStdString(), msg.toStdString());
}

// ---------------------------------------------------------------------

void Log::info(const std::string & sTag, const std::string &sMessage){
    Log::add("INFO",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(QString tag, QString msg){ // deprecated
    Log::add("ERR",tag.toStdString(), msg.toStdString());
}

// ---------------------------------------------------------------------

void Log::err(const std::string & sTag, const std::string &sMessage){
    Log::add("ERR",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(QString tag, QAbstractSocket::SocketError socketError){ // deprecated
	QString msg = "Unknown error";
	if(socketError == QAbstractSocket::ConnectionRefusedError){
		msg = "QAbstractSocket::ConnectionRefusedError, The connection was refused by the peer (or timed out).";
	}else if(socketError == QAbstractSocket::RemoteHostClosedError){
		msg = "QAbstractSocket::RemoteHostClosedError, The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.";
	}else if(socketError == QAbstractSocket::HostNotFoundError){
		msg = "QAbstractSocket::HostNotFoundError, The host address was not found.";
	}else if(socketError == QAbstractSocket::SocketAccessError){
		msg = "QAbstractSocket::SocketAccessError, The socket operation failed because the application lacked the required privileges.";
	}else if(socketError == QAbstractSocket::SocketResourceError){
		msg = "QAbstractSocket::SocketResourceError, The local system ran out of resources (e.g., too many sockets).";
	}else if(socketError == QAbstractSocket::SocketTimeoutError){
		msg = "QAbstractSocket::SocketTimeoutError, The socket operation timed out.";
	}else if(socketError == QAbstractSocket::DatagramTooLargeError){
		msg = "QAbstractSocket::DatagramTooLargeError, The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
	}else if(socketError == QAbstractSocket::NetworkError){
		msg = "QAbstractSocket::NetworkError, An error occurred with the network (e.g., the network cable was accidentally plugged out).";
	}else if(socketError == QAbstractSocket::AddressInUseError){
		msg = "QAbstractSocket::AddressInUseError, The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.";
	}else if(socketError == QAbstractSocket::SocketAddressNotAvailableError){
		msg = "QAbstractSocket::SocketAddressNotAvailableError, The address specified to QAbstractSocket::bind() does not belong to the host.";
	}else if(socketError == QAbstractSocket::UnsupportedSocketOperationError){
		msg = "QAbstractSocket::UnsupportedSocketOperationError, The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
	}else if(socketError == QAbstractSocket::ProxyAuthenticationRequiredError){
		msg = "QAbstractSocket::ProxyAuthenticationRequiredError, The socket is using a proxy, and the proxy requires authentication.";
	}else if(socketError == QAbstractSocket::SslHandshakeFailedError){
		msg = "QAbstractSocket::SslHandshakeFailedError, The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)";
	}else if(socketError == QAbstractSocket::UnfinishedSocketOperationError){
		msg = "QAbstractSocket::UnfinishedSocketOperationError, Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
	}else if(socketError == QAbstractSocket::ProxyConnectionRefusedError){
		msg = "QAbstractSocket::ProxyConnectionRefusedError, Could not contact the proxy server because the connection to that server was denied";
	}else if(socketError == QAbstractSocket::ProxyConnectionClosedError){
		msg = "QAbstractSocket::ProxyConnectionClosedError, The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)";
	}else if(socketError == QAbstractSocket::ProxyConnectionTimeoutError){
		msg = "QAbstractSocket::ProxyConnectionTimeoutError, The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
	}else if(socketError == QAbstractSocket::ProxyNotFoundError){
		msg = "QAbstractSocket::ProxyNotFoundError, The proxy address set with setProxy() (or the application proxy) was not found.";
	}else if(socketError == QAbstractSocket::ProxyProtocolError){
		msg = "QAbstractSocket::ProxyProtocolError, The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.";
	}else if(socketError == QAbstractSocket::OperationError){
		msg = "QAbstractSocket::OperationError, An operation was attempted while the socket was in a state that did not permit it.";
	}else if(socketError == QAbstractSocket::SslInternalError){
		msg = "QAbstractSocket::SslInternalError, The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.";
	}else if(socketError == QAbstractSocket::SslInvalidUserDataError){
		msg = "QAbstractSocket::SslInvalidUserDataError, Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.";
	}else if(socketError == QAbstractSocket::TemporaryError){
		msg = "QAbstractSocket::TemporaryError, A temporary error occurred (e.g., operation would block and socket is non-blocking).";
	}else if(socketError == QAbstractSocket::UnknownSocketError){
		msg = "QAbstractSocket::UnknownSocketError, An unidentified error occurred.";
	}

    Log::add("ERR",tag.toStdString(), msg.toStdString());
}

// ---------------------------------------------------------------------

void Log::warn(QString tag, QString msg){ // deprecated
    Log::add("WARN",tag.toStdString(), msg.toStdString());
}

// ---------------------------------------------------------------------

void Log::warn(const std::string & sTag, const std::string &sMessage){
    Log::add("WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::setdir(const std::string &sDirectoryPath){
    g_LOG_DIR_PATH = sDirectoryPath;
}

// ---------------------------------------------------------------------

void Log::initGlobalVariables(){
    // create deque if not created
    if(g_LAST_LOG_MESSAGES == NULL){
        g_LAST_LOG_MESSAGES = new std::deque<std::string>();
        // std::cout << Log::currentTime() + ", " + Log::threadId() + " Init last messages deque\r\n";
    }
    // create mutex if not created
    if(g_LOG_MUTEX == NULL){
        g_LOG_MUTEX = new std::mutex();
        // std::cout << Log::currentTime() + ", " + Log::threadId() + " Init mutex for log\r\n";
    }
}

// ---------------------------------------------------------------------

std::string Log::currentTime(){
    // milleseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }

    // datetime
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X.", &tstruct);
    return std::string(buf) + std::to_string(millisec);
}

// ---------------------------------------------------------------------

std::string Log::threadId(){
    std::thread::id this_id = std::this_thread::get_id();
    std::stringstream stream;
    stream << std::hex << this_id;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

nlohmann::json Log::last_logs(){
    Log::initGlobalVariables();
    g_LOG_MUTEX->lock();
    auto lastLogMessages = nlohmann::json::array();
    int len = g_LAST_LOG_MESSAGES->size();
    for(int i = 0; i < len; i++){
        lastLogMessages.push_back(g_LAST_LOG_MESSAGES->at(i));
    }
    g_LOG_MUTEX->unlock();
    return lastLogMessages;
}

// ---------------------------------------------------------------------

void Log::add(const std::string &sType, const std::string &sTag, const std::string &sMessage){
    Log::initGlobalVariables();

    g_LOG_MUTEX->lock();
     // TODO write to file
    std::string sLogMessage = Log::currentTime() + ", " + Log::threadId() + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << sLogMessage << "\r\n";

    g_LAST_LOG_MESSAGES->push_front(sLogMessage);
    while(g_LAST_LOG_MESSAGES->size() > 50){
        g_LAST_LOG_MESSAGES->pop_back();
	}
    g_LOG_MUTEX->unlock();
}

// ---------------------------------------------------------------------
