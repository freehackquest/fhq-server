
#include "argument_processor_main.h"

#include <QFile>
#include <QString>
#include <QtCore>

#include <employees.h>
#include <wsjcpp_core.h>
#include <wsjcpp_yaml.h>

#include "argument_processor_api.h"
#include "argument_processor_config.h"
#include "argument_processor_database.h"
#include <employ_database.h>
#include <employ_images.h>
#include <employ_server_info.h>
#include <employees.h>
#include <http_handler_web_admin_folder.h>
#include <http_handler_web_public_folder.h>
#include <http_handler_web_user_folder.h>
#include <utils_prepare_deb_package.h>
#include <websocketserver.h>
#include <wsjcpp_light_web_server.h>
#include <wsjcpp_print_tree.h>

WsjcppLightWebServer g_httpServer;

// ---------------------------------------------------------------------
// ArgumentProcessorMain

ArgumentProcessorMain::ArgumentProcessorMain(QCoreApplication *pQtApp)
    : WsjcppArgumentProcessor({"main"}, "FreeHackQuest Server", "FreeHackQuest Server") {
  TAG = "ArgumentProcessorMain";
  // registrySingleArgument("--single", "What exactly do this single param?");
  registryParameterArgument("--workdir", "<path>", "workdir");
  registryParameterArgument("-wd", "<path>", "workdir (short)");
  registryParameterArgument("--init-default-data-for-containers", "<path>",
                            "Init all basic data for containers (nginx, fhq data, ssl)");

  // registryExample("here example of command");
  registryProcessor(new ArgumentProcessorVersion());
  registryProcessor(new ArgumentProcessorStart(pQtApp));
  registryProcessor(new ArgumentProcessorDatabase());
  registryProcessor(new ArgumentProcessorConfig());
  registryProcessor(new ArgumentProcessorApi());
  registryProcessor(new ArgumentProcessorShowEmployees());
  registryProcessor(new ArgumentProcessorPrepareDeb());

  m_sWorkDir = "";
  std::vector<std::string> vDefaultsWorkdirs = {WsjcppCore::getCurrentDirectory() + "./data/",
                                                WsjcppCore::getCurrentDirectory() + "./ci/travis/data/",
                                                "/usr/share/fhq-server/data"};
  for (int i = 0; i < vDefaultsWorkdirs.size(); i++) {
    if (WsjcppCore::dirExists(vDefaultsWorkdirs[i])) {
      m_sWorkDir = vDefaultsWorkdirs[i];
    }
  }
}

bool ArgumentProcessorMain::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
  WsjcppLog::err(TAG, "Not implemented");
  return false;
}

bool ArgumentProcessorMain::applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName,
                                                   const std::string &sValue) {
  if (sArgumentName == "--workdir" || sArgumentName == "-wd") {
    return setWorkDir(sValue);
  }
  if (sArgumentName == "--init-default-data-for-containers") {
    return initDefaultDataForContainers(sValue);
  }
  return false;
}

int ArgumentProcessorMain::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
  return -10;
}

bool ArgumentProcessorMain::setWorkDir(const std::string &sWorkDir_) {
  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  std::string sWorkDir = sWorkDir_;
  if (sWorkDir[0] == '.') {
    sWorkDir = WsjcppCore::getCurrentDirectory() + sWorkDir;
  }
  sWorkDir = WsjcppCore::doNormalizePath(sWorkDir);

  std::cout << "\n Workdir: " << sWorkDir << " \n\n";
  if (!WsjcppCore::dirExists(sWorkDir)) {
    WsjcppLog::err(TAG, "Directory '" + sWorkDir + "' did not exists");
    return false;
  }

  // configure employ
  if (sWorkDir != "") {
    pGlobalSettings->setWorkDir(sWorkDir);
    pGlobalSettings->update("work_dir", sWorkDir);
  }

  std::string sDirLogs = WsjcppCore::doNormalizePath(sWorkDir + "/logs");
  if (!WsjcppCore::dirExists(sDirLogs)) {
    WsjcppCore::makeDir(sDirLogs);
    if (!WsjcppCore::dirExists(sDirLogs)) {
      WsjcppLog::err(TAG, "Could not create directory " + sDirLogs);
      return false;
    }
  }
  pGlobalSettings->update("log_dir", sDirLogs);
  WsjcppLog::setLogDirectory(sDirLogs);
  m_sWorkDir = sWorkDir;
  return true;
}

