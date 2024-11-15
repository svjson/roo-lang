
#ifndef __LISPLE_SERVER__MESSAGE_H_
#define __LISPLE_SERVER__MESSAGE_H_

#include <cstdint>
#include <map>
#include <optional>
#include <stack>
#include <stddef.h>
#include <string>
#include <variant>

namespace Lisple
{
  namespace Server
  {
    /*! @brief Constant for Message/MSG block name */
    extern const std::string _MSG;
    /*! @brief Constant for Command/CMD block name */
    extern const std::string _CMD;
    /*! @brief Constant for Response/RESP block name */
    extern const std::string _RESP;
    /*! @brief Constant for id/ID poperty name */
    extern const std::string _ID;
    /*! @brief Constant for code/CODE poperty name */
    extern const std::string _CODE;
    /*! @brief Constant for body/BODY poperty name */
    extern const std::string _BODY;

    /*!
     * @brief Describes a parsing error
     */
    struct Error
    {
      uint8_t code;
      std::string msg;
    };

    /*!
     * @brief A named block of a message, that may contain properties or other
     * blocks.
     *
     * Forms the basic building block a message, which should always consist of
     * a wrapping Message/MSG block.
     */
    class Block
    {
      /*!
       * @brief The named parts of the block, either of type Block or std::string
       */
      std::map<const std::string, std::variant<Block, const std::string>> properties;

     public:
      /*!
       * @brief Tests if this Block contains any child blocks or properties.
       */
      bool empty() const;
      /*!
       * @brief Tests if this Block contains a child block or property of a
       * specific name.
       */
      bool has_property(const std::string& name) const;
      /*!
       * @brief Tests if this Block contains a child block of a specific name.
       */
      bool has_block(const std::string& name) const;
      /*!
       * @brief Creates an empty child Block on this instance, and returns a
       * pointer to the new child.
       */
      Block* add_block(const std::string& name);
      /*!
       * @brief Adds a string property to this block instance.
       */
      void set_property(const std::string& name, const std::string& value);
      /*!
       * @brief Get the value of a string property on this block instance.
       */
      const std::string& get_property(const std::string& name) const;
      /*!
       * @brief Get a reference to a named child block.
       */
      const Block& get_block(const std::string& name) const;
      /*!
       * @brief Encode this block instance for wire transfer, wrapped in a
       * Message/MSG section.
       */
      std::string encode() const;
      /*!
       * @brief Encode this block instance for wire transfer as is, without
       * wrapping it in any containing block.
       */
      std::string encode_bl() const;
    };

    /*! @brief Type-alias for Message, which is a Block */
    typedef Block Message;
    /*! @brief Type-alias for Response, which is a Block */
    typedef Block Response;
    /*! @brief Type-alias for Command, which is a Block */
    typedef Block Command;

    /*!
     * @brief Builder contraption for iterative construction of Message
     * instances.
     *
     * Allows the @ref MessageParser to build up a message by adding
     * blocks and properties as they are encountered.
     *
     * @ref MessageBuilder::begin_block opens a new Block at the current
     * level and pushes it to the context stack, making subsequent calls to
     * @ref MessageBuilder::begin_block or @ref MessageBuilder::set_property
     * target this new block.
     *
     * @ref MessageBuilder::end_block() pops the current block from the
     * context stack and redirects any further operations to the new parent.
     */
    class MessageBuilder
    {
      /*!
       * @brief A frame on the context stack, which contains the name and
       * block content of a block.
       */
      struct StackFrame
      {
        /*! @brief The name of the target block */
        const std::string name;
        /*! @brief The target block */
        Block* block;
      };

      /*!
       * @brief The resulting message.
       */
      Message message;
      /*!
       * @brief Context stack that keeps tracking of the current nesting level
       * and target block for @ref begin_block, @ref set_property and @ref end_block
       */
      std::stack<StackFrame> ctx_stack;

