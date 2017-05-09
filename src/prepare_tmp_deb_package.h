#ifndef PREPARE_TMP_DEB_PACKAGE_H
#define PREPARE_TMP_DEB_PACKAGE_H

#include <QString>

class PrepareTmpDebPackage {
	public:
		static QString version();
		static QString name();
		static void prepare(QString repo, QString tmpdeb);
};

#endif // PREPARE_TMP_DEB_PACKAGE_H
