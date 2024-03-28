#!/bin/bash

npm run build

input_directory="./webserver/public"
output_directory="./OTPAuth-M5StickC/webserver/dist/gzip"
# Check if gzip is installed
if command -v gzip &> /dev/null; then
    # Further actions if gzip is installed

    # Check if output directory exists
    if [ ! -d "$output_directory" ]; then
        echo "Creating output directory: $output_directory"
        mkdir -p "$output_directory"
    fi

    # Check if input directory exists
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

    # Add PROGMEM declaration and array name to the output file
sed "1s/^/${declaration}/" "$output_file" > "$output_file.tmp" && mv "$output_file.tmp" "$output_file"
    echo "};" >> "$output_file"
#
#    echo "Conversion complete. Output saved to $output_file"
    done
else
    echo "Error: gzip is not installed"
fi
