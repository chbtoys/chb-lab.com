#!/bin/bash

create_dir_if_not_exists() {
    local dir_path="$1"

    if [[ ! -d "$dir_path" ]]; then
        echo "Creating directory: $dir_path"
        mkdir -p "$dir_path"
    else
        echo "Directory already exists: $dir_path"
    fi
}

clear
create_dir_if_not_exists "../output"
create_dir_if_not_exists "../builds"

for i in {1..5}; do
    let output_index=i
    printf -v formatted_index "%02d" $i
    printf -v formatted_output_index "%02d" $output_index

    echo ==============================================================
    echo "// Compile: clear && clang++ -std=c++20 intro_${formatted_index}.cpp -o fptc_${formatted_output_index}_binary"
    clang++ -std=c++20 intro_${formatted_index}.cpp -o fptc_${formatted_output_index}_binary
    echo ./fptc_${formatted_output_index}_binary
    ./fptc_${formatted_output_index}_binary > ../output/fptc_${formatted_output_index}_binary.txt
done

for i in {1..32}; do
    let output_index=i+5
    printf -v formatted_index "%02d" $i
    printf -v formatted_output_index "%02d" $output_index

    echo ==============================================================
    echo "// Compile: clear && clang++ -std=c++20 ${formatted_index}-main.cpp -o fptc_${formatted_output_index}_binary"
    clang++ -std=c++20 ${formatted_index}-main.cpp -o fptc_${formatted_output_index}_binary
    echo ./fptc_${formatted_output_index}_binary
    ./fptc_${formatted_output_index}_binary > ../output/fptc_${formatted_output_index}_binary.txt
done

echo ==============================================================
echo CLEAN UP
for i in {1..37}; do
    printf -v formatted_index "%02d" $i

    echo mv fptc_${formatted_index}_binary ../builds/
    mv fptc_${formatted_index}_binary ../builds/
done

echo Finished.