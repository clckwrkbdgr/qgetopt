#include "qgetopt.h"
#include <QtTest>

class TestQGetopt : public QObject {
	Q_OBJECT
private slots:
void option()
{
	QCOMPARE(QGetopt::Option('a').option, QChar('a'));
	QCOMPARE(QGetopt::Option('a').longOption, QString());
	QCOMPARE(QGetopt::Option('a', true).option, QChar('a'));
	QCOMPARE(QGetopt::Option('a', false).longOption, QString());
	QCOMPARE(QGetopt::Option('a', true).withArg, true);
	QCOMPARE(QGetopt::Option('a', false).withArg, false);

	QCOMPARE(QGetopt::Option("add").option, QChar());
	QCOMPARE(QGetopt::Option("add").longOption, QString("add"));
	QCOMPARE(QGetopt::Option("add", true).option, QChar());
	QCOMPARE(QGetopt::Option("add", false).longOption, QString("add"));
	QCOMPARE(QGetopt::Option("add", true).withArg, true);
	QCOMPARE(QGetopt::Option("add", false).withArg, false);

	QCOMPARE(QGetopt::Option('a', "add").option, QChar('a'));
	QCOMPARE(QGetopt::Option('a', "add").longOption, QString("add"));
	QCOMPARE(QGetopt::Option('a', "add", true).option, QChar('a'));
	QCOMPARE(QGetopt::Option('a', "add", false).longOption, QString("add"));
	QCOMPARE(QGetopt::Option('a', "add", true).withArg, true);
	QCOMPARE(QGetopt::Option('a', "add", false).withArg, false);
}
void optionEqual()
{
	QVERIFY(QGetopt::Option('a') == QGetopt::Option('a'));
	QVERIFY(QGetopt::Option('a') != QGetopt::Option('b'));

	QVERIFY(QGetopt::Option("add") == QGetopt::Option("add"));
	QVERIFY(QGetopt::Option("count") != QGetopt::Option("add"));

	QVERIFY(QGetopt::Option('a') != QGetopt::Option("add"));
	QVERIFY(QGetopt::Option("add") != QGetopt::Option('a'));

	QVERIFY(QGetopt::Option('a', "add") == QGetopt::Option('a'));
	QVERIFY(QGetopt::Option('a', "add") == QGetopt::Option("add"));
	QVERIFY(QGetopt::Option('a', "add") == QGetopt::Option('a', "add"));
	QVERIFY(QGetopt::Option('a', "add") != QGetopt::Option('b'));
	QVERIFY(QGetopt::Option('a', "add") != QGetopt::Option("count"));

	QVERIFY(QGetopt::Option('a', "add") != QGetopt::Option('b', "add"));
	QVERIFY(QGetopt::Option('a', "add") != QGetopt::Option('a', "count"));
}
void shortOptions()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption('c');
	getopt.addOption('d');

