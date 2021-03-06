#ifndef TEST_SESSION_H
#define TEST_SESSION_H

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include <libKitsunemimiCommon/test_helper/speed_test_helper.h>

namespace Kitsunemimi
{
struct StackBuffer;

namespace Sakura
{
class SessionController;
class Session;

typedef std::chrono::microseconds chronoMicroSec;
typedef std::chrono::nanoseconds chronoNanoSec;
typedef std::chrono::seconds chronoSec;
typedef std::chrono::high_resolution_clock::time_point chronoTimePoint;
typedef std::chrono::high_resolution_clock chronoClock;

class TestSession
        : public Kitsunemimi::SpeedTestHelper
{
public:
    TestSession(const std::string &address,
                const uint16_t port,
                const std::string &socket,
                const std::string &transferType);
    void runTest(const long packageSize);
    double calculateSpeed(double duration);

    static TestSession* m_instance;

    bool m_isClient = false;
    bool m_isTcp = false;
    std::string m_transferType = "";

    uint64_t m_size = 0;
    uint64_t m_totalSize = 0;
    uint64_t m_sizeCounter = 0;
    uint8_t* m_dataBuffer = nullptr;
    Kitsunemimi::StackBuffer* m_stackBuffer = nullptr;

    Kitsunemimi::Sakura::SessionController* m_controller = nullptr;
    Kitsunemimi::Sakura::Session* m_clientSession = nullptr;
    Kitsunemimi::Sakura::Session* m_serverSession = nullptr;

    TimerSlot m_timeSlot;
    std::mutex m_cvMutex;
    std::condition_variable m_cv;
};

}
}

#endif // TEST_SESSION_H
