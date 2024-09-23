#!/bin/bash
export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH

# Directorio donde están los videos
VIDEO_DIR="$(pwd)/videos"

# Itera sobre todos los archivos de video en el directorio
for video in "$VIDEO_DIR"/*; do
    # Verifica si el archivo es un video (puedes ajustar las extensiones según tus necesidades)
    if [[ $video == *.mp4 || $video == *.avi || $video == *.mov ]]; then
        echo "Procesando $video"
        ./OpticalFlow "$video" "$@"
    fi
done