	getopt.parse(QStringList() << "./test" << "-a" << "-c");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "./test" << "-c" << "-a");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "./test" << "-a" << "-d");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption('c'));
	QVERIFY(getopt.hasOption('d'));

	getopt.parse(QStringList() << "./test" << "-ac");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "./test" << "-ca");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "./test" << "-caaa");
	QVERIFY(getopt.hasOption('a'));
	QCOMPARE(getopt.count('a'), 3);
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.count('c'), 1);
}
void optargs()
{
	QGetopt getopt;
	getopt.addOptionWithArg('a').addOption('b').addOptionWithArg('c');

	getopt.parse(QStringList() << "./test" << "-a" << "1" << "-b" << "-c" << "2");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('b'));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('a'), QString("1"));
	QCOMPARE(getopt.getArg('c'), QString("2"));

	getopt.parse(QStringList() << "./test" << "-a" << "1" << "-bc" << "2");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('b'));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('a'), QString("1"));
	QCOMPARE(getopt.getArgs('a'), QStringList() << "1");
	QCOMPARE(getopt.getArg('c'), QString("2"));

	getopt.parse(QStringList() << "./test" << "-a" << "1" << "-a" << "2");
	QVERIFY(getopt.hasOption('a'));
	QCOMPARE(getopt.count('a'), 2);
	QCOMPARE(getopt.getArg('a'), QString("1"));
	QCOMPARE(getopt.getArgs('a'), QStringList() << "1" << "2");
}
void optWithoutOptarg()
{
	QGetopt getopt;
	getopt.addOptionWithArg('a');
	getopt.addOption('b');
	getopt.addOptionWithArg('c');

	try {
		getopt.parse(QStringList() << "./test" << "-b" << "-c" << "2" << "-a");
		QFAIL("No exception");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, QChar('a'));
	}

	try {
		getopt.parse(QStringList() << "./test" << "-a" << "1" << "-bc");
		QFAIL("No exception");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, QChar('c'));
	}
}
void unknownOption()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption('b');
	getopt.addOptionWithArg('c');

	try {
		getopt.parse(QStringList() << "./test" << "-a" << "-d" << "-c" << "2");
		QFAIL("No exception");
	} catch(QGetopt::UnknownOptionException & e) {
		QCOMPARE(e.option, QChar('d'));
	}
}
void longOptions()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption("add");
	getopt.addOption('c', "count");

	getopt.parse(QStringList() << "./test" << "-a" << "--count");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption("add"));
	QVERIFY(getopt.hasOption("count"));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "./test" << "-a" << "--count" << "--add");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("add"));
	QVERIFY(getopt.hasOption("count"));
}
void longOptionsWithArg()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOptionWithArg("add");
	getopt.addOptionWithArg('c', "count");

	getopt.parse(QStringList() << "./test" << "-a" << "--count" << "1");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption("add"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(getopt.hasOption("count"));
	QCOMPARE(getopt.getArg('c'), QString("1"));
	QCOMPARE(getopt.getArg("count"), QString("1"));

	getopt.parse(QStringList() << "./test" << "-a" << "--count" << "1" << "--add" << "2");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("add"));
	QVERIFY(getopt.hasOption("count"));
	QCOMPARE(getopt.getArg('c'), QString("1"));
	QCOMPARE(getopt.getArg("count"), QString("1"));
	QCOMPARE(getopt.getArg("add"), QString("2"));

	getopt.parse(QStringList() << "./test" << "--count" << "1" << "--count" << "2");
	QVERIFY(!getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption("add"));
	QVERIFY(getopt.hasOption("count"));
	QCOMPARE(getopt.count("count"), 2);
	QCOMPARE(getopt.getArg("count"), QString("1"));
	QCOMPARE(getopt.getArgs("count"), QStringList() << "1" << "2");
}
void longOptionsWithoutArg()
{
	QGetopt getopt;
	getopt.addOptionWithArg("add");
	getopt.addOption('b');
	getopt.addOptionWithArg('c', "count");

	try {
		getopt.parse(QStringList() << "./test" << "--add");
		QFAIL("No exception");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, QChar());
		QCOMPARE(e.longOption, QString("add"));
	}

	try {
		getopt.parse(QStringList() << "./test" << "--add" << "1" << "-bc");
		QFAIL("No exception");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, QChar('c'));
		QCOMPARE(e.longOption, QString("count"));
	}

	try {
		getopt.parse(QStringList() << "./test" << "--add" << "1" << "-b" << "--count");
		QFAIL("No exception");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, QChar('c'));
		QCOMPARE(e.longOption, QString("count"));
	}
}
void unknownLongOption()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption("batch");
	getopt.addOptionWithArg('c', "count");

	try {
		getopt.parse(QStringList() << "./test" << "-a" << "--delete" << "--count" << "2");
		QFAIL("No exception");
	} catch(QGetopt::UnknownOptionException & e) {
		QCOMPARE(e.option, QChar());
		QCOMPARE(e.longOption, QString("delete"));
	}
}
void nonOptionArgs()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption('3');
	getopt.addOption('4');
	getopt.addOption("batch");
	getopt.addOptionWithArg('c', "count");

	getopt.parse(QStringList() << "./test" << "-a" << "--batch" << "--count" << "2" << "3" << "4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList() << QString("3") << QString("4"));

	getopt.parse(QStringList() << "./test" << "-a" << "--batch" << "--count" << "2" << "3" << "-4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(getopt.hasOption('4'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList() << QString("3"));

	getopt.parse(QStringList() << "./test" << "-a" << "--batch" << "--count" << "2" << "-3" << "-4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(getopt.hasOption('3'));
	QVERIFY(getopt.hasOption('4'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList());

	getopt.parse(QStringList() << "./test" << "-a" << "--batch" << "--count" << "2" << "--" << "-3" << "-4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(!getopt.hasOption('3'));
	QVERIFY(!getopt.hasOption('4'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList() << QString("-3") << QString("-4"));
}
};

QTEST_MAIN(TestQGetopt);
#include "test.moc"
