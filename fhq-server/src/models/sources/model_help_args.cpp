#include <model_help_args.h>
#include <iostream>
#include <algorithm>
#include <iomanip> // setfill, setw 

// ---------------------------------------------------------------------

HelpArg::HelpArg(const std::string &sName, const std::string &sShortName, const std::string &sDescription) {
	m_sName = sName;
	m_sShortName = sShortName;
	m_sDescription = sDescription;
	m_bExists = false;
}

// ---------------------------------------------------------------------

HelpArg::HelpArg() {
	m_sName = "";
	m_sShortName = "";
	m_sDescription = "";
	m_bExists = false;
}

// ---------------------------------------------------------------------

std::string HelpArg::getName(){
	return m_sName;
}

// ---------------------------------------------------------------------

std::string HelpArg::getShortName(){
	return m_sShortName;
}

// ---------------------------------------------------------------------

std::string HelpArg::getDescription(){
	return m_sDescription;
}

// ---------------------------------------------------------------------

bool HelpArg::isExists(){
	return m_bExists;
}

// ---------------------------------------------------------------------

HelpArgs::HelpArgs(int argc, char** argv){
    for(int i = 0; i < argc; i++){
		m_vArgs.push_back(std::string(argv[i]));
	}
}

// ---------------------------------------------------------------------

bool HelpArgs::has(const std::string &sName){
	return std::find(m_vArgs.begin(), m_vArgs.end(), sName) != m_vArgs.end();
}

// ---------------------------------------------------------------------

void HelpArgs::addHelp(HelpArg ha){
	// TODO check existing this argument
	m_vHelpArgs.push_back(ha);
}

// ---------------------------------------------------------------------

void HelpArgs::printHelp(){
	// TODO calculate max size
	int nMaxSize = 0;
	for(int i = 0; i < m_vHelpArgs.size(); i++){
		HelpArg ha = m_vHelpArgs[i];
		std::string sTmp = ha.getName() + ", " + ha.getShortName();
		nMaxSize = std::max(nMaxSize, (int)sTmp.size());
	}
	nMaxSize += 4;

	std::cout << "Usage: " << m_vArgs.at(0) << "   [PARAM]\n";
	for(int i = 0; i < m_vHelpArgs.size(); i++){
		HelpArg ha = m_vHelpArgs[i];
		std::string sTmp = ha.getName() + ", " + ha.getShortName();
		int nIntent = nMaxSize - sTmp.size();

		std::cout << "\t " << sTmp;
		for(int x = 0; x < nIntent; x++){
			std::cout << " ";
		}
		std::cout << ha.getDescription() << "\n";
	}

	std::cout << "\n";
}