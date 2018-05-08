#ifndef MODEL_HELP_ARGS_H
#define MODEL_HELP_ARGS_H

#include <string>
#include <vector>

class HelpArg {
	private:
		std::string m_sName;
		std::string m_sShortName;
		std::string m_sDescription;
		bool m_bExists;
		
	public:
		HelpArg(const std::string &sName, const std::string &sShortName, const std::string &sDescription);
		HelpArg();
		std::string getName();
		std::string getShortName();
		std::string getDescription();
		bool isExists();
};

class HelpArgs {
	private:
		std::vector<std::string> m_vArgs;
		std::vector<HelpArg> m_vHelpArgs;
	public:
		HelpArgs(int argc, char** argv);
		bool has(const std::string &sName);
		void addHelp(HelpArg ha);
		void printHelp();
};

#endif // MODEL_HELP_ARGS_H
