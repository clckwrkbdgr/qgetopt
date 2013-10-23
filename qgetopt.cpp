#include "qgetopt.h"
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QtDebug>
#include <getopt.h>

QGetopt::Option::Option()
	: withArg(false), count(0)
{
}

QGetopt::Option::Option(const QChar & _option, int _withArg)
	: option(_option), withArg(_withArg), count(1)
{
}

QGetopt::Option::Option(const QString & _longOption, int _withArg)
	: longOption(_longOption), withArg(_withArg), count(1)
{
}

QGetopt::Option::Option(const QChar & _option, const QString & _longOption, int _withArg)
	: option(_option), longOption(_longOption), withArg(_withArg), count(1)
{
}

bool QGetopt::Option::operator==(const Option & other) const
{
	if(!option.isNull() && other.option == option) {
		return true;
	}
	if(!longOption.isNull() && other.longOption == longOption) {
		return true;
	}
	return false;
}

void QGetopt::addOption(const QChar & shortOption)
{
	options << Option(shortOption);
}

void QGetopt::addOption(const QString & longOption)
{
	options << Option(QChar(), longOption);
}

void QGetopt::addOption(const QChar & shortOption, const QString & longOption)
{
	options << Option(shortOption, longOption);
}

void QGetopt::addOptionWithArg(const QChar & shortOption)
{
	options << Option(shortOption, QString(), true);
}

void QGetopt::addOptionWithArg(const QString & longOption)
{
	options << Option(QChar(), longOption, true);
}

void QGetopt::addOptionWithArg(const QChar & shortOption, const QString & longOption)
{
	options << Option(shortOption, longOption, true);
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
		if(c.option.isNull()) {
			continue;
		}
		optionString += c.option;
		if(c.withArg) {
			optionString += ":";
		}
	}
	return optionString;
}

struct LongOpts {
	QVector<option> longopts;

	LongOpts(const QList<QGetopt::Option> & options)
	{
		foreach(const QGetopt::Option & o, options) {
			if(o.longOption.isEmpty()) {
				continue;
			}

			QByteArray text = o.longOption.toLocal8Bit();
			text.append(char(0));
			char * s = new char[text.size()];
			strcpy(s, text.constData());

			longopts.append(option());
			longopts.last().name = s;
			longopts.last().has_arg = o.withArg ? required_argument : no_argument;
			longopts.last().flag = 0;
			longopts.last().val = 0;
		}
		longopts.append(option());
		longopts.last().name = 0;
		longopts.last().has_arg = 0;
		longopts.last().flag = 0;
		longopts.last().val = 0;
	}
	~LongOpts()
	{
		for(int i = 0; i < longopts.size(); ++i) {
			delete[] longopts[i].name;
		}
	}
};

void QGetopt::parse(const QStringList & arguments)
{
	QStringList fullCmdArgs = arguments;
	fullCmdArgs.prepend(QCoreApplication::applicationFilePath());
	Args args(fullCmdArgs);

	QString optionString = getOptionString();
	QByteArray text = optionString.toLocal8Bit();
	text.append(char(0));

	foundOptions.clear();
	nonArgs.clear();
	optind = 0;
	opterr = 0;

	LongOpts longopts(options);
	
	QChar c;
	int indexopt = 0;
	while((c = getopt_long(args.argc, args.argv, text.data(), longopts.longopts.constData(), &indexopt)) != -1) {
		QString l = (c.isNull() || c == '?') ? QString(longopts.longopts[indexopt].name) : QString();

		if(c.isNull()) {
			if(hasOption(l)) {
				getOption(l).count++;
				if(optarg) {
					getOption(l).args << QString(optarg);
				}
			} else {
				int index = options.indexOf(Option(l));
				if(index == -1) {
					throw UnknownOptionException(Option(l));
				}
				Option newOption = options[index];
				if(optarg) {
					newOption.args << QString(optarg);
				}
				foundOptions << newOption;
			}
		} else {
			if(c == '?') {
				if(optionString.contains(QChar(optopt))) {
					int index = options.indexOf(Option(QChar(optopt)));
					throw NoArgException(options.value(index));
				} else {
					if(optopt) {
						throw UnknownOptionException(Option(QChar(optopt)));
					} else {
						l = args.argv[optind - 1];
						if(l.startsWith("--")) {
							l.remove(0, 2);
						}
						int index = options.indexOf(Option(l));
						if(index < 0) {
							throw UnknownOptionException(Option(l));
						} else {
							throw NoArgException(options[index]);
						}
					}
				}
			}
			if(hasOption(c)) {
				getOption(c).count++;
				if(optarg) {
					getOption(c).args << QString(optarg);
				}
			} else {
				int index = options.indexOf(Option(c));
				Q_ASSERT(index > -1);
				Option newOption = options[index];
				if(optarg) {
					newOption.args << QString(optarg);
				}
				foundOptions << newOption;
			}
		}
	}

	for(int i = optind; i < args.argc; ++i) {
		nonArgs << QString(args.argv[i]);
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

const QGetopt::Option & QGetopt::getOption(const QString & longOption) const
{
	int index = foundOptions.indexOf(Option(QChar(), longOption));
	if(index == -1) {
		static Option result;
		result = Option();
		return result;
	}
	return foundOptions[index];
}

QGetopt::Option & QGetopt::getOption(const QString & longOption)
{
	int index = foundOptions.indexOf(Option(QChar(), longOption));
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

bool QGetopt::hasOption(const QString & longOption) const
{
	//qDebug() << "hasoption" << longOption;
	return foundOptions.contains(Option(longOption));
	//qDebug() << "hasoption" << ok;
}

const QString & QGetopt::getArg(const QChar & shortOption) const
{
	return getOption(shortOption).args.first();
}

const QString & QGetopt::getArg(const QString & longOption) const
{
	return getOption(longOption).args.first();
}

const QStringList & QGetopt::getArgs(const QChar & shortOption) const
{
	return getOption(shortOption).args;
}

const QStringList & QGetopt::getArgs(const QString & longOption) const
{
	return getOption(longOption).args;
}

int QGetopt::count(const QChar & shortOption) const
{
	return getOption(shortOption).count;
}

int QGetopt::count(const QString & longOption) const
{
	return getOption(longOption).count;
}

const QStringList & QGetopt::getNonArgs() const
{
	return nonArgs;
}