bool ArgumentProcessorMain::initDefaultDataForContainers(const std::string &sInitDir) {
  std::string _TAG = "initDefaultDataForContainers";

  // absolute path
  std::string sNormalizedInitDir = sInitDir;
  if (sNormalizedInitDir[0] == '.') {
    sNormalizedInitDir = WsjcppCore::getCurrentDirectory() + sNormalizedInitDir;
  }
  sNormalizedInitDir = WsjcppCore::doNormalizePath(sNormalizedInitDir);

  WsjcppLog::info(_TAG, "Init directory " + sNormalizedInitDir);

  // make dirs
  std::vector<std::string> vDirs;
  vDirs.push_back(sNormalizedInitDir + "/ssl");
  vDirs.push_back(sNormalizedInitDir + "/nginx/conf.d");
  // vDirs.push_back(sNormalizedInitDir + "/nginx/html/files/users");
  vDirs.push_back(sNormalizedInitDir + "/nginx/logs");
  vDirs.push_back(sNormalizedInitDir + "/data/public/games");
  vDirs.push_back(sNormalizedInitDir + "/data/public/quests");
  vDirs.push_back(sNormalizedInitDir + "/data/public/users");
  vDirs.push_back(sNormalizedInitDir + "/data/fhqjad-store");
  vDirs.push_back(sNormalizedInitDir + "/data/file_storage");
  vDirs.push_back(sNormalizedInitDir + "/data/file_storage/games");
  vDirs.push_back(sNormalizedInitDir + "/data/file_storage/quests");
  vDirs.push_back(sNormalizedInitDir + "/data/logs");

  for (int i = 0; i < vDirs.size(); i++) {
    if (!WsjcppCore::dirExists(vDirs[i])) {
      WsjcppLog::info(_TAG, "Creating directory " + vDirs[i]);
      WsjcppCore::makeDirsPath(vDirs[i]);
      if (!WsjcppCore::dirExists(vDirs[i])) {
        WsjcppLog::throw_err(_TAG, "Could not create a directory " + vDirs[i]);
      }
    }
  }
  setWorkDir(WsjcppCore::doNormalizePath(sNormalizedInitDir + "/data/"));
  initDefaultDataConfigYml(sNormalizedInitDir);
  initDefaultSsl(sNormalizedInitDir);
  initDefaultNginx(sNormalizedInitDir);

  return true;
}

