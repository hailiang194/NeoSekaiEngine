#!/bin/sh
# Build the Web platform in a pinned emscripten/emsdk Docker image, using the
# host's CMake (>= 3.25) to drive the Emscripten toolchain from the image.
#
# Usage: scripts/build-web.sh [-u] [--target <Game>] [-- <extra cmake args>]
#   -u           also build + stage the Vue web UI (BUILD_WEB_UI=ON)
#   --target X   build only target X (output then lands in build/out/X/)
#   -- ...       extra flags forwarded to the configure step

set -eu

REPO_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
IMAGE="emscripten/emsdk:4.0.7"

UI=OFF
TARGET=
EXTRA=
while [ "$#" -gt 0 ]; do
    case "$1" in
        -u) UI=ON; shift ;;
        --target) TARGET=$2; shift 2 ;;
        --) shift; EXTRA=$*; break ;;
        *) EXTRA="$EXTRA $1"; shift ;;
    esac
done

CBIN=$(command -v cmake)
CVER=$(cmake --version | awk '/cmake version/{print $3}' | cut -d. -f1-2)
CSHARE=$(dirname "$(dirname "$CBIN")")/share/cmake-$CVER

echo "host cmake: $CVER ($CBIN)  ui: $UI  target: ${TARGET:-all}  image: $IMAGE"

# A cache written on the host records absolute host paths, which never match the
# container-mount layout (/src). Reconfigure fresh instead of inheriting a stale cache.
if [ -f "$REPO_ROOT/build/CMakeCache.txt" ] && ! grep -q '/src/build' "$REPO_ROOT/build/CMakeCache.txt"; then
    echo "build/ holds a cache for a different filesystem path; archiving it to /tmp/opencode/sekai-build-stale and reconfiguring fresh"
    mv "$REPO_ROOT/build" /tmp/opencode/sekai-build-stale
fi

docker run --rm \
    -v "$REPO_ROOT:/src" \
    -v "$CBIN:/opt/cmake/bin/cmake:ro" \
    -v "$CSHARE:/opt/cmake/share/cmake-$CVER:ro" \
    -e "PATH=/opt/cmake/bin:$PATH" \
    -e "WUI=$UI" \
    -e "WTARGET=${TARGET:-}" \
    -e "WEXTRA=$EXTRA" \
    -w /src \
    "$IMAGE" \
    sh -c '
        set -eu
        echo "configuring (container: $(emcc --version | head -1))"
        emcmake cmake -S . -B build \
            -DPLATFORM=Web -DBUILD_SHARED_LIBS=0 -DBUILD_ENGINE_EXAMPLES=1 \
            -DBUILD_WEB_UI="$WUI" $WEXTRA
        echo "building"
        if [ -n "$WTARGET" ]; then
            cmake --build build --target "$WTARGET"
        else
            cmake --build build
        fi
    '

# Return ownership of the root-written build tree to the calling user
docker run --rm -v "$REPO_ROOT/build:/src/build" "$IMAGE" chown -R "$(id -u):$(id -g)" /src/build