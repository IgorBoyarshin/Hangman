#include "Protocol.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WannaPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Protocol::MessageWannaPlay::MessageWannaPlay(
        const std::string& initiatorNick, const std::string& initiatorAddress,
        unsigned int initiatorPort, const std::string& word)
    : m_InitiatorNick(initiatorNick),
      m_InitiatorAddress(initiatorAddress),
      m_InitiatorPort(initiatorPort),
      m_Word(word) {}


std::string Protocol::MessageWannaPlay::initiatorNick() const noexcept {
    return m_InitiatorNick;
}

std::string Protocol::MessageWannaPlay::initiatorAddress() const noexcept {
    return m_InitiatorAddress;
}

unsigned int Protocol::MessageWannaPlay::initiatorPort() const noexcept {
    return m_InitiatorPort;
}

std::string Protocol::MessageWannaPlay::initiatorWord() const noexcept {
    return m_Word;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AcceptedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Protocol::MessageAcceptedPlay::MessageAcceptedPlay(
        const std::string& accepteeNick)
    : m_AccepteeNick(accepteeNick) {}


std::string Protocol::MessageAcceptedPlay::accepteeNick() const noexcept {
    return m_AccepteeNick;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// RejectedPlay
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Protocol::MessageRejectedPlay::MessageRejectedPlay(
        const std::string& rejecteeNick)
    : m_RejecteeNick(rejecteeNick) {}


std::string Protocol::MessageRejectedPlay::rejecteeNick() const noexcept {
    return m_RejecteeNick;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Message
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const std::string Protocol::Message::delimiter = ":";

Protocol::MessageType Protocol::Message::retrieveMessageType(const std::string& source) noexcept {
    const size_t pos = source.find(delimiter);
    const size_t position = (pos == std::string::npos) ? source.size() : pos;
    const std::string first = source.substr(0, position);
    if      (first == "wannaplay")    return MessageType::WannaPlay;
    else if (first == "acceptedplay") return MessageType::AcceptedPlay;
    else if (first == "rejectedplay") return MessageType::RejectedPlay;
    else if (first == "playnomore")   return MessageType::PlayNoMore;

    Log::error().setClass("Message").setFunc("retrieveMessageType")
        << "Couldn't retrieve MessageType from:" << first << std::endl;
    return MessageType::PlayNoMore;
}

std::vector<std::string> Protocol::Message::retrieveArguments(std::string source) noexcept {
    std::vector<std::string> arguments;
    size_t pos = 0;
    while ((pos = source.find(delimiter)) != std::string::npos) {
        // Skip first argument === message type
        if (arguments.size() > 0) arguments.push_back(source.substr(0, pos));
        source.erase(0, pos + delimiter.length());
    }

    return arguments;
}

Protocol::Message::Message(const std::string& message)
    : m_MessageType(retrieveMessageType(message)),
      m_Arguments(retrieveArguments(message)) {}


Protocol::Message::~Message() {}

Protocol::MessageType Protocol::Message::type() const noexcept {
    return m_MessageType;
}

std::optional<Protocol::MessageWannaPlay> Protocol::Message::asWannaPlay() const noexcept {
    if (m_MessageType == MessageType::WannaPlay) {
        return {MessageWannaPlay(
            m_Arguments[0], // nick
            m_Arguments[1], // address
            static_cast<unsigned int>(std::stoi(std::string(m_Arguments[2]))), // port
            m_Arguments[3] // word
        )};
    } else return std::nullopt;
}

std::optional<Protocol::MessageAcceptedPlay> Protocol::Message::asAcceptedPlay() const noexcept {
    if (m_MessageType == MessageType::AcceptedPlay) {
        return {MessageAcceptedPlay(
            m_Arguments[0] // nick
        )};
    } else return std::nullopt;
}

std::optional<Protocol::MessageRejectedPlay> Protocol::Message::asRejectedPlay() const noexcept {
    if (m_MessageType == MessageType::RejectedPlay) {
        return {MessageRejectedPlay(
            m_Arguments[0] // nick
        )};
    } else return std::nullopt;
}

std::optional<Protocol::MessagePlayNoMore> Protocol::Message::asPlayNoMore() const noexcept {
    if (m_MessageType == MessageType::PlayNoMore) {
        return {MessagePlayNoMore{}};
    } else return std::nullopt;
}
