#include "qgetopt.h"
#include <QtTest>

class TestQGetopt : public QObject {
	Q_OBJECT
private slots:
void shortOptions()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption('c');

	getopt.parse(QStringList() << "-a" << "-c");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "-c" << "-a");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "-a" << "-d");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption('c'));
	QVERIFY(getopt.hasOption('d'));

	getopt.parse(QStringList() << "-ac");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "-ca");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('c'));

	getopt.parse(QStringList() << "-caaa");
	QVERIFY(getopt.hasOption('a'));
	QCOMPARE(getopt.count('a'), 3);
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.count('c'), 1);
}
void optargs()
{
	QGetopt getopt;
	getopt.addOptionWithArg('a');
	getopt.addOption('b');
	getopt.addOptionWithArg('c');

	getopt.parse(QStringList() << "-a" << "1" << "-b" << "-c" << "2");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('b'));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('a'), QString("1"));
	QCOMPARE(getopt.getArg('c'), QString("2"));

	getopt.parse(QStringList() << "-a" << "1" << "-bc" << "2");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption('b'));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('a'), QString("1"));
	QCOMPARE(getopt.getArgs('a'), QStringList() << "1");
	QCOMPARE(getopt.getArg('c'), QString("2"));

	getopt.parse(QStringList() << "-a" << "1" << "-a" << "2");
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

	bool found_exception = false;
	try {
		getopt.parse(QStringList() << "-a" << "-b" << "-c" << "2");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, 'a');
		found_exception = true;
	}
	QVERIFY(found_exception);

	found_exception = true;
	try {
		getopt.parse(QStringList() << "-a" << "1" "-bc");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, 'c');
		found_exception = true;
	}
	QVERIFY(found_exception);
}
void unknownOption()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption('b');
	getopt.addOptionWithArg('c');

	bool found_exception = false;
	try {
		getopt.parse(QStringList() << "-a" << "-d" << "-c" << "2");
	} catch(QGetopt::UnknownOptionException & e) {
		QCOMPARE(e.option, 'd');
		found_exception = true;
	}
	QVERIFY(found_exception);
}
void longOptions()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption("add");
	getopt.addOption('c', "count");

	getopt.parse(QStringList() << "-a" << "--count");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption("add"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(getopt.hasOption("count"));

	getopt.parse(QStringList() << "-a" << "--count" << "--add");
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

	getopt.parse(QStringList() << "-a" << "--count" << "1");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(!getopt.hasOption("add"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(getopt.hasOption("count"));
	QCOMPARE(getopt.getArg('c'), QString("1"));
	QCOMPARE(getopt.getArg("count"), QString("1"));

	getopt.parse(QStringList() << "-a" << "--count" << "1" << "--add" << "2");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("add"));
	QVERIFY(getopt.hasOption("count"));
	QCOMPARE(getopt.getArg('c'), QString("1"));
	QCOMPARE(getopt.getArg("count"), QString("1"));
	QCOMPARE(getopt.getArg("add"), QString("2"));

	getopt.parse(QStringList() << "--count" << "1" << "--count" << "2");
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

	bool found_exception = false;
	try {
		getopt.parse(QStringList() << "--add" << "-b" << "-c" << "2");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, 0);
		QCOMPARE(e.longOption, "add");
		found_exception = true;
	}
	QVERIFY(found_exception);

	found_exception = true;
	try {
		getopt.parse(QStringList() << "--add" << "1" << "-bc");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, 'c');
		QCOMPARE(e.longOption, "count");
		found_exception = true;
	}
	QVERIFY(found_exception);

	found_exception = true;
	try {
		getopt.parse(QStringList() << "--add" << "1" << "-b" << "--count");
	} catch(QGetopt::NoArgException & e) {
		QCOMPARE(e.option, 'c');
		QCOMPARE(e.longOption, "count");
		found_exception = true;
	}
	QVERIFY(found_exception);
}
void unknownLongOption()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption("batch");
	getopt.addOptionWithArg('c', "count");

	bool found_exception = false;
	try {
		getopt.parse(QStringList() << "-a" << "--delete" << "--count" << "2");
	} catch(QGetopt::UnknownOptionException & e) {
		QCOMPARE(e.option, 0);
		QCOMPARE(e.longOption, "delete");
		found_exception = true;
	}
	QVERIFY(found_exception);
}
void nonOptionArgs()
{
	QGetopt getopt;
	getopt.addOption('a');
	getopt.addOption('3');
	getopt.addOption('4');
	getopt.addOption("batch");
	getopt.addOptionWithArg('c', "count");

	getopt.parse(QStringList() << "-a" << "--batch" << "--count" << "2" << "3" << "4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList() << QString("3") << QString("4"));

	getopt.parse(QStringList() << "-a" << "--batch" << "--count" << "2" << "3" << "-4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList() << QString("3") << QString("-4"));

	getopt.parse(QStringList() << "-a" << "--batch" << "--count" << "2" << "-3" << "-4");
	QVERIFY(getopt.hasOption('a'));
	QVERIFY(getopt.hasOption("batch"));
	QVERIFY(getopt.hasOption('c'));
	QVERIFY(getopt.hasOption('3'));
	QVERIFY(getopt.hasOption('4'));
	QCOMPARE(getopt.getArg('c'), QString("2"));
	QCOMPARE(getopt.getNonArgs(), QStringList());

	getopt.parse(QStringList() << "-a" << "--batch" << "--count" << "2" << "--" << "-3" << "-4");
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
