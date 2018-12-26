#include "Protocol.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WannaPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageWannaPlay::MessageWannaPlay(
        const std::string& initiatorNick, const std::string& initiatorAddress,
        unsigned int initiatorPort, const std::string& word)
    : m_InitiatorNick(initiatorNick),
      m_InitiatorAddress(initiatorAddress),
      m_InitiatorPort(initiatorPort),
      m_Word(word) {}


std::string MessageWannaPlay::initiatorNick() const noexcept {
    return m_InitiatorNick;
}

std::string MessageWannaPlay::initiatorAddress() const noexcept {
    return m_InitiatorAddress;
}

unsigned int MessageWannaPlay::initiatorPort() const noexcept {
    return m_InitiatorPort;
}

std::string MessageWannaPlay::initiatorWord() const noexcept {
    return m_Word;
}

std::string MessageWannaPlay::asPacket() const noexcept {
    return toLowerString(MessageType::WannaPlay) + Message::delimiter
        + m_InitiatorNick + Message::delimiter
        + m_InitiatorAddress + Message::delimiter
        + std::to_string(m_InitiatorPort) + Message::delimiter
        + m_Word;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AcceptedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageAcceptedPlay::MessageAcceptedPlay(const std::string& accepteeNick)
    : m_AccepteeNick(accepteeNick) {}


std::string MessageAcceptedPlay::accepteeNick() const noexcept {
    return m_AccepteeNick;
}

std::string MessageAcceptedPlay::asPacket() const noexcept {
    return toLowerString(MessageType::AcceptedPlay) + Message::delimiter
        + m_AccepteeNick;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// RejectedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageRejectedPlay::MessageRejectedPlay(const std::string& rejecteeNick)
    : m_RejecteeNick(rejecteeNick) {}


std::string MessageRejectedPlay::rejecteeNick() const noexcept {
    return m_RejecteeNick;
}

std::string MessageRejectedPlay::asPacket() const noexcept {
    return toLowerString(MessageType::RejectedPlay) + Message::delimiter
        + m_RejecteeNick;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TryLetter
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageTryLetter::MessageTryLetter(char letter)
    : m_Letter(letter) {}


char MessageTryLetter::letter() const noexcept {
    return m_Letter;
}

std::string MessageTryLetter::asPacket() const noexcept {
    return toLowerString(MessageType::TryLetter) + Message::delimiter
        + m_Letter;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PlayNoMore
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessagePlayNoMore::MessagePlayNoMore() {}


std::string MessagePlayNoMore::asPacket() const noexcept {
    return toLowerString(MessageType::PlayNoMore);
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Message
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const std::string Message::delimiter = ":";

MessageType Message::retrieveMessageType(const std::string& source) noexcept {
    const size_t pos = source.find(delimiter);
    const size_t position = (pos == std::string::npos) ? source.size() : pos;
    const std::string first = source.substr(0, position);
    return toMessageType(first);
}

std::vector<std::string> Message::retrieveArguments(std::string source) noexcept {
    std::vector<std::string> arguments;
    size_t pos = 0;
    bool isFirst = true;
    while ((pos = source.find(delimiter)) != std::string::npos) {
        // Skip first argument === message type
        if (isFirst) {
            isFirst = false;
        } else {
            arguments.push_back(source.substr(0, pos));
        }
        source.erase(0, pos + delimiter.length());
    }
    arguments.push_back(source.substr(0, source.size()));

    return arguments;
}

Message::Message(const std::string& message)
    : m_MessageType(retrieveMessageType(message)),
      m_Arguments(retrieveArguments(message)) {}


Message::~Message() {}

MessageType Message::type() const noexcept {
    return m_MessageType;
}

std::optional<MessageWannaPlay> Message::asWannaPlay() const noexcept {
    if (m_MessageType == MessageType::WannaPlay) {
        return {MessageWannaPlay(
            m_Arguments[0], // nick
            m_Arguments[1], // address
            static_cast<unsigned int>(std::stoi(std::string(m_Arguments[2]))), // port
            m_Arguments[3] // word
        )};
    } else return std::nullopt;
}

std::optional<MessageAcceptedPlay> Message::asAcceptedPlay() const noexcept {
    if (m_MessageType == MessageType::AcceptedPlay) {
        return {MessageAcceptedPlay(
            m_Arguments[0] // nick
        )};
    } else return std::nullopt;
}

std::optional<MessageRejectedPlay> Message::asRejectedPlay() const noexcept {
    if (m_MessageType == MessageType::RejectedPlay) {
        return {MessageRejectedPlay(
            m_Arguments[0] // nick
        )};
    } else return std::nullopt;
}

std::optional<MessageTryLetter> Message::asTryLetter() const noexcept {
    if (m_MessageType == MessageType::TryLetter) {
        return {MessageTryLetter(
            m_Arguments[0][0] // letter
        )};
    } else return std::nullopt;
}

std::optional<MessagePlayNoMore> Message::asPlayNoMore() const noexcept {
    if (m_MessageType == MessageType::PlayNoMore) {
        return {MessagePlayNoMore{}};
    } else return std::nullopt;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Misc
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
std::string toLowerString(MessageType messageType) noexcept {
    switch (messageType) {
        case MessageType::WannaPlay:
            return "wannaplay";
        case MessageType::AcceptedPlay:
            return "acceptedplay";
        case MessageType::RejectedPlay:
            return "rejectedplay";
        case MessageType::TryLetter:
            return "tryletter";
        case MessageType::PlayNoMore:
            return "playnomore";
        default:
            Log::error().setClass("MessageType").setFunc("toString")
                << "Unknown MessageType received." << std::endl;
            return "";
    }
}

MessageType toMessageType(const std::string& messageType) noexcept {
    if      (messageType == "wannaplay")    return MessageType::WannaPlay;
    else if (messageType == "acceptedplay") return MessageType::AcceptedPlay;
    else if (messageType == "rejectedplay") return MessageType::RejectedPlay;
    else if (messageType == "playnomore")   return MessageType::PlayNoMore;
    else if (messageType == "tryletter")    return MessageType::TryLetter;

    Log::error().setClass("MessageType").setFunc("toMessageType")
        << "Couldn't retrieve MessageType from:" << messageType << std::endl;
    return MessageType::PlayNoMore;
}