void ArgumentProcessorMain::initDefaultDataConfigYml(const std::string &sNormalizedInitDir) {
  std::string sConfigYmlPath = m_sWorkDir + "/config.yml";
  WsjcppYaml yaml;
  std::string sError;
  bool bSaveYaml = false;
  if (WsjcppCore::fileExists(sConfigYmlPath)) {
    if (!yaml.loadFromFile(sConfigYmlPath, sError)) {
      WsjcppLog::throw_err(TAG, "Could not load yaml " + sError);
    }
  } else {
    std::string sWebAdminFolder = sNormalizedInitDir + "/../web-admin";
    if (!WsjcppCore::dirExists(sWebAdminFolder)) {
      sWebAdminFolder = "/usr/share/fhq-server/web-admin";
    }

    std::string sWebUserFolder = sNormalizedInitDir + "/../web-user/dist";
    if (!WsjcppCore::dirExists(sWebUserFolder)) {
      sWebUserFolder = "/usr/share/fhq-server/web-user";
    }
    std::string sDefaultConfigYml = "";
    sDefaultConfigYml += "# init default config for containers \n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "# Database Configuration\n";
    sDefaultConfigYml += "file_storage: " + sNormalizedInitDir + "/file_storage\n";
    sDefaultConfigYml += "usemysql: yes\n";
    sDefaultConfigYml += "storage_type: mysql\n";
    sDefaultConfigYml += "dbhost: 127.0.0.1\n";
    sDefaultConfigYml += "dbname: fhqserver\n";
    sDefaultConfigYml += "dbport: 3306\n";
    sDefaultConfigYml += "dbuser: fhqserver\n";
    sDefaultConfigYml += "dbpass: \"VveGJemxFb\"\n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "# Server configurations\n";
    sDefaultConfigYml += "port: 1234\n";
    sDefaultConfigYml += "ssl_on: yes\n";
    sDefaultConfigYml += "ssl_port: 4613\n";
    sDefaultConfigYml += "ssl_key_file: " + sNormalizedInitDir + "/ssl/test_fhqserver.key\n";
    sDefaultConfigYml += "ssl_cert_file: " + sNormalizedInitDir + "/ssl/test_fhqserver.crt\n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "# Web Configuration\n";
    sDefaultConfigYml += "web_port: 7080\n";
    sDefaultConfigYml += "web_max_threads: 4\n";
    sDefaultConfigYml += "web_admin_folder: " + sWebAdminFolder + "\n";
    sDefaultConfigYml += "web_user_folder: " + sWebUserFolder + "\n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "web_public_folder: " + m_sWorkDir + "/public/\n";
    sDefaultConfigYml += "web_public_folder_url: http://localhost:7080/public/\n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "# Jobs Pool Config\n";
    sDefaultConfigYml += "jobs_fast_threads: 2\n";
    sDefaultConfigYml += "jobs_slow_threads: 1\n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "# fhqjad-store\n";
    sDefaultConfigYml += "web_fhqjad_store: " + m_sWorkDir + "/fhqjad-store\n";
    sDefaultConfigYml += "\n";
    sDefaultConfigYml += "# UI configs\n";
    sDefaultConfigYml += "allow_quests_proposals: no\n";
    sDefaultConfigYml += "\n";

    if (!yaml.loadFromString("default-config.yml", sDefaultConfigYml, sError)) {
      WsjcppLog::throw_err(TAG, "Error parsing: " + sError);
    }
    bSaveYaml = true;
  }

  std::string sValue;
  if (WsjcppCore::getEnv("FREEHACKQUEST_DATABASE_HOST", sValue)) {
    yaml["dbhost"].val(sValue);
    bSaveYaml = true;
  }

  if (WsjcppCore::getEnv("FREEHACKQUEST_DATABASE_PORT", sValue)) {
    int nPort = atoi(sValue.c_str());
    yaml["dbport"].val(nPort);
    bSaveYaml = true;
  }

  if (WsjcppCore::getEnv("FREEHACKQUEST_DATABASE_NAME", sValue)) {
    yaml["dbname"].val(sValue);
    bSaveYaml = true;
  }

  if (WsjcppCore::getEnv("FREEHACKQUEST_DATABASE_USER", sValue)) {
    yaml["dbuser"].val(sValue);
    bSaveYaml = true;
  }

  if (WsjcppCore::getEnv("FREEHACKQUEST_DATABASE_PASS", sValue)) {
    yaml["dbpass"].val(sValue);
    bSaveYaml = true;
  }

  if (bSaveYaml) {
    if (!yaml.saveToFile(sConfigYmlPath, sError)) {
      WsjcppLog::throw_err(TAG, "Could not save yaml " + sError);
    }
  }
}

