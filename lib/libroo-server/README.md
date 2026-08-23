# roo-server

`roo-server` is a small server layer for Roo message parsing, dispatch,
and server experiments.

It is a sibling library to `libroo`, not part of the core runtime. The code
is currently primitive and should be treated as an integration experiment rather
than the model for new Roo application architecture.

## Standalone evaluation server

The `roo-server` executable starts a persistent vanilla Roo runtime rooted at
its working directory:

```sh
roo-server --port 0
```

It binds to `127.0.0.1` by default. Port `0` selects an available ephemeral
port. Once ready, it prints a machine-readable line containing the actual
address and port:

```text
ROO_SERVER_READY host=127.0.0.1 port=43217
```

Remote access must be enabled explicitly by selecting another local interface
or all interfaces:

```sh
roo-server --host 0.0.0.0 --port 8100
```

The protocol permits remote code evaluation and currently has no
authentication or encryption. Only expose it on a trusted network, preferably
through an authenticated tunnel.

## Evaluation semantics

`roo-server` is intended as a development tool and intentionally loosens some `roo-lang`
semantics.

### Re-evaluation of `def` and `defun`

Redefinition of a symbol or function with `def` or `defun` is not permitted in `roo-lang`.

A successful evaluation of a `def` or `defun` form with `eval-sexp` is allowed to redefine
namespace symbols in non-language namespaces. It is treated as an intentional live edit, as
opposed to a duplicate definition.

