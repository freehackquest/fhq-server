/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include <QFile>
#include <QSysInfo>
#include <QTextStream>
#include <QtCore>
#include <iostream>
#include <utils_prepare_deb_package.h>

void UtilsPrepareDebPackage::prepare(QString /*repo*/, QString tmpdeb) {
  std::cout << "Distribution: " + QSysInfo::productType().toStdString() + "\n";
  std::cout << "Distribution version: " + QSysInfo::productVersion().toStdString() + "\n";

  QFile file(tmpdeb + "/DEBIAN/control");
  if (file.exists()) {
    file.remove();
  }
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);
    stream << "Source: " << WSJCPP_APP_NAME << endl;
    stream << "Section: misc" << endl;
    stream << "Priority: optional" << endl;
    // TODO redesign in static method
    stream << "Maintainer: Evgenii Sopov <mrseakg@gmail.com>" << endl;
    // TODO different for ubuntu (default-mysql-server) and for debian
    // (mysql-server)
    QStringList depends;
    if (QSysInfo::productType() == "ubuntu") {
      depends << "default-mysql-server";
    } else if (QSysInfo::productType() == "debian") {
      depends << "mysql-server";
    } else {
      depends << "mysql-server";
    }
    depends << "libqt5websockets5"
            << "libqt5network5"
            << "libqt5sql5"
            << "libqt5sql5-mysql"
            << "libqt5core5a";
    depends << "libc6"
            << "libstdc++6"
            << "libgcc1"
            << "zlib1g"
            << "libicu52"
            << "libglib2.0-0"
            << "libpcre3";

    stream << "Depends: " << depends.join(", ") << endl;

    // Version
    stream << "Version: " << WSJCPP_APP_VERSION << endl;
    std::cout << "Version: " << WSJCPP_APP_VERSION << "\n";

    // TODO calculate correct installed size
    stream << "Installed-Size: 1024" << endl;
    stream << "Homepage: https://github.com/freehackquest/backend" << endl;

    // Package
    stream << "Package: " << WSJCPP_APP_NAME << endl;
    std::cout << "Package: " << std::string(WSJCPP_APP_NAME) << "\n";

    // Architecture
    if (QSysInfo::currentCpuArchitecture() == "x86_64") {
      stream << "Architecture: amd64" << endl;
      std::cout << "Architecture: amd64\n";
    } else if (QSysInfo::currentCpuArchitecture() == "i386") {
      stream << "Architecture: i386" << endl;
      std::cout << "Architecture: i386\n";
    } else {
      stream << "Architecture: all" << endl;
      std::cout << "Architecture: all\n";
    }

    stream << "Description: Backend for FreeHackQuest" << endl;
    stream << "  This is an open source platform for competitions in computer "
              "security."
           << endl;
  }
}
