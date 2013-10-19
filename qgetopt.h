#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>

class QGetopt {
public:
	struct Option {
		QChar option;
		bool withArg;
		QStringList args;
		int count;
		Option() : withArg(false), count(0) {}
		Option(const QChar & _option, bool _withArg = false) : option(_option), withArg(_withArg), count(1) {}
		bool operator==(const Option & other);
	};

	struct NoArgException {
		QChar option;
		QString longOption;
		NoArgException(const QChar & _option) : option(_option) {}
	};
	struct UnknownOptionException {
		QChar option;
		QString longOption;
		UnknownOptionException(const QChar & _option) : option(_option) {}
	};

	QGetopt() {}
	virtual ~QGetopt() {}

	void addOption(const QChar & shortOption);
	void addOption(const QString & longOption);
	void addOption(const QChar & shortOption, const QString & longOption);
	void addOptionWithArg(const QChar & shortOption);
	void addOptionWithArg(const QString & longOption);
	void addOptionWithArg(const QChar & shortOption, const QString & longOption);

	void parse(const QStringList & arguments);

	bool hasOption(const QChar & shortOption) const;
	bool hasOption(const QString & longOption) const;
	const QString & getArg(const QChar & shortOption) const;
	const QString & getArg(const QString & longOption) const;
	const QStringList & getArgs(const QChar & shortOption) const;
	const QStringList & getArgs(const QString & longOption) const;
	int count(const QChar & shortOption) const;
	int count(const QString & longOption) const;

	const QStringList & getNonArgs() const;
private:
	QList<Option> options, foundOptions;
	Option & getOption(const QChar & shortOption);
	const Option & getOption(const QChar & shortOption) const;
	QString getOptionString() const;
};
