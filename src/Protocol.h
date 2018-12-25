#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <optional>
#include "Log.h"


class Packet {
    public:
        virtual std::string asPacket() const noexcept = 0;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WannaPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum class MessageType {
    WannaPlay,
    AcceptedPlay,
    RejectedPlay,
    PlayNoMore
};

class MessageWannaPlay : public Packet {
    private:
        const std::string m_InitiatorNick;
        const std::string m_InitiatorAddress;
        const unsigned int m_InitiatorPort;
        const std::string m_Word;

    public:
        MessageWannaPlay(
                const std::string& initiatorNick,
                const std::string& initiatorAddress,
                unsigned int initiatorPort,
                const std::string& word);
        std::string initiatorNick() const noexcept;
        std::string initiatorAddress() const noexcept;
        unsigned int initiatorPort() const noexcept;
        std::string initiatorWord() const noexcept;
        std::string asPacket() const noexcept override;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AcceptedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MessageAcceptedPlay : public Packet {
    private:
        const std::string m_AccepteeNick;

    public:
        MessageAcceptedPlay(const std::string& accepteeNick);
        std::string accepteeNick() const noexcept;
        std::string asPacket() const noexcept override;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// RejectedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MessageRejectedPlay : public Packet {
    private:
        const std::string m_RejecteeNick;

    public:
        MessageRejectedPlay(const std::string& rejecteeNick);
        std::string rejecteeNick() const noexcept;
        std::string asPacket() const noexcept override;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PlayNoMore
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MessagePlayNoMore : public Packet {
    public:
        MessagePlayNoMore();
        std::string asPacket() const noexcept override;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Message
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Message {
    public:
        static const std::string delimiter;

    private:
        const MessageType m_MessageType;
        const std::vector<std::string> m_Arguments;
        static MessageType retrieveMessageType(const std::string& source) noexcept;
        static std::vector<std::string> retrieveArguments(std::string source) noexcept;

    public:
        Message(const std::string& message);
        virtual ~Message();
        MessageType type() const noexcept;
        std::optional<MessageWannaPlay> asWannaPlay() const noexcept;
        std::optional<MessageAcceptedPlay> asAcceptedPlay() const noexcept;
        std::optional<MessageRejectedPlay> asRejectedPlay() const noexcept;
        std::optional<MessagePlayNoMore> asPlayNoMore() const noexcept;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Misc
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::string toLowerString(MessageType messageType) noexcept;
MessageType toMessageType(const std::string& messageType) noexcept;


#endif
