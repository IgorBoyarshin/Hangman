#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <optional>
#include "Log.h"


class Protocol {
    public:
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WannaPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        enum class MessageType {
            WannaPlay,
            AcceptedPlay,
            RejectedPlay,
            PlayNoMore
        };

        class MessageWannaPlay {
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
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AcceptedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class MessageAcceptedPlay {
            private:
                const std::string m_AccepteeNick;

            public:
                MessageAcceptedPlay(const std::string& accepteeNick);
                std::string accepteeNick() const noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// RejectedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class MessageRejectedPlay {
            private:
                const std::string m_RejecteeNick;

            public:
                MessageRejectedPlay(const std::string& rejecteeNick);
                std::string rejecteeNick() const noexcept;
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PlayNoMore
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        class MessagePlayNoMore {
            public:
                MessagePlayNoMore();
        };
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Message
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    class Message {
        private:
            const MessageType m_MessageType;
            const std::vector<std::string> m_Arguments;
            static const std::string delimiter;

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

};


#endif