     public:
      /*!
       * @brief Begins a new child block on the current target and pushes it to the
       * content stack, effectively making it the new target for further operations.
       *
       * @return An @ref Error if the operation fails, otherwise std::nullopt.
       */
      std::optional<Error> begin_block(const std::string& name);
      /*!
       * @brief Sets a property on the current target block.
       *
       * @return An @ref Error if the operation fails, otherwise std::nullopt.
       */
      std::optional<Error> set_property(const std::string& name, const std::string& value);

      /*!
       * @brief Closes the current target and pops it from the current stack,
       * again making its parent the new target. The name of the block to close
       * must match the name of the current target, or an error will be raised
       * as the Message structure is inconsistent.
       *
       * @return An @ref Error if the operation fails, otherwise std::nullopt.
       */
      std::optional<Error> end_block(const std::string& name);

      /*!
       * @brief Finalizes message construction and returns the constructed
       * Message instance.
       *
       * This operation will fail if all opened blocks have not properly been
       * closed or if the message block is empty.
       *
       * @return A @ref Message instance if successful, otherwise an @ref Error.
       */
      std::variant<Message, Error> finalize_message();

      /*!
       * @brief Convenience factory method that constructs a simple Message
       * consisting only of a Response/RESP block with an ID property and a BODY
       * property.
       */
      static Response build_simple_response(const std::string& id, const std::string& body);
      /*!
       * @brief Convenience factory method that constructs a simple Message
       * consisting only of a Response/RESP block with an ID property, a BODY
       * property and any number of additional properties.
       */
      static Response build_response_with_props(const std::string& id,
                                                const std::string& body,
                                                const std::map<std::string, std::string>& props);
      /*!
       * @brief Convenience factory method that constructs a simple Message
       * containing an Error response.
       */
      static Response build_error_response(const Error& error);
    };

    /*!
     * @brief Parses the wire-transfer format into a @ref Message instance.
     */
    class MessageParser
    {
    public:
      /*!
       * @brief Attempts to parse a structured @ref Message from a raw string
       * of input data.
       *
       * @return A @ref Message instance if successful, otherwise @ref Error.
       */
      std::variant<Message, Error> parse_message(const std::string& data);

    private:
      /*!
       * @brief Attempts to read a symbol/property/block name at the current
       * position @a i of the input data.
       *
       * @return A string containing the symbol name if successful, otherwise
       * @ref Error.
       */
      std::variant<std::string, Error> read_symbol(const std::string& data, size_t* i);
      /*!
       * @brief Attempts to read a property value at the current position @a i
       * of the input data
       *
       * @return A string containing the property value if successful, otherwise
       * @ref Error.
       */
      std::variant<std::string, Error> read_value(const std::string& data, size_t* i);
      /*!
       * @brief Forward the read position @a i until the char @a c is encountered.
       */
      std::optional<Error> skip_until(char c, const std::string& data, size_t* i);
      /*!
       * @brief Attempts to read the opening of a Block from the input data at
       * position @a i, and notifies the MessageBuilder @a builder.
       *
       * @return An @ref Error if the operation fails, otherwise std::nullopt,
       * indicating success.
       */
      std::optional<Error> read_begin_block(const std::string& data, size_t* i, MessageBuilder* builder);
      /*!
       * @brief Attempts to read the closing of a Block from the input data at
       * position @a i, and notifies the MessageBuilder @a builder.
       *
       * @return An @ref Error if the operation fails, otherwise std::nullopt,
       * indicating success.
       */
      std::optional<Error> read_end_block(const std::string& data, size_t* i, MessageBuilder* builder);
      /*!
       * @brief Attempts to read a property(name and value) from the input data
       * at position @a i, and notifies the MessageBuilder @a builder.
       *
       * @return An @ref Error if the operation fails, otherwise std::nullopt,
       * indicating success.
       */
      std::optional<Error> read_property(const std::string& data, size_t* i, MessageBuilder* builder);
    };
  }
}


#endif
