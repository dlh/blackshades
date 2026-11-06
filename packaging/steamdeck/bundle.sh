#!/bin/sh
# Runs inside the SDK container; see build.sh.
set -eu

BUILD=build-steamdeck
STAGE=$BUILD/stage/blackshades
SDL=$PWD/$BUILD/sdl
SDL_VERSION=3.4.18
SDL_SHA256=9c75cf16330322c217dedd2e0609f1124f1b54b8633e763467b4684d0f4334a3

# The SDK's own SDL links X11, Wayland, libdecor and more directly, and SteamOS doesn't
# promise those. Upstream's default build loads them at runtime when they're there.
if [ ! -f "$SDL/install/lib/libSDL3.so.0" ]; then
	mkdir -p "$SDL"
	curl -fsSL -o "$SDL/sdl.tar.gz" \
		"https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VERSION/SDL3-$SDL_VERSION.tar.gz"
	echo "$SDL_SHA256  $SDL/sdl.tar.gz" | sha256sum -c -
	tar -xzf "$SDL/sdl.tar.gz" -C "$SDL"
	cmake -S "$SDL/SDL3-$SDL_VERSION" -B "$SDL/build" -G Ninja \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX="$SDL/install" \
		-DCMAKE_INSTALL_LIBDIR=lib \
		-DSDL_DEPS_SHARED=ON \
		-DSDL_STATIC=OFF \
		-DSDL_TEST_LIBRARY=OFF \
		-DSDL_X11=ON \
		-DSDL_WAYLAND=ON \
		-DSDL_OPENGL=ON \
		-DSDL_HIDAPI=ON
	cmake --build "$SDL/build" --target install
fi

# RPATH rather than RUNPATH: it also covers the bundled libraries' own dependencies
# and outranks the LD_LIBRARY_PATH that Steam sets.
cmake -S . -B "$BUILD" -G Ninja \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_PREFIX_PATH="$SDL/install" \
	-DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
	-DCMAKE_INSTALL_RPATH='$ORIGIN/lib' \
	-DCMAKE_EXE_LINKER_FLAGS=-Wl,--disable-new-dtags
cmake --build "$BUILD"

rm -rf "$BUILD/stage"
mkdir -p "$STAGE/lib" "$STAGE/licenses"
strip -o "$STAGE/blackshades" "$BUILD/blackshades"
cp COPYING Readme uDevGame_License uDevGame_Readme "$STAGE/"
tar -cf - --exclude=Highscore --exclude=.DS_Store Data | tar -xf - -C "$STAGE"

# Bundle what SteamOS doesn't promise, with each library's license. The GL driver,
# glibc and libstdc++ must come from the system.
cp "$SDL/SDL3-$SDL_VERSION/LICENSE.txt" "$STAGE/licenses/SDL3.txt"
LD_LIBRARY_PATH=$SDL/install/lib ldd "$BUILD/blackshades" |
	awk '/=> \// { print $1, $3 }' |
	while read -r name path; do
		case "$name" in
			libSDL3.*)
				cp -L "$path" "$STAGE/lib/$name"
				;;
			libopenal.*|libGLU.*|libvorbisfile.*|libvorbis.*|libogg.*)
				cp -L "$path" "$STAGE/lib/$name"
				pkg=$(dpkg -S "$(readlink -f "$path")" | cut -d: -f1)
				cp "/usr/share/doc/$pkg/copyright" "$STAGE/licenses/$pkg.txt"
				;;
		esac
	done

mkdir -p dist
tar -czf dist/blackshades-steamdeck.tar.gz -C "$BUILD/stage" blackshades
echo "Built dist/blackshades-steamdeck.tar.gz"
