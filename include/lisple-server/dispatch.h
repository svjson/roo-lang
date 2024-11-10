
#ifndef __LISPLE_SERVER__DISPATCH_H_
#define __LISPLE_SERVER__DISPATCH_H_

#include <variant>

#include "message.h"

namespace Lisple
{
  class LispReader;

  namespace Server
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
       * @brief The Lisple runtime that the Server is exposing for remote
       * access.
       */
      LispReader& runtime;

    public:
      Dispatcher(const ServerConfig& config, LispReader& runtime);

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
  }
}


#endif
