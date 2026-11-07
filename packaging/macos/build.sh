#!/bin/sh
# Builds dist/blackshades-macos.zip, holding Black Shades.app with the Homebrew
# libraries it links bundled inside, so it runs without Homebrew. Needs the Homebrew
# packages in README.md. The app needs the macOS version it was built on, or later,
# because Homebrew builds its libraries for that version.
set -eu

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
cd "$ROOT"

BUILD=build-macos
APP="$BUILD/stage/Black Shades.app"
FRAMEWORKS="$APP/Contents/Frameworks"
RESOURCES="$APP/Contents/Resources"
BREW=$(brew --prefix)

# The bundle ID names whoever publishes this build, so take it from the GitHub
# repository (as Actions reports it, or from the origin remote) to give each fork
# its own. Set BUNDLE_ID to override it.
if [ -z "${BUNDLE_ID:-}" ]; then
	REPO=${GITHUB_REPOSITORY:-$(git remote get-url origin 2>/dev/null |
		sed -nE 's#^(https://|ssh://git@|git@)github\.com[:/]([^/]+/[^/]+)$#\2#p' |
		sed 's/\.git$//')}
	if [ -n "$REPO" ]; then
		BUNDLE_ID=io.github.$(echo "$REPO" | tr 'A-Z' 'a-z' | sed 's#[^a-z0-9/-]#-#g; s#/#.#')
	else
		BUNDLE_ID=local.blackshades
	fi
fi

# Only the bundle's own rpath: the build-tree ones would load Homebrew's copies first.
cmake -S . -B "$BUILD" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
	-DCMAKE_INSTALL_RPATH=@executable_path/../Frameworks
cmake --build "$BUILD" -j

rm -rf "$BUILD/stage"
mkdir -p "$APP/Contents/MacOS" "$FRAMEWORKS" "$RESOURCES/licenses"
strip -o "$APP/Contents/MacOS/blackshades" "$BUILD/blackshades"
cp COPYING Readme uDevGame_License uDevGame_Readme "$RESOURCES/"
tar -cf - --exclude=Highscore --exclude=.DS_Store Data | tar -xf - -C "$RESOURCES"

# Copies a Homebrew library's license files, found at the top of its keg.
license() {
	keg=$(dirname "$(dirname "$(realpath "$1")")")
	formula=$(basename "$(dirname "$keg")")
	found=
	for file in "$keg"/LICENSE* "$keg"/COPYING*; do
		if [ -f "$file" ]; then
			cp "$file" "$RESOURCES/licenses/$formula-$(basename "$file" .txt).txt"
			found=1
		fi
	done
	if [ -z "$found" ]; then
		echo "error: no license file in $keg" >&2
		exit 1
	fi
}

# Copies each Homebrew library that $1 links into Frameworks, recursively, and
# points $1 at the copy. The first line otool prints for a library is its own ID.
bundle() {
	for dep in $(otool -L "$1" | awk 'NR > 1 { print $1 }' | grep "^$BREW/" || true); do
		name=$(basename "$dep")
		install_name_tool -change "$dep" "@rpath/$name" "$1"
		if [ ! -f "$FRAMEWORKS/$name" ]; then
			cp -L "$dep" "$FRAMEWORKS/$name"
			chmod u+w "$FRAMEWORKS/$name"
			install_name_tool -id "@rpath/$name" "$FRAMEWORKS/$name"
			license "$dep"
			bundle "$FRAMEWORKS/$name"
		fi
	done
}

bundle "$APP/Contents/MacOS/blackshades"

# Anything left outside the system must have been missed.
RPATHS=$(otool -l "$APP/Contents/MacOS/blackshades" | awk '/LC_RPATH/ { getline; getline; print $2 }')
if [ "$RPATHS" != "@executable_path/../Frameworks" ]; then
	echo "error: unexpected rpaths: $RPATHS" >&2
	exit 1
fi
for file in "$APP/Contents/MacOS/blackshades" "$FRAMEWORKS"/*.dylib; do
	otool -L "$file" | awk 'NR > 1 { print $1 }' | while read -r dep; do
		case "$dep" in
			/usr/lib/*|/System/*)
				;;
			@rpath/*)
				[ -f "$FRAMEWORKS/${dep#@rpath/}" ] || { echo "error: $file needs $dep" >&2; exit 1; }
				;;
			*)
				echo "error: $file needs $dep" >&2
				exit 1
				;;
		esac
	done
done

# The app can't run on an older macOS than any of its parts.
MINOS=$(for file in "$APP/Contents/MacOS/blackshades" "$FRAMEWORKS"/*.dylib; do
	vtool -show-build "$file" | awk '/minos/ { print $2 }'
done | sort -t. -k1,1n -k2,2n | tail -n 1)

cat > "$APP/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleDisplayName</key>
	<string>Black Shades</string>
	<key>CFBundleExecutable</key>
	<string>blackshades</string>
	<key>CFBundleIdentifier</key>
	<string>$BUNDLE_ID</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>Black Shades</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>LSApplicationCategoryType</key>
	<string>public.app-category.action-games</string>
	<key>LSMinimumSystemVersion</key>
	<string>$MINOS</string>
	<key>NSHighResolutionCapable</key>
	<true/>
</dict>
</plist>
EOF

# install_name_tool voids the libraries' signatures, and Apple Silicon won't load
# unsigned code. An ad-hoc signature satisfies that, though not Gatekeeper.
codesign --force --sign - "$FRAMEWORKS"/*.dylib
codesign --force --sign - "$APP"
codesign --verify --strict --deep "$APP"

mkdir -p dist
rm -f dist/blackshades-macos.zip
ditto -c -k --keepParent "$APP" dist/blackshades-macos.zip
echo "Built dist/blackshades-macos.zip ($BUNDLE_ID, macOS $MINOS or later)"
