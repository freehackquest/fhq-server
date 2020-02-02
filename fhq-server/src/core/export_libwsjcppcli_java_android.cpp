#include <export_libwsjcppcli_java_android.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <wsjcpp_employees.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip> // put_time
#include <ctime>
#include <fallen.h>
#include <sstream>

// ---------------------------------------------------------------------

class PyCodeLine{
    
    PyCodeLine *m_pParent;
    std::string m_sLine;
    std::vector<PyCodeLine *> m_vLines;
public:
    
    PyCodeLine() {
        m_pParent = NULL;
        m_sLine = "";
    }

    PyCodeLine(PyCodeLine *parent, const std::string &sLine) {
        m_pParent = parent;
        m_sLine = sLine;
    }

    ~PyCodeLine() {
        if (m_pParent == NULL) {
            std::cout << "destruct root \n";
        } else {
            std::cout << "destruct something else [" << m_sLine << "]\n";
        }
    }

    PyCodeLine *addLine(const std::string &sLine) {
        PyCodeLine *pPyCodeLine = new PyCodeLine(this,sLine);
        m_vLines.push_back(pPyCodeLine);
        return pPyCodeLine;
    }

    PyCodeLine *getParent() {
        return m_pParent;
    }

    std::string getLine() {
        return m_sLine;
    }

    PyCodeLine *findRoot() {
        if (m_pParent == NULL) {
            return this;
        }
        return m_pParent->findRoot();
    }

    void print(std::ofstream &__init__, std::string intent = "") {
        if (m_pParent != NULL) {
            __init__ << intent << m_sLine << std::endl;
            intent += "    ";
        }
        for (int i = 0; i < m_vLines.size(); i++) {
            m_vLines[i]->print(__init__, intent);
        }
    }
};

// ---------------------------------------------------------------------

ExportLibWsjCppCliJavaAndroid::ExportLibWsjCppCliJavaAndroid() {
    m_sVersion = "";
    TAG = "ExportLibWsjCppCliJavaAndroid";
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliJavaAndroid::setVersion(const std::string &sVersion) {
    m_sVersion = sVersion;
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliJavaAndroid::exportLib() {
    exportPrepareDirs();
    exportManifest();
    exportBuildGradle();
    exportAPImd();
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliJavaAndroid::exportPrepareDirs() {
    int status;
    std::vector<std::string> vDirectories;
    vDirectories.push_back("libfhqcli-java-android");
    vDirectories.push_back("libfhqcli-java-android/libfhqcli");
    vDirectories.push_back("libfhqcli-java-android/libfhqcli/src");
    vDirectories.push_back("libfhqcli-java-android/libfhqcli/src/main");

    for (int i = 0; i < vDirectories.size(); i++) {
        std::string sDir = vDirectories[i];
        WSJCppLog::info(TAG, "Try create dir " + sDir);
        if (WSJCppCore::makeDir(sDir)) {
            WSJCppLog::ok(TAG, "Created dir " + sDir);
        } else {
            WSJCppLog::warn(TAG, "Could not create dir " + sDir);
        }
    }
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliJavaAndroid::exportManifest() {
    std::ofstream fManifest;
    WSJCppLog::info(TAG, "write code to libfhqcli-java-android/libfhqcli/src/main/AndroidManifest.xml");
    fManifest.open ("libfhqcli-java-android/libfhqcli/src/main/AndroidManifest.xml");

    fManifest << ""
        "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\" \n"
        "package=\"com.freehackquest.libfhqcli\"> \n"
        "<application> \n"
        "<service android:name=\".FHQClient\" /> \n"
        "</application> \n"
        "</manifest> \n";
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliJavaAndroid::exportBuildGradle() {
    std::ofstream fBuildGradle;
    WSJCppLog::info(TAG, "write code to libfhqcli-java-android/libfhqcli/build.gradle ");
    fBuildGradle.open ("libfhqcli-java-android/libfhqcli/build.gradle");

    // TODO version code must be date + time
    fBuildGradle << ""
        "apply plugin: 'com.android.library' \n"
        "\n"
        "android {\n"
        "    compileSdkVersion 28\n"
        "\n"
        "    defaultConfig {\n"
        "        minSdkVersion 15\n"
        "        targetSdkVersion 28\n"
        "        versionCode 1\n"
        "        versionName \"" + m_sVersion + "\"\n"
        "    }\n"
        "\n"
        "    buildTypes {\n"
        "        release {\n"
        "            minifyEnabled false\n"
        "            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'\n"
        "        }\n"
        "    }\n"
        "}\n"
        "\n"
        "dependencies {\n"
        "    implementation fileTree(dir: 'libs', include: ['*.jar'])\n"
        "    implementation 'com.android.support:appcompat-v7:28.0.0'\n"
        "    implementation 'com.neovisionaries:nv-websocket-client:2.8'\n"
        "}\n"
    ;
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliJavaAndroid::exportAPImd() {
    /*
    std::ofstream apimd;
    std::cout << " * write file to libfhqcli-py/API.md" << std::endl;
    apimd.open("libfhqcli-py/API.md");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    apimd << "# FHQCli\n\n";
    apimd << " Automatically generated by fhq-server. \n";
    apimd << " * Version: " << FHQSRV_VERSION << "\n";
    apimd << " * Date: " << buffer.str() << "\n\n";
    apimd << " Example connect/disconnect:\n"
        << "```\n"
        << "from libfreehackquestclient import FreeHackQuestClient \n\n"
        << "fhq = FreeHackQuestClient('ws://localhost:1234')\n"
        << " ... \n"
        << "fhq.close()\n"
        << "```\n";
    apimd << "\n";

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        apimd
            << "<details>\n"
            << "<summary>" << sCmd << "</summary>\n\n"
            << "## " << sCmd << "\n\n";

        if (pCmdHandlerBase->description() != "") {
            apimd << pCmdHandlerBase->description() << "\n\n";
        }
        apimd 
            << "Access: unauthorized - **" << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no") << "**, "
            << " user - **" << (pCmdHandlerBase->accessUser() ? "yes" : "no") << "**, "
            << " admin - **" << (pCmdHandlerBase->accessAdmin() ? "yes" : "no") << "**\n"
            << "\n";

        apimd << " #### Input params \n\n";

        std::string pythonTemplate = "";

        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());

            apimd << " * " << inDef.getName() << " - " << inDef.getType() << ", " << inDef.getRestrict() << "; " << inDef.getDescription() << "\n";

            if (pythonTemplate != "") {
                pythonTemplate += ",\n";
            }
            if (inDef.isInteger()) {
                int nVal = 0;
                if (inDef.getName() == "onpage") {
                    nVal = 10;
                }
                pythonTemplate += "    \"" + inDef.getName() + "\": " + std::to_string(nVal);
            } else {
                pythonTemplate += "    \"" + inDef.getName() + "\": \"\"";
            }
        }
        apimd 
            << "\n\n"
            << " #### example call method \n\n ```\nresponse = fhq." + sCmd + "({\n" + pythonTemplate + "\n})\n```"
            << "\n\n</details>"
            << "\n\n";
    }

    apimd.close();
    std::cout << "\t> OK" << std::endl;
    */
}

// ---------------------------------------------------------------------

