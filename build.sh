#!/bin/sh

jq '. | .buildDirs.executable.flags |= . + ["-ccopt", "-static"]' package.json > package.json.new && mv package.json.new package.json
npx esy i
npx esy verify-fnm-package
npx esy pesy
npx esy b --release
npx esy test
