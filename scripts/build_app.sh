#!/usr/bin/env bash
set -e

# ========================
# Configuration
# ========================
APP_NAME="${3:-FuncDoodle}"
BIN_DIR="${1:-bin/macos}"
ASSETS_DIR="assets"
THEMES_DIR="themes"
OUTPUT_DIR="${2:-appbin}"
BINARY="$BIN_DIR/FuncDoodle"
APP_DIR="$OUTPUT_DIR/$APP_NAME.app"
MACOS_DIR="$APP_DIR/Contents/MacOS"
RESOURCES_DIR="$APP_DIR/Contents/Resources"

# ========================
# Clean previous build
# ========================
rm -rf "$APP_DIR"
mkdir -p "$MACOS_DIR"
mkdir -p "$RESOURCES_DIR"

# ========================
# Copy binary
# ========================
if [ ! -f "$BINARY" ]; then
	echo "Binary not found: $BINARY"
	exit 1
fi
cp "$BINARY" "$MACOS_DIR/$APP_NAME"
chmod +x "$MACOS_DIR/$APP_NAME"

# ==============================
# Copy assets into MacOS folder
# ==============================
mkdir -p "$MACOS_DIR/assets"
cp -r "$ASSETS_DIR/"* "$MACOS_DIR/assets/"

# ==============================
# Copy themes into MacOS folder
# ==============================
mkdir -p "$MACOS_DIR/themes"
cp -r "$THEMES_DIR/"* "$MACOS_DIR/themes/"

# ========================
# Copy icon (if exists)
# ========================
ICON_SRC="$ASSETS_DIR/icon.icns"
if [ -f "$ICON_SRC" ]; then
    cp "$ICON_SRC" "$RESOURCES_DIR/"
fi

# ========================
# Create Info.plist
# ========================
cat > "$APP_DIR/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleName</key><string>$APP_NAME</string>
  <key>CFBundleExecutable</key><string>$APP_NAME</string>
  <key>CFBundleIdentifier</key><string>com.yourcompany.$APP_NAME</string>
  <key>CFBundleVersion</key><string>1.0</string>
  <key>CFBundlePackageType</key><string>APPL</string>
  <key>CFBundleIconFile</key><string>icon</string>
</dict>
</plist>
EOF

echo "App bundle created at $APP_DIR"
