#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>

class QGetopt {
	struct Option;
public:
	struct GetoptException {
		QChar option;
		QString longOption;
		GetoptException(const Option & o) : option(o.option), longOption(o.longOption) {}
		virtual ~GetoptException() {}
	};
	struct NoArgException : GetoptException {
		NoArgException(const Option & o) : GetoptException(o) {}
	};
	struct UnknownOptionException : GetoptException {
		UnknownOptionException(const Option & o) : GetoptException(o) {}
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
	struct Option {
		QChar option;
		QString longOption;
		bool withArg;
		QStringList args;
		int count;

		Option();
		explicit Option(const QChar & _option, int _withArg = false);
		explicit Option(const QString & _longOption, int _withArg = false);
		explicit Option(const QChar & _option, const QString & _longOption, int _withArg = false);
		operator bool() const { return !option.isNull() || !longOption.isEmpty(); }
		bool operator==(const Option & other) const;
		bool operator!=(const Option & other) const { return !(*this == other); }
	};
	QList<Option> options, foundOptions;
	QStringList nonArgs;

	Option & getOption(const Option & option);
	const Option & getOption(const Option & option) const;
	QString getOptionString() const;

	friend class LongOpts;
	friend class TestQGetopt;
};
