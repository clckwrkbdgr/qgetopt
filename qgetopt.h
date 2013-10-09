#pragma once
#include <QtCore/QString>

class QGetopt {
public:
	struct NoArgException {
		QChar option;
		QString longOption;
	};
	struct UnknownOptionException {
		QChar option;
		QString longOption;
	};

	QGetopt();
	QGetopt(const QGetopt & other);
	QGetopt & operator=(const QGetopt & other);
	virtual ~QGetopt();

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
};
