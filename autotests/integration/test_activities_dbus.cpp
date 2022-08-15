#include <QTest>

#include "testhelper.h"
#include <QObject>

class Activities : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testActivitySwitch();
    void testActivityRemoved();
    void testActivitiesListIsAccurate();
    void testTryRemoveLastActivity();
    void testSetActivity();

    std::unique_ptr<ManagerDaemonTestInstance> instance;
};

QTEST_GUILESS_MAIN(Activities);

void Activities::initTestCase()
{
    // Called before the first testfunction is executed
    instance = std::make_unique<ManagerDaemonTestInstance>();
}

void Activities::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void Activities::init()
{
    // Called before each testfunction is executed
}

void Activities::cleanup()
{
    // Called after every testfunction
    instance.reset();
}

void Activities::testActivitySwitch()
{
}

void Activities::testActivityRemoved()
{
}

void Activities::testActivitiesListIsAccurate()
{
}

void Activities::testTryRemoveLastActivity()
{
}

void Activities::testSetActivity()
{
}

#include "test_activities_dbus.moc"
