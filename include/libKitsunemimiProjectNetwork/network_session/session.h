/**
 * @file       session.h
 *
 * @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright  Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <assert.h>
#include <atomic>

#include <libKitsunemimiCommon/statemachine.h>

namespace Kitsunemimi
{
namespace Network {
class AbstractSocket;
}
namespace Project
{
class SessionHandler;
class SessionController;
class InternalSessionInterface;
class MultiblockIO;

class Session
{
public:
    ~Session(); 

    bool sendStreamData(const void* data,
                        const uint64_t size,
                        const bool dynamic = false,
                        const bool replyExpected = false);

    uint64_t sendMultiblockData(const void* data,
                                const uint64_t size);
    void abortMessages(const uint64_t multiblockMessageId=0);

    bool closeSession(const bool replyExpected = false);
    uint32_t sessionId() const;
    bool isClientSide() const;

    enum errorCodes
    {
        UNDEFINED_ERROR = 0,
        FALSE_VERSION = 1,
        UNKNOWN_SESSION = 2,
        INVALID_MESSAGE_SIZE = 3,
        MESSAGE_TIMEOUT = 4,
        MULTIBLOCK_FAILED = 5,
    };

    uint32_t increaseMessageIdCounter();




    //=====================================================================
    // ALL BELOW IS INTERNAL AND SHOULD NEVER BE USED BY EXTERNAL METHODS!
    //=====================================================================
    Session(Network::AbstractSocket* socket);

    Kitsunemimi::Statemachine m_statemachine;
    Network::AbstractSocket* m_socket = nullptr;
    MultiblockIO* m_multiblockIo = nullptr;
    uint32_t m_sessionId = 0;
    uint64_t m_sessionIdentifier = 0;

    // init session
    bool connectiSession(const uint32_t sessionId,
                         const uint64_t sessionIdentifier,
                         const bool init = false);
    bool makeSessionReady(const uint32_t sessionId,
                          const uint64_t sessionIdentifier);

    // end session
    bool endSession(const bool init = false);
    bool disconnectSession();

    bool sendHeartbeat();
    void initStatemachine();

    // callbacks
    void* m_sessionTarget = nullptr;
    void (*m_processSession)(void*, bool, Session*, const uint64_t);
    void* m_dataTarget = nullptr;
    void (*m_processData)(void*, Session*, const bool, const void*, const uint64_t);
    void* m_errorTarget = nullptr;
    void (*m_processError)(void*, Session*, const uint8_t, const std::string);

    // counter
    std::atomic_flag m_messageIdCounter_lock = ATOMIC_FLAG_INIT;
    uint32_t m_messageIdCounter = 0;
};

} // namespace Project
} // namespace Kitsunemimi

#endif // SESSION_H
