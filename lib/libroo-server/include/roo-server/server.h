#ifndef ROO_SERVER_SERVER_H
#define ROO_SERVER_SERVER_H

#include <poll.h>
#include <string>
#include <vector>

#include "dispatch.h"
#include "message.h"

namespace Roo
{
  /*!
   * @brief Forward-declaration of the core runtime
   */
  class Runtime;
} // namespace Roo

namespace Roo::Server
{
  /*!
   * @namespace Roo::Server
   * @brief Namespace containing the Roo Embeddable REPL Server and all of
   * its required collaborators.
   */
  /*!
   * @brief Default server name that a started server will identify itself
   * as.
   *
   * This should typically be specified by any application that embeds a
   * Roo Server, and this will only be used if the application fails to do
   * so.
   *
   * It has no functional purpose, other than letting the connect REPL know
   * what it is talking to.
   */
  inline constexpr const char __NAME[] = "Roo Embeddable REPL Server";

  /*!
   * @brief Default version number that a started server will identify itself
   * as.
   *
   * This can be overriden by any application that embeds a Roo Server.
   */
  inline constexpr const char __VERSION[] = "0.1.0";

  /*!
   * @brief Type-alias used to signify a Server Status Code.
   *
   * Positive values indicate success modes, and negative values indicate
   * server failure.
   */
  typedef int ServerStatusCode;

  /*!
   * @brief Struct that encapsulates the ServerStatus, which includes a status
   * code and a human-readable message.
   */
  struct ServerStatus
  {
    /*!
     * @brief Status code that indicates success or failure of operations.
     * Positive status code values indicate success modes, and negative values
     * indiciate server failure.
     */
    ServerStatusCode code;
    /*!
     * @brief An arbitrary human-readable message that describes the result of
     * an operation.
     */
    std::string message;
  };

  /*!
   * @brief Struct for providing configuration to a Roo Server, most
   * importantly which port to bind to.
   */
  struct ServerConfig
  {
    /*!
     * @brief The name that the server itself will identify itself as.
     * Defaults to @ref __NAME.
     *
     * Should typically not be overridden.
     */
    std::string server_name = std::string(__NAME);
    /*!
     * @brief The version that the server itself will identify itself as.
     * Defaults to @ref __VERSION.
     *
     * Should typically not be overridden.
     */
    std::string server_version = __VERSION;
    /*!
     * @brief The name of the application that the server is embedded within.
     *
     * Shoule be set by the application, to allow the REPL to know who it is
     * talking to.
     */
    std::string application_name = "";
    /*!
     * @brief The version of the application that the server is embedded
     * within.
     *
     * Shoule be set by the application, to allow the REPL to know who it is
     * talking to.
     */
    std::string application_version = "";
    /*!
     * @brief The port that the Server will bind to.
     */
    unsigned int port = 8100;
  };

  /*!
   * @brief The Roo Embeddable REPL Server itself.
   *
   * After instantiation @ref Server::start() must be called to start the
   * server and allow it to bind to the specified port.
   *
   * To handle incoming messages, the application must repeatedly call
   * @ref Server::query_sockets. When running in a single-threaded application
   * this would be typically be once per iteration of the main loop of
   * the application.
   *
   * Applications that want to run the Server in a separate thread thread is
   * responsible for creating and managing the thread itself, using whatever
   * threading model available to it. The thread should then call @ref
   * Server::query_sockets repeatedly.
   */
  class Server
  {
   private:
    /*!
     * @brief The configuration that should be used to start the Server.
     * @see Roo::Server::ServerConfig.
     */
    ServerConfig config;
    /*!
     * @brief A collection of active socket file descriptors, containing
     * all active connections. The server socket will always be stored at
     * position 0.
     */
    std::vector<pollfd> socket_descriptors;
    /*!
     * @brief Responsible for parsing incoming messages. @see MessageParser.
     */
    MessageParser message_parser;
    /*!
     * @brief Dispatches and handles incoming messages. @see Dispatcher
     */
    Dispatcher dispatcher;
    /*!
     * @brief Accepts, reads and handles an incoming message on the specified
     * socket.
     */
    void accept_request(int socket);

   public:
    /*!
     * @brief Creates a new Server instance with the provided configuration
     * and runtime.
     */
    Server(const ServerConfig& config, Roo::Runtime& runtime);
    /*!
     * @brief Start the server, ie bind to the specified port and start
     * listening for messages.
     *
     * If binding to the port fails for whatever reason, the returned
     * ServerStatus will contain a negative ServerStatusCode.
     *
     * Note that the Server uses a single-threade model and does not block.
     * For messages to be read and dispatched, the application must repeatedly
     * call @ref Server::query_sockets()
     */
    ServerStatus start();
    /*!
     * @brief Shutdown the server and close any open sockets.
     */
    ServerStatus shutdown();

    /*!
     * @brief Poll all active sockets for incoming traffic, and handle any
     * successfully parsed messages. Also reads the server socket and sets up
     * any new incoming connections.
     */
    void query_sockets();
  };
} // namespace Roo::Server

#endif
