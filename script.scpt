on run {iconFilePath, targetFilePath}
    tell application "Finder"
        set theFile to POSIX file targetFilePath as alias
        set theIcon to POSIX file iconFilePath as alias
        set icon of theFile to theIcon
    end tell
end run

