
#include "roo-server/dispatch.h"

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <roo/form.h>
#include <roo/namespace.h>
#include <roo/runtime.h>

#include "roo-server/server.h"

namespace Roo::Server
{
  using Roo::Runtime;

  const std::string _NS = "NS";

  const std::string _SERVER_NAME = "SERVER_NAME";
  const std::string _SERVER_VERSION = "SERVER_VERSION";
  const std::string _APP_NAME = "APPLICATION_NAME";
  const std::string _APP_VERSION = "APPLICATION_VERSION";

  Dispatcher::Dispatcher(const ServerConfig& config, Runtime& runtime)
    : config(config)
    , runtime(runtime)
  {
  }

  std::variant<Response, Error> Dispatcher::dispatch(const Message& message)
  {
    if (message.has_block(_CMD))
    {
      const Command& command = message.get_block(_CMD);

      if (!command.has_property(_ID))
      {
        return Error{0x01, "CMD requires @ID"};
      }

      return execute_command(command);
    }

    return Error{0x01, "Unrecognized message"};
  }

  std::variant<Response, Error> Dispatcher::execute_command(const Command& cmd)
  {
    if (cmd.get_property(_ID) == "eval-sexp")
    {
      if (!cmd.has_property(_BODY))
      {
        return Error{0x01, "CMD 'eval-sexp' requires @BODY"};
      }

      if (!cmd.has_property(_NS))
      {
        return Error{0x01, "CMD 'eval-sexp' requires @ID"};
      }

      const std::string& ns = cmd.get_property(_NS);
      const std::string& prev_ns = runtime.get_current_namespace().get_name();

      try
      {
        runtime.ns(ns, true);
        const std::string result = runtime.eval(cmd.get_property(_BODY))->to_string();
        Response response;

        if (runtime.get_current_namespace().get_name() != ns)
        {
          response = MessageBuilder::build_response_with_props(
            "switch-ns",
            result,
            {{_NS, runtime.get_current_namespace().get_name()}});
        }
        else
        {
          response = MessageBuilder::build_simple_response("eval-result", result);
        }

        runtime.ns(prev_ns);
        return response;
      }
      catch (std::exception& e)
      {
        runtime.ns(prev_ns);
        return Error{0x10, e.what()};
      }
    }
    else if (cmd.get_property(_ID) == "server-info")
    {
      std::string body = config.application_name.size()
                           ? config.application_name + " " + config.application_version
                           : config.server_name + " " + config.server_name;

      return MessageBuilder::build_response_with_props(
        "server-info",
        body,
        {{_SERVER_NAME, config.server_name},
         {_SERVER_VERSION, config.server_version},
         {_APP_NAME, config.application_name},
         {_APP_VERSION, config.application_version}});
    }

    return Error{0x01, "Unrecognized CMD with @ID=" + cmd.get_property(_ID)};
  }
} // namespace Roo::Server
