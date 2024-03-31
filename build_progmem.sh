#!/bin/bash

npm run build

input_directory="./webserver/public"
output_directory="./OTPAuth-M5StickC/webserver/dist/gzip"

if command -v gzip &> /dev/null; then
    if [ ! -d "$output_directory" ]; then
        echo "Creating output directory: $output_directory"
        mkdir -p "$output_directory"
    fi

    if [ ! -d "$input_directory" ]; then
        echo "Error: Input directory not found"
        exit 1
    fi

   for file in "$input_directory"/*; do
     output_file="${output_directory}/$(basename "${file}" .gz).h"
     base_name=$(basename "$file" .gz)
     array_name="${base_name//./_}_gz"
     declaration="const char ${array_name}[] PROGMEM = {\\n"
     gzip -c "$file"  | xxd -i > "${output_file}"

sed "1s/^/${declaration}/" "$output_file" > "$output_file.tmp" && mv "$output_file.tmp" "$output_file"
    echo "};" >> "$output_file"
    done
else
    echo "Error: gzip is not installed"
fi
