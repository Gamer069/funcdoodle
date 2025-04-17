if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <playlist> <fps>"
    exit 1
fi

playlist="$1"
fps="$2"

image_display_duration=$(echo "scale=4; 1 / $fps" | bc)

mpv --image-display-duration=$image_display_duration --no-correct-pts --keep-open=yes --window-scale=32 --scale=nearest --loop-playlist=inf --fps=$fps $1