#!/bin/sh

set -eu

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <lookup-binary> <roo-symbol-index>" >&2
  exit 2
fi

lookup_binary=$1
index_path=$2

"$lookup_binary" audit \
  --output-format text \
  --require-summary \
  --require-param-docs \
  --require-signatures \
  --allow-zero-arity roo.io/current-directory! \
  --allow-zero-arity roo.io/home-directory! \
  --allow-zero-arity roo/epoch-ms \
  --fail-on warning \
  "$index_path"
