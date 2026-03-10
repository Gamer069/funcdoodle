#!/usr/bin/env bash
set -euo pipefail

src_dir="${1:-appbin}"
out_dir="${2:-release}"
dmg_dir="$out_dir/dmg"

rm -rf "$dmg_dir"
mkdir -p "$dmg_dir"
cp -r "$src_dir"/*.app "$dmg_dir/"
ln -s /Applications/ "$dmg_dir/Applications"
sudo hdiutil create "$out_dir/FuncDoodle Installer" -volname "FuncDoodle" -srcfolder "$dmg_dir/" -size 50M
