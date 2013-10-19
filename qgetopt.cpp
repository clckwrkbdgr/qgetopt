#include "qgetopt.h"
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QtDebug>
#include <unistd.h>

bool QGetopt::Option::operator==(const Option & other)
{
	if(other.option == option) {
		return true;
	}
	return false;
}

void QGetopt::addOption(const QChar & shortOption)
{
	options << Option(shortOption);
}

void QGetopt::addOption(const QString & /*longOption*/)
{
}

void QGetopt::addOption(const QChar & /*shortOption*/, const QString & /*longOption*/)
{
}

void QGetopt::addOptionWithArg(const QChar & shortOption)
{
	options << Option(shortOption, true);
}

void QGetopt::addOptionWithArg(const QString & /*longOption*/)
{
}

void QGetopt::addOptionWithArg(const QChar & /*shortOption*/, const QString & /*longOption*/)
{
}

struct Args {
	int argc;
	char ** argv;
	Args(const QStringList & arguments)
	{
		argc = arguments.count();
		argv = new char*[argc];
		for(int i = 0; i < argc; ++i) {
			QByteArray data = arguments[i].toAscii();
			argv[i] = new char[data.size() + 1];
			strcpy(argv[i], data.constData());
		}
	}
	~Args()
	{
		for(int i = 0; i < argc; ++i) {
			delete[] argv[i];
		}
		delete[] argv;
	}
};

QString QGetopt::getOptionString() const
{
	QString optionString;
	foreach(const Option & c, options) {
		optionString += c.option;
		if(c.withArg) {
			optionString += ":";
		}
	}
	return optionString;
}

void QGetopt::parse(const QStringList & arguments)
{
	QStringList fullCmdArgs = arguments;
	fullCmdArgs.prepend(QCoreApplication::applicationFilePath());
	Args args(fullCmdArgs);

	QString optionString = getOptionString();
	QByteArray text = optionString.toLocal8Bit();
	text.append(char(0));

	foundOptions.clear();
	optind = 0;
	opterr = 0;

	QChar c;
	while((c = getopt(args.argc, args.argv, text.data())) != -1) {
		if(c == '?') {
			if(optionString.contains(QChar(optopt))) {
				throw NoArgException(QChar(optopt));
			} else {
				throw UnknownOptionException(QChar(optopt));
			}
		}
		if(hasOption(c)) {
			getOption(c).count++;
			if(optarg) {
				getOption(c).args << QString(optarg);
			}
		} else {
			Option newOption(c);
			if(optarg) {
				newOption.args << QString(optarg);
			}
			foundOptions << newOption;
		}
	}
}

const QGetopt::Option & QGetopt::getOption(const QChar & shortOption) const
{
	int index = foundOptions.indexOf(Option(shortOption));
	if(index == -1) {
		static Option result;
		result = Option();
		return result;
	}
	return foundOptions[index];
}

QGetopt::Option & QGetopt::getOption(const QChar & shortOption)
{
	int index = foundOptions.indexOf(Option(shortOption));
	if(index == -1) {
		static Option result;
		result = Option();
		return result;
	}
	return foundOptions[index];
}

bool QGetopt::hasOption(const QChar & shortOption) const
{
	return foundOptions.contains(Option(shortOption));
}

bool QGetopt::hasOption(const QString & /*longOption*/) const
{
	return false;
}

const QString & QGetopt::getArg(const QChar & shortOption) const
{
	return getOption(shortOption).args.first();
}

const QString & QGetopt::getArg(const QString & /*longOption*/) const
{
	static QString result;
	return result;
}

const QStringList & QGetopt::getArgs(const QChar & shortOption) const
{
	return getOption(shortOption).args;
}

const QStringList & QGetopt::getArgs(const QString & /*longOption*/) const
{
	static QStringList result;
	return result;
}

int QGetopt::count(const QChar & shortOption) const
{
	return getOption(shortOption).count;
}

int QGetopt::count(const QString & /*longOption*/) const
{
	return 0;
}

const QStringList & QGetopt::getNonArgs() const
{
	static QStringList result;
	return result;
}

