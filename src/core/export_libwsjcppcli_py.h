#ifndef EXPORT_LIBWSJCPPCLI_PY_H
#define EXPORT_LIBWSJCPPCLI_PY_H

#include <string>
#include <vector>

struct ApiGroup {
  ApiGroup(std::string sGroupName, std::string sGroupNameCammelCase) {
    this->sGroupName = sGroupName;
    this->sGroupNameCammelCase = sGroupNameCammelCase;
    this->sClassname = "FreeHackQuestApi" + sGroupNameCammelCase;
    this->sFilename = "freehackquest_api_" + sGroupName;
  }
  std::string sGroupName;
  std::string sGroupNameCammelCase;
  std::string sClassname;
  std::string sFilename;
};

class ExportLibWsjCppCliPy {
public:
  ExportLibWsjCppCliPy();
  void exportLib();

private:
  void exportSetupPy();
  void exportAPImd();
  std::string convertCammelCaseToSnakCase(const std::string &sName, bool bSilent);
  void export__init__py();
  void exportApiGroupsPy();
  void exportClientPy();
  void exportPrepareDirs();

  std::string TAG;
  long m_nCurrentTimeInSec;
  std::string m_sCurrentDate;
  std::vector<ApiGroup> m_vGroups;
};

#endif // EXPORT_LIBWSJCPPCLI_PY_H