void ArgumentProcessorMain::initDefaultSsl(const std::string &sNormalizedInitDir) {
  std::string sTestFhqServerCrt = sNormalizedInitDir + "/ssl/test_fhqserver.crt";
  if (!WsjcppCore::fileExists(sTestFhqServerCrt)) {
    WsjcppCore::writeFile(sTestFhqServerCrt, "-----BEGIN CERTIFICATE-----\n"
                                             "MIIDazCCAlOgAwIBAgIUW2mVSJp6DAQa6X0j3TQXeCs8hOcwDQYJKoZIhvcNAQEL\n"
                                             "BQAwRTELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoM\n"
                                             "GEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDAeFw0yMTAzMjcwODA4MjFaFw0zMTAz\n"
                                             "MjUwODA4MjFaMEUxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEw\n"
                                             "HwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQwggEiMA0GCSqGSIb3DQEB\n"
                                             "AQUAA4IBDwAwggEKAoIBAQC8fAitk+xvn4MSRqwXO+dqdYnVwi/aCAf6TlE+jT4w\n"
                                             "pcKle+JWKWxJ/cgbzInXSF53TDg4Nn5mvRB8px8TiTQxoJItuuWrqUgckw0s+tca\n"
                                             "YPYctgwzwdrZAvEkCG7LVgZoFQr/3WJtJA+hqIYydu7TzGOBD326R8PblCnLScA2\n"
                                             "mvO4RjRU7TJMg+Z662PyYh33Kt4NToTjbDsVKparGTFE5q5FwDOC8VPraQzCRYB2\n"
                                             "mdj5wVhyXUDisO2qOG4UoNTB1ypcyqZBEWaxc7GBI3/Wv787ZvXlpr+owdif/bjw\n"
                                             "3TLf/NySHztJmn3xJrr05givxQC4qS6AqDAmIYh2gH9BAgMBAAGjUzBRMB0GA1Ud\n"
                                             "DgQWBBSZN819rm68JcSrcjZWDUl3w2AzaDAfBgNVHSMEGDAWgBSZN819rm68JcSr\n"
                                             "cjZWDUl3w2AzaDAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQBv\n"
                                             "ZCUIjGXzkQCK91Sbsucs+iWg7uBr9ITxG/uxhn7pPw5rCu6B3B2C0m0bXG3e0hvb\n"
                                             "vl4Lo0a1YLdWQWW8FCHziNSxqAyfs+QbK2rFA3VzrdYi9C12jff5+C6FTX0oc7Gb\n"
                                             "tMnqCLDLwUVJ6tjVMBkpjBm8TDESFh93Mw605A31VTyxKBezaSbpheqR4MI3myfI\n"
                                             "fwkKenYpgLh9/Pn6rx5LeMlzb+oBdgq87UF0aExzg+7eat62lZndBVMcAttIpFh+\n"
                                             "CtkSPjxqQ9fGmEtxnqaIJCCw8vHFA4TwkxQcEE7JnSSriSwZi254jJYEAXvxUqWj\n"
                                             "PdgOc2wwEnazLguwZm02\n"
                                             "-----END CERTIFICATE-----\n");
  }

  std::string sTestFhqServerKey = sNormalizedInitDir + "/ssl/test_fhqserver.key";
  if (!WsjcppCore::fileExists(sTestFhqServerKey)) {
    WsjcppCore::writeFile(sTestFhqServerKey, "-----BEGIN PRIVATE KEY-----\n"
                                             "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC8fAitk+xvn4MS\n"
                                             "RqwXO+dqdYnVwi/aCAf6TlE+jT4wpcKle+JWKWxJ/cgbzInXSF53TDg4Nn5mvRB8\n"
                                             "px8TiTQxoJItuuWrqUgckw0s+tcaYPYctgwzwdrZAvEkCG7LVgZoFQr/3WJtJA+h\n"
                                             "qIYydu7TzGOBD326R8PblCnLScA2mvO4RjRU7TJMg+Z662PyYh33Kt4NToTjbDsV\n"
                                             "KparGTFE5q5FwDOC8VPraQzCRYB2mdj5wVhyXUDisO2qOG4UoNTB1ypcyqZBEWax\n"
                                             "c7GBI3/Wv787ZvXlpr+owdif/bjw3TLf/NySHztJmn3xJrr05givxQC4qS6AqDAm\n"
                                             "IYh2gH9BAgMBAAECggEAcTPJfn7GVTAnZ5WcjAPFdFINq52KYdfK+tGIyBnKjSsx\n"
                                             "IXSrNnaAfFv4kIGjE/J6EgpsBY7tEcVhnow9BN21xEydHuwOPrBYCBllo2swP76W\n"
                                             "ch4AbCF00DuNGsgZyvPqux5QUeXj1IsA/GWb27lErV4nMP9d0sCBaGKvS1ILKH+V\n"
                                             "TkPlIHxpGmDoiCZEfhypIHkIpcCZKj204OIs6jAkdSEkb7aGWVHPEOAIwc+qG9hx\n"
                                             "1NdPIPOzq6iO3dwTk5TKJZHT3oCeEurWOB+GT4xGWXW4qDpvezAmWESq2LGVml0I\n"
                                             "d/chDgQbCjm25+rDcw8JeFNqQxU3J6adODL8OPIjAQKBgQDtoJG1hzhjmt/SuheE\n"
                                             "GPbA8r4NyttxFPmCBmRWng98kchblIKBBwAtfFNT3sn+spedkR2u+S/tOsCA/0Tm\n"
                                             "3CtI9SFAdkNlG4UWGXpW/ak5p0shSx2DD9ha79k5qhzce8VddBf4Is42ouC8rD/9\n"
                                             "kEMIxCdAwieeSYSSK8NeKnpTMQKBgQDLDsR0Dn6gRuYa9zbstH0/XXm0Fghk+eUg\n"
                                             "LgF7htezHhI42nGGwMCR61+siD41ShvpXDWdwGpV+BQnS2IjHwofeZCeEcNab9YT\n"
                                             "HM+0zodhU+95kAj72vQFg/eDnZ/H/W4ikHEwFKurTheQ/JEtusjCBTkKJlAr0nJ/\n"
                                             "SzdrMyhJEQKBgQCYhXSg8G74N8MsDbfkWOWgyO3GCou5VH4lwBaNJhzIlwXkCjmN\n"
                                             "ucGacG9HqPs8GMOKJkfINDdX7Fr9MVu3VmAODxDNwFrXvcc5fRbyWRRSZheqAf2q\n"
                                             "TvROwh5Kje7CcksnBcoQwb5cgUUAtQrJpNvx930+aqBlT8jgzWgzu84SMQKBgDNZ\n"
                                             "Z6qlo7XqV/RHwfb36ZhljgpRbAwE+cE6gRt4zQE5wIfqDb1y7v+3kvkxdvHI7OgF\n"
                                             "bnWXHAzjcE1Djeytw8ST3FNs0IcFMxCgP5JMjAW70RpfpzXxd7YRWj1JVgef+gL5\n"
                                             "nfX3tDXcyK7cf+YdjvkpkvQTHkEGtfek0L3mMPDRAoGARCdlVA/HlbVntq52RA3Z\n"
                                             "XENe4QDQgzl7rlTos+33Hcsb+AuJUfRLMadJk7b91h1/hJP0buIaSTzv5EgijgHh\n"
                                             "3OqwvxFoRcNrIuQgIA1vIVNBQUrF9BvriwVATGHPF5D/7pNzVY7b7cLFtKKtZ86d\n"
                                             "rLmX5kleBLTrsj4QkTqI0gU=\n"
                                             "-----END PRIVATE KEY-----\n");
  }

  std::string sTestFhqServerPub = sNormalizedInitDir + "/ssl/test_fhqserver.pub";
  if (!WsjcppCore::fileExists(sTestFhqServerPub)) {
    WsjcppCore::writeFile(sTestFhqServerPub, "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDIi6P4tNXqOG2yxTvchWk/cOmE98p"
                                             "6FknQVQkYYyTONWRhQpeotb/yhxxx6pluSGKS/PMJOtXczLXpBEBZQNi3kNjIxr27yJ"
                                             "QmniHgyE8UPLy0oCnL3Jte1JCnuBbxzUSgVLW1fIyEwbzJTUSCHox1zsSjQ1BZRhgSf"
                                             "dydatuZ+CcBaE7lVgIf7gFNm1upe/gh61AKgPRdVSp04O4Yv5ymTeLLOxPZvJ0hwwf/"
                                             "/5+cLpEcWsER8D8mkaFzf8L0KS6I/pcrjBeY3Zp06v9RbFziTM8a+IE18S83mu8Vw3t"
                                             "8hfK3C/d69dJn3y0FhZUuOuYoY4gR/E9KfdW+e68p9G/IoYnF/M129WITBHHb/dQhK6"
                                             "3fGabYe625TE0AxxqQzTZKQMDspuU4VLYYcL4C4Jjj5ukAfOa1uyca6ZN1QZB46NEed"
                                             "wxot2ufhFu/z7ZcdHeStXTJo04Vkm0BhKHLY+q76iVO/JCFweL8YAyN3FZNS1VA8bBQ"
                                             "GHr4SraAv0oiiP8= test@freehackquest\n");
  }
}

