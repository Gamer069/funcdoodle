rm -rf dmg
mkdir dmg
cp -r appbin/* dmg/
ln -s /Applications/ dmg/Applications
sudo hdiutil create "dmg/FuncDoodle Installer" -volname "FuncDoodle" -srcfolder dmg/ -size 50M
