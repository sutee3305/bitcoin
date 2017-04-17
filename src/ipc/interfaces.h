#ifndef BITCOIN_IPC_INTERFACES_H
#define BITCOIN_IPC_INTERFACES_H

#include "netaddress.h" // For Network

#include <memory>
#include <string>

class proxyType;

namespace ipc {

class Handler;

//! Top-level interface for a bitcoin node (bitcoind process).
class Node
{
public:
    virtual ~Node() {}

    //! Set command line arguments.
    virtual void parseParameters(int argc, const char* const argv[]) = 0;

    //! Set a command line argument if it doesn't already have a value
    virtual bool softSetArg(const std::string& arg, const std::string& value) = 0;

    //! Set a command line boolean argument if it doesn't already have a value
    virtual bool softSetBoolArg(const std::string& arg, bool value) = 0;

    //! Load settings from configuration file.
    virtual void readConfigFile(const std::string& confPath) = 0;

    //! Choose network parameters.
    virtual void selectParams(const std::string& network) = 0;

    //! Init logging.
    virtual void initLogging() = 0;

    //! Init parameter interaction.
    virtual void initParameterInteraction() = 0;

    //! Get warnings.
    virtual std::string getWarnings(const std::string& type) = 0;

    //! Start node.
    virtual bool appInit() = 0;

    //! Stop node.
    virtual void appShutdown() = 0;

    //! Start shutdown.
    virtual void startShutdown() = 0;

    //! Return whether shutdown was requested.
    virtual bool shutdownRequested() = 0;

    //! Map port.
    virtual void mapPort(bool useUPnP) = 0;

    //! Get proxy.
    virtual bool getProxy(Network net, proxyType& proxyInfo) = 0;

    //! Register handler for init messages.
    using InitMessageFn = std::function<void(const std::string& message)>;
    virtual std::unique_ptr<Handler> handleInitMessage(InitMessageFn fn) = 0;

    //! Register handler for message box messages.
    using MessageBoxFn =
        std::function<bool(const std::string& message, const std::string& caption, unsigned int style)>;
    virtual std::unique_ptr<Handler> handleMessageBox(MessageBoxFn fn) = 0;

    //! Register handler for question messages.
    using QuestionFn = std::function<bool(const std::string& message,
        const std::string& nonInteractiveMessage,
        const std::string& caption,
        unsigned int style)>;
    virtual std::unique_ptr<Handler> handleQuestion(QuestionFn fn) = 0;
};

//! Interface for managing a registered handler.
class Handler
{
public:
    virtual ~Handler() {}
    //! Disconnect the handler.
    virtual void disconnect() = 0;
};

//! Protocol IPC interface should use to communicate with implementation.
enum Protocol {
    LOCAL, //!< Call functions linked into current executable.
};

//! Create IPC node interface, communicating with requested protocol. Returns
//! null if protocol isn't implemented or is not available in the current build
//! configuation.
std::unique_ptr<Node> MakeNode(Protocol protocol);

} // namespace ipc

#endif // BITCOIN_IPC_INTERFACES_H