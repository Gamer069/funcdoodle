# Create structure
mkdir -p FuncDoodle.app
mkdir -p FuncDoodle.app/Contents
mkdir -p FuncDoodle.app/Contents/MacOS
mkdir -p FuncDoodle.app/Contents/Resources

# Copy executable so the app would function
cp build/FuncDoodle FuncDoodle.app/Contents/MacOS/FuncDoodle

# Convert icon to .icns and copy to the required path
cp icon.png icon2.png
magick icon2.png FuncDoodle.app/Contents/Resources/AppIcon.icns
cp FuncDoodle.app/Contents/Resources/AppIcon.icns icon.icns

cp -r assets FuncDoodle.app/Contents/MacOS/

# Register all in .plist
echo "
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleIdentifier</key>
	<string>me.illia.FuncDoodle</string>
	<key>CFBundleName</key>
	<string>FuncDoodle</string>
	<key>CFBundleIconFile</key>
	<string>AppIcon.icns</string>
	<key>CFBundleExecutable</key>
	<string>FuncDoodle</string>
	<key>CFBundleGetInfoString</key>
	<string></string>
	<key>CFBundleLongVersionString</key>
	<string>3.5.0</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>3.5.0</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>CFBundleVersion</key>
	<string>0.0.1</string>
	<key>CSResourcesFileMapped</key>
	<true/>
	<key>NSHighResolutionCapable</key>
	<true/>
	<key>LSRequiresCarbon</key>
	<true/>
	<key>NSHumanReadableCopyright</key>
	<string></string>
	<key>LSApplicationCategoryType</key>
	<string>Animation</string>
</dict>
</plist>
" > FuncDoodle.app/Contents/Info.plist

osascript script.scpt icon.icns FuncDoodle.app

mkdir -p appbuild
cp -r FuncDoodle.app appbuild/
rm -rf FuncDoodle.app

open appbuild/FuncDoodle.app
