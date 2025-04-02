_script_completion() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    case "$COMP_CWORD" in
        1) opts="Debug Release" ;;
        2) opts="true false" ;;
		3) opts="true false" ;;
		4|5) opts=$(compgen -f) ;;
        *) return 0 ;;
    esac
    COMPREPLY=( $(compgen -W "$opts" -- "$cur") )
    return 0
}

complete -F _script_completion build_cross.sh
