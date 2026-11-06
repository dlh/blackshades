#!/bin/sh
# Builds dist/blackshades-steamdeck.tar.gz in the Steam Runtime 3 (sniper) SDK, so
# the result runs on SteamOS whatever machine it was built on. Needs Docker; on
# non-x86 hosts Docker emulates linux/amd64, which is slower but works.
set -eu

IMAGE=registry.gitlab.steamos.cloud/steamrt/sniper/sdk@sha256:bb30053d3cd19d35c76ae69b2790dbaa1430448006f8240cb6d7e723f2d1940e
ROOT=$(cd "$(dirname "$0")/../.." && pwd)

exec docker run --rm --platform linux/amd64 \
	--user "$(id -u):$(id -g)" \
	--volume "$ROOT:/src" \
	--workdir /src \
	"$IMAGE" \
	sh packaging/steamdeck/bundle.sh