void ArgumentProcessorMain::initDefaultNginx(const std::string &sNormalizedInitDir) {
  std::string sNginxConf = sNormalizedInitDir + "/nginx/conf.d/default.conf";
  if (!WsjcppCore::fileExists(sNginxConf)) {
    WsjcppCore::writeFile(sNginxConf, "server {\n"
                                      "    listen       80;\n"
                                      "    server_name  localhost;\n"
                                      "\n"
                                      "    location = /files/ {\n"
                                      "        root   /usr/share/nginx/html/files/;\n"
                                      "    }\n"
                                      "\n"
                                      "    location / {\n"
                                      "        proxy_pass http://fhqserver:7080;\n"
                                      "        proxy_set_header Host $host;\n"
                                      "        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;\n"
                                      "        proxy_set_header X-Real-IP $remote_addr;\n"
                                      "    }\n"
                                      "\n"
                                      "    location /api-ws/ {\n"
                                      "        proxy_pass http://fhqserver:1234;\n"
                                      "        proxy_http_version 1.1;\n"
                                      "        proxy_set_header Upgrade $http_upgrade;\n"
                                      "        proxy_set_header Connection \"upgrade\";\n"
                                      "        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;\n"
                                      "    }\n"
                                      "\n"
                                      "    #error_page  404              /404.html;\n"
                                      "    # redirect server error pages to the static page /50x.html\n"
                                      "    error_page   500 502 503 504  /50x.html;\n"
                                      "    location = /50x.html {\n"
                                      "        root   /usr/share/nginx/html;\n"
                                      "    }\n"
                                      "}\n"
                                      "\n"
                                      "server {\n"
                                      "    listen              443 ssl;\n"
                                      "    server_name         localhost;\n"
                                      "    ssl_certificate     /etc/nginx/ssl/test_fhqserver.crt;\n"
                                      "    ssl_certificate_key /etc/nginx/ssl/test_fhqserver.key;\n"
                                      "    ssl_protocols       TLSv1 TLSv1.1 TLSv1.2;\n"
                                      "    ssl_ciphers         HIGH:!aNULL:!MD5;\n"
                                      "\n"
                                      "    location ^~ /files {\n"
                                      "        alias /usr/share/nginx/html/files/;\n"
                                      "    }\n"
                                      "\n"
                                      "    location /api-wss/ {\n"
                                      "        proxy_pass https://fhqserver:4613;\n"
                                      "        proxy_http_version 1.1;\n"
                                      "        proxy_set_header Upgrade $http_upgrade;\n"
                                      "        proxy_set_header Connection \"upgrade\";\n"
                                      "        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;\n"
                                      "    }\n"
                                      "\n"
                                      "    location / {\n"
                                      "        proxy_pass http://fhqserver:7080;\n"
                                      "        proxy_set_header Host $host;\n"
                                      "        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;\n"
                                      "        proxy_set_header X-Real-IP $remote_addr;\n"
                                      "    }\n"
                                      "}\n");
  }

  std::string sUsers0Png = sNormalizedInitDir + "/data/public/users/default.png";
  if (!WsjcppCore::fileExists(sUsers0Png)) {
    WsjcppResourceFile *pFile = WsjcppResourcesManager::get("./src/resources/user_default_icon.png");
    WsjcppCore::writeFile(sUsers0Png, pFile->getBuffer(), pFile->getBufferSize());
  }
}

