#include <logger.h>
#include <iostream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <ostream>
#include <fallen.h>

// Last log messages
std::deque<std::string> *g_LAST_LOG_MESSAGES = NULL;
std::mutex *g_LOG_MUTEX = NULL;

// ---------------------------------------------------------------------

void Log::info(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier def(Color::FG_DEFAULT);
    Log::add(def, "INFO",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier red(Color::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::warn(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier yellow(Color::FG_YELLOW);
    Log::add(yellow, "WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::ok(const std::string &sTag, const std::string &sMessage) {
    Color::Modifier green(Color::FG_GREEN);
    Log::add(green, "OK", sTag, sMessage);
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

nlohmann::json Log::last_logs() {
    Log::initGlobalVariables();
    std::lock_guard<std::mutex> lock(*g_LOG_MUTEX);
    auto lastLogMessages = nlohmann::json::array();
    int len = g_LAST_LOG_MESSAGES->size();
    for(int i = 0; i < len; i++){
        lastLogMessages.push_back(g_LAST_LOG_MESSAGES->at(i));
    }
    return lastLogMessages;
}

// ---------------------------------------------------------------------

void Log::add(Color::Modifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage){
    Log::initGlobalVariables();

    std::lock_guard<std::mutex> lock(*g_LOG_MUTEX);
    Color::Modifier def(Color::FG_DEFAULT);

    std::string sLogMessage = Fallen::currentTime_logformat() + ", " + Log::threadId() + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;

    g_LAST_LOG_MESSAGES->push_front(sLogMessage);
    while(g_LAST_LOG_MESSAGES->size() > 50){
        g_LAST_LOG_MESSAGES->pop_back();
    }
    
    // TODO write to file
    /*std::ofstream logFile(g_LOG_FILE, std::ios::app);
    if (!logFile) {
        std::cout << "Error Opening File" << std::endl;
        return;
    }

    logFile << sLogMessage << std::endl;
    logFile.close();
    */
}

// ---------------------------------------------------------------------
