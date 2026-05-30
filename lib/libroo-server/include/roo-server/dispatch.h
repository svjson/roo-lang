
#ifndef ROO_SERVER_DISPATCH_H
#define ROO_SERVER_DISPATCH_H

#include <variant>

#include "message.h"

namespace Roo
{
  class Runtime;
}

namespace Roo::Server
{
  struct ServerConfig;

  /*!
   * @brief Responsible for dispatching and executing incoming command
   * messages.
   */
  class Dispatcher
  {
    /*!
     * @brief The configuration of the owning Server instance. Used to
     * communicate server/application name and version.
     */
    const ServerConfig& config;
    /*!
     * @brief The runtime that the Server is exposing for remote
     * access.
     */
    Roo::Runtime& runtime;

   public:
    Dispatcher(const ServerConfig& config, Roo::Runtime& runtime);

    /*!
     * @brief Analyze and dispatch an incoming message to the appropriate
     * handler.
     */
    std::variant<Response, Error> dispatch(const Message& message);

   protected:
    /*!
     * @brief Called to execute Command sections of incoming messages.
     */
    std::variant<Response, Error> execute_command(const Command& command);
  };
} // namespace Roo::Server

#endif