// ---------------------------------------------------------------------
// ArgumentProcessorVersion

ArgumentProcessorVersion::ArgumentProcessorVersion()
    : WsjcppArgumentProcessor({"version", "ver", "--version"}, "Print version", "Print version") {
  TAG = "ArgumentProcessorVersion";
}

// ---------------------------------------------------------------------

int ArgumentProcessorVersion::exec(const std::vector<std::string> &vRoutes,
                                   const std::vector<std::string> &vSubParams) {
  std::cout << WSJCPP_APP_NAME << "-" << WSJCPP_APP_VERSION << "\n";
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorPrepareDeb

ArgumentProcessorPrepareDeb::ArgumentProcessorPrepareDeb()
    : WsjcppArgumentProcessor({"prepare-deb"}, "TODO Prepare Deb Package", "Prepare Deb Package") {
  TAG = "ArgumentProcessorPrepareDeb";
}

// ---------------------------------------------------------------------

int ArgumentProcessorPrepareDeb::exec(const std::vector<std::string> &vRoutes,
                                      const std::vector<std::string> &vSubParams) {
  UtilsPrepareDebPackage::prepare("", "tmpdeb");
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorShowEmployees

ArgumentProcessorShowEmployees::ArgumentProcessorShowEmployees()
    : WsjcppArgumentProcessor({"show-employees", "se"}, "Show employees", "Show employees") {
  TAG = "ArgumentProcessorShowEmployees";
}

int ArgumentProcessorShowEmployees::exec(const std::vector<std::string> &vRoutes,
                                         const std::vector<std::string> &vSubParams) {
  // dev
  WsjcppPrintTree tree("WsjcppEmployees (" + std::to_string(g_pWsjcppEmployees->size()) + ")");

  std::map<std::string, WsjcppEmployBase *>::iterator it = g_pWsjcppEmployees->begin();
  for (; it != g_pWsjcppEmployees->end(); ++it) {
    std::string sEmployName = it->first;
    WsjcppEmployBase *pEmployBase = it->second;
    tree.addChild(sEmployName);
    tree.switchToLatestChild();
    if (pEmployBase->loadAfter().size() > 0) {
      for (int i = 0; i < pEmployBase->loadAfter().size(); i++) {
        tree.addChild("after: " + pEmployBase->loadAfter().at(i));
      }
    }
    tree.switchToParent();
  }
  std::cout << tree.printTree() << std::endl;
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorStart

ArgumentProcessorStart::ArgumentProcessorStart(QCoreApplication *pQtApp)
    : WsjcppArgumentProcessor({"start", "-s"}, "Show employees", "Show employees") {
  TAG = "ArgumentProcessorStart";
  m_pQtApp = pQtApp;
}

int ArgumentProcessorStart::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  // auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  pGlobalSettings->registrySetting("web_server", "web_admin_folder")
      .dirPath("/usr/share/fhq-server/web-admin")
      .inFile();
  pGlobalSettings->registrySetting("web_server", "web_user_folder").dirPath("/usr/share/fhq-server/web-user").inFile();
  pGlobalSettings->registrySetting("web_server", "web_public_folder")
      .dirPath("/usr/share/fhq-server/fhq-web-public")
      .inFile();
  pGlobalSettings->registrySetting("web_server", "web_public_folder_url")
      .string("http://localhost:7080/public/")
      .inFile();
  pGlobalSettings->registrySetting("web_server", "web_fhqjad_store")
      .dirPath("/usr/share/fhq-server/web/fhqjad-store")
      .inFile();

  WsjcppEmployees::init({"start_server"});

  QThreadPool::globalInstance()->setMaxThreadCount(5);
  WebSocketServer *pServer = new WebSocketServer(); // here will be init settings
  if (pServer->isFailed()) {
    WsjcppLog::err(TAG, "Could not start server");
    return -1;
  }

  QObject::connect(pServer, &WebSocketServer::closed, m_pQtApp, &QCoreApplication::quit);
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  // TODO redesign to check config
  QSqlDatabase *db = pDatabase->database();
  if (!db->open()) {
    return -1;
  }

  // TODO move inside server start
  // start web server
  int nWebPort = pGlobalSettings->get("web_port").getNumberValue();
  int nWebMaxThreads = pGlobalSettings->get("web_max_threads").getNumberValue();
  std::string sWebAdminFolder = pGlobalSettings->get("web_admin_folder").getDirPathValue();
  std::string sWebUserFolder = pGlobalSettings->get("web_user_folder").getDirPathValue();
  std::string sWebPublicFolder =
      pGlobalSettings->get("web_public_folder").getDirPathValue(); // TODO must be declared in server
  std::string sWebPublicFolderUrl =
      pGlobalSettings->get("web_public_folder_url").getStringValue(); // TODO must be declared in server

  WsjcppLog::info(TAG, "Starting web-server on " + std::to_string(nWebPort) + " with " +
                           std::to_string(nWebMaxThreads) + " worker threads");

  g_httpServer.addHandler(new HttpHandlerWebAdminFolder(sWebAdminFolder));
  g_httpServer.addHandler(new HttpHandlerWebPublicFolder(sWebPublicFolder));
  g_httpServer.addHandler(new HttpHandlerWebUserFolder(sWebUserFolder));

  g_httpServer.setPort(nWebPort);
  g_httpServer.setMaxWorkers(nWebMaxThreads);
  g_httpServer.start(); // will be block thread

  return m_pQtApp->exec();
}
