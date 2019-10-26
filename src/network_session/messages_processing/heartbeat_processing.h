/**
 *  @file       heartbeat_processing.h
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  Apache License Version 2.0
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

#ifndef HEARTBEAT_PROCESSING_H
#define HEARTBEAT_PROCESSING_H

#include <network_session/message_definitions.h>
#include <network_session/session_handler.h>
#include <network_session/internal_session_interface.h>

#include <libKitsuneNetwork/abstract_socket.h>
#include <libKitsuneNetwork/message_ring_buffer.h>

#include <libKitsuneProjectCommon/network_session/session_controller.h>
#include <libKitsuneProjectCommon/network_session/session.h>

#include <libKitsunePersistence/logger/logger.h>

using Kitsune::Network::MessageRingBuffer;
using Kitsune::Network::AbstractSocket;
using Kitsune::Network::getObjectFromBuffer;

namespace Kitsune
{
namespace Project
{
namespace Common
{

/**
 * @brief send_Heartbeat_Start
 * @param id
 * @param socket
 */
inline void
send_Heartbeat_Start(Session* session)
{
    if(DEBUG_MODE) {
        LOG_DEBUG("SEND heartbeat start");
    }

    Heartbeat_Start_Message message(session->sessionId(),
                                    session->increaseMessageIdCounter());

    SessionHandler::m_sessionInterface->sendMessage(session,
                                                    message.commonHeader,
                                                    &message,
                                                    sizeof(message));
}

/**
 * @brief send_Heartbeat_Reply
 * @param id
 * @param socket
 */
inline void
send_Heartbeat_Reply(Session* session,
                     const uint32_t messageId)
{
    if(DEBUG_MODE) {
        LOG_DEBUG("SEND heartbeat reply");
    }

    Heartbeat_Reply_Message message(session->sessionId(),
                                    messageId);

    SessionHandler::m_sessionInterface->sendMessage(session,
                                                    message.commonHeader,
                                                    &message,
                                                    sizeof(message));
}

/**
 * @brief process_Heartbeat_Start
 */
inline void
process_Heartbeat_Start(Session* session,
                        const Heartbeat_Start_Message* message)
{
    if(DEBUG_MODE) {
        LOG_DEBUG("process heartbeat start");
    }

    send_Heartbeat_Reply(session,
                         message->commonHeader.messageId);
}

/**
 * @brief process_Heartbeat_Reply
 */
inline void
process_Heartbeat_Reply(Session*,
                        const Heartbeat_Reply_Message*)
{
    if(DEBUG_MODE) {
        LOG_DEBUG("process heartbeat reply");
    }
}

/**
 * @brief process_Heartbeat_Type
 * @param header
 * @param recvBuffer
 * @return
 */
inline uint64_t
process_Heartbeat_Type(Session* session,
                       const CommonMessageHeader* header,
                       MessageRingBuffer* recvBuffer)
{
    if(DEBUG_MODE) {
        LOG_DEBUG("process heartbeat-type");
    }

    switch(header->subType)
    {
        case HEARTBEAT_START_SUBTYPE:
            {
                const Heartbeat_Start_Message* message =
                        getObjectFromBuffer<Heartbeat_Start_Message>(recvBuffer);
                if(message == nullptr) {
                    break;
                }
                process_Heartbeat_Start(session, message);
                return sizeof(*message);
            }
        case HEARTBEAT_REPLY_SUBTYPE:
            {
                const Heartbeat_Reply_Message* message =
                        getObjectFromBuffer<Heartbeat_Reply_Message>(recvBuffer);
                if(message == nullptr) {
                    break;
                }
                process_Heartbeat_Reply(session, message);
                return sizeof(*message);
            }
        default:
            break;
    }

    return 0;
}

} // namespace Common
} // namespace Project
} // namespace Kitsune

#endif // HEARTBEAT_PROCESSING_H
