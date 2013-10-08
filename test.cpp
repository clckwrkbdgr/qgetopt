#include <QtTest>

class TestQGetopt : public QObject {
	Q_OBJECT
private slots:
void shortArguments() {}
void optargs() {}
void optWithoutOptarg() {}
void unknownOption() {}
void longOptions() {}
void longOptionsWithArg() {}
void longOptionsWithoutArg() {}
void sameLongAndShortOption() {}
void unknownLongOption() {}
void nonOptionArgs() {}
};

QTEST_MAIN(TestQGetopt);
#include "test.moc"
