#!/bin/bash

check_zlib() {
    local os_name="$(uname -s)"

    case "$os_name" in
        Linux* | MSYS* | MINGW*)
            # Check for zlib on Linux or MSYS2/MINGW
            if ldconfig -p | grep -q libz.so 2>/dev/null; then
                echo "=============================================================="
                echo "zlib library found."
            else
                echo "=============================================================="
                echo "> zlib library not found. Please install zlib. <"
                return 1
            fi

            if [ -f /usr/include/zlib.h ]; then
                echo "zlib header files found."
            else
                echo "=============================================================="
                echo "> zlib header files not found. Please install zlib development package <"
                return 1
            fi
            ;;

        Darwin*)
            # Check for zlib on macOS
            if [ -f /usr/local/opt/zlib/include/zlib.h ]; then
                echo "=============================================================="
                echo "zlib header files found on macOS."
            else
                echo "=============================================================="
                echo "> zlib header files not found on macOS. Consider installing zlib via Homebrew. <"
                return 1
            fi
            ;;

        *)
            echo "=============================================================="
            echo "> Unsupported operating system: $os_name <"
            return 1
            ;;
    esac

    echo "=============================================================="
    echo "zlib checks passed."
    return 0
}

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
create_dir_if_not_exists "output"
create_dir_if_not_exists "builds"
create_dir_if_not_exists "ppm"

# 01_tuples
echo ==============================================================
head -1 01_tuples.cpp
clang++ -std=c++20 01_tuples.cpp -o 01_tuples
echo ./01_tuples
./01_tuples > output/01_tuples.txt

# 02_index.cpp
echo ==============================================================
head -1 02_index.cpp
clang++ -std=c++20 02_index.cpp -o 02_index
echo ./02_index
./02_index > output/02_index.txt

# 03_calcsize.cpp
echo ==============================================================
head -1 03_calcsize.cpp
clang++ -std=c++20 03_calcsize.cpp -o 03_calcsize
echo ./03_calcsize
./03_calcsize > output/03_calcsize.txt

# 04_helloworld.cpp
echo ==============================================================
head -1 04_helloworld.cpp
clang++ -std=c++20 04_helloworld.cpp -o 04_helloworld
echo ./04_helloworld
./04_helloworld > output/04_helloworld.txt

# 05_randomwalker.cpp
echo ==============================================================
head -1 05_randomwalker.cpp
clang++ -std=c++20 05_randomwalker.cpp -o 05_randomwalker
echo ./05_randomwalker
./05_randomwalker > output/05_randomwalker.txt

# 06_xor_and_or_texture.cpp
echo ==============================================================
head -1 06_xor_and_or_texture.cpp
clang++ -std=c++20 06_xor_and_or_texture.cpp -o 06_xor_and_or_texture
echo ./06_xor_and_or_texture
./06_xor_and_or_texture > output/06_xor_and_or_texture.txt

# 07_clouds_marble_wood_texture.cpp
echo ==============================================================
head -1 07_clouds_marble_wood_texture.cpp
clang++ -std=c++20 07_clouds_marble_wood_texture.cpp -o 07_clouds_marble_wood_texture
echo ./07_clouds_marble_wood_texture
./07_clouds_marble_wood_texture > output/07_clouds_marble_wood_texture.txt

# 08_light_and_color.cpp
echo ==============================================================
head -1 08_light_and_color.cpp
clang++ -std=c++20 08_light_and_color.cpp -o 08_light_and_color
echo ./08_light_and_color
./08_light_and_color > output/08_light_and_color.txt

# 09_lines.cpp & 09_angle_lines.cpp
echo ==============================================================
head -1 09_lines.cpp 
clang++ -std=c++20 09_lines.cpp -o 09_lines
echo ./09_lines
./09_lines > output/09_lines.txt
head -1 09_angle_lines.cpp
clang++ -std=c++20 09_angle_lines.cpp -o 09_angle_lines
echo ./09_angle_lines
./09_angle_lines > output/09_angle_lines.txt

# 10_curved_dots.cpp & 10_curved_lines.cpp
echo ==============================================================
head -1 10_curved_dots.cpp
clang++ -std=c++20 10_curved_dots.cpp -o 10_curved_dots
echo ./10_curved_dots
./10_curved_dots > output/10_curved_dots.txt
head -1 10_curved_lines.cpp
clang++ -std=c++20 10_curved_lines.cpp -o 10_curved_lines
echo ./10_curved_lines
./10_curved_lines > output/10_curved_lines.txt

# 11_rectangles.cpp & 11_rectangles_from_file.cpp
echo ==============================================================
head -1 11_rectangles.cpp
clang++ -std=c++20 11_rectangles.cpp -o 11_rectangles
echo ./11_rectangles
./11_rectangles > output/11_rectangles.txt
head -1 11_rectangles_from_file.cpp 
clang++ -std=c++20 11_rectangles_from_file.cpp -o 11_rectangles_from_file
echo ./11_rectangles_from_file
./11_rectangles_from_file > output/11_rectangles_from_file.txt

# 12_circle.cpp & 12_wedge.cpp & 12_circles_from_file.cpp
echo ==============================================================
head -1 12_circle.cpp
clang++ -std=c++20 12_circle.cpp -o 12_circle
echo ./12_circle
./12_circle > output/12_circle.txt
head -1 12_wedge.cpp
clang++ -std=c++20 12_wedge.cpp -o 12_wedge
echo ./12_wedge
./12_wedge > output/12_wedge.txt
head -1 12_circles_from_file.cpp
clang++ -std=c++20 12_circles_from_file.cpp -o 12_circles_from_file
echo ./12_circles_from_file
./12_circles_from_file > output/12_circles_from_file.txt

# 13_triangles.cpp & 13_triangles_from_file.cpp
echo ==============================================================
head -1 13_triangles.cpp
clang++ -std=c++20 13_triangles.cpp -o 13_triangles
echo ./13_triangles
./13_triangles > output/13_triangles.txt
head -1 13_triangles_from_file.cpp
clang++ -std=c++20 13_triangles_from_file.cpp -o 13_triangles_from_file
echo ./13_triangles_from_file
./13_triangles_from_file > output/13_triangles_from_file.txt

# 14_animation05.cpp & 14_animation11.cpp & 14_animation12.cpp & 14_animation13.cpp
echo ==============================================================
head -1 14_animation05.cpp
clang++ -std=c++20 14_animation05.cpp -o 14_animation05
echo ./14_animation05
head -1 14_animation11.cpp
clang++ -std=c++20 14_animation11.cpp -o 14_animation11
echo ./14_animation11
head -1 14_animation12.cpp
clang++ -std=c++20 14_animation12.cpp -o 14_animation12
echo ./14_animation12
head -1 14_animation13.cpp
clang++ -std=c++20 14_animation13.cpp -o 14_animation13
echo ./14_animation13

# 15_draw_font.cpp & 15_weekchart.cpp & 15_radarchart.cpp & 15_read_p6_ppm.cpp
echo ==============================================================
head -1 15_draw_font.cpp
clang++ -std=c++20 15_draw_font.cpp -o 15_draw_font
echo ./15_draw_font
./15_draw_font > output/15_draw_font.txt
head -1 15_weekchart.cpp
clang++ -std=c++20 15_weekchart.cpp -o 15_weekchart
echo ./15_weekchart
./15_weekchart > output/15_weekchart.txt
head -1 15_radarchart.cpp
clang++ -std=c++20 15_radarchart.cpp -o 15_radarchart
echo ./15_radarchart
./15_radarchart > output/15_radarchart.txt
head -2 15_read_p6_ppm.cpp
cp 15_weekchart.ppm 15_read_p6_ppm.ppm
clang++ -std=c++20 15_read_p6_ppm.cpp -o 15_read_p6_ppm
echo ./15_read_p6_ppm
./15_read_p6_ppm > output/15_read_p6_ppm.txt

# 16_draw_bar_chart.cpp & 16_draw_donut_chart.cpp & 16_draw_investment.cpp &
# 16_draw_market_size.cpp & 16_draw_pie_chart.cpp & 16_draw_table.cpp
echo ==============================================================
head -1 16_draw_bar_chart.cpp
clang++ -std=c++20 16_draw_bar_chart.cpp -o 16_draw_bar_chart
echo ./16_draw_bar_chart
./16_draw_bar_chart > output/16_draw_bar_chart.txt
head -1 16_draw_donut_chart.cpp
clang++ -std=c++20 16_draw_donut_chart.cpp -o 16_draw_donut_chart
echo ./16_draw_donut_chart
./16_draw_donut_chart > output/16_draw_donut_chart.txt
head -1 16_draw_investment.cpp
clang++ -std=c++20 16_draw_investment.cpp -o 16_draw_investment
echo ./16_draw_investment
./16_draw_investment > output/16_draw_investment.txt
head -1 16_draw_market_size.cpp
clang++ -std=c++20 16_draw_market_size.cpp -o 16_draw_market_size
echo ./16_draw_market_size
./16_draw_market_size > output/16_draw_market_size.txt
head -1 16_draw_pie_chart.cpp
clang++ -std=c++20 16_draw_pie_chart.cpp -o 16_draw_pie_chart
echo ./16_draw_pie_chart
./16_draw_pie_chart > output/16_draw_pie_chart.txt
head -1 16_draw_table.cpp
clang++ -std=c++20 16_draw_table.cpp -o 16_draw_table
echo ./16_draw_table
./16_draw_table > output/16_draw_table.txt

# 17_colab_peacock.cpp
echo ==============================================================
head -1 17_colab_peacock.cpp
clang++ -std=c++20 17_colab_peacock.cpp -o 17_colab_peacock
echo ./17_colab_peacock
./17_colab_peacock > output/17_colab_peacock.txt

# 18_concentric_hexagons.cpp
echo ==============================================================
head -1 18_concentric_hexagons.cpp
clang++ -std=c++20 18_concentric_hexagons.cpp -o 18_concentric_hexagons
echo ./18_concentric_hexagons
./18_concentric_hexagons > output/18_concentric_hexagons.txt

# 19_triangle_grid.cpp
echo ==============================================================
head -1 19_triangle_grid.cpp
clang++ -std=c++20 19_triangle_grid.cpp -o 19_triangle_grid
echo ./19_triangle_grid
./19_triangle_grid > output/19_triangle_grid.txt

# 20_color_quantizer.cpp
echo ==============================================================
head -1 20_color_quantizer.cpp
clang++ -std=c++20 20_color_quantizer.cpp -o 20_color_quantizer
echo ./20_color_quantizer
./20_color_quantizer > output/20_color_quantizer.txt

# 21_exchange_colors.cpp
echo ==============================================================
head -1 21_exchange_colors.cpp
clang++ -std=c++20 21_exchange_colors.cpp -o 21_exchange_colors
echo ./21_exchange_colors
./21_exchange_colors > output/21_exchange_colors.txt

if check_zlib; then
	# 22_sprite_animation_tester.cpp
	head -1 22_sprite_animation_tester.cpp
	clang++ -std=c++20 -lz 22_sprite_animation_tester.cpp -o 22_sprite_animation_tester
	echo ./22_sprite_animation_tester
	./22_sprite_animation_tester > output/22_sprite_animation_tester.txt

	# 22_PPM_compressor.cpp
	head -1 22_PPM_compressor.cpp
	clang++ -std=c++20 -lz 22_PPM_compressor.cpp -o 22_PPM_compressor
	echo ./22_PPM_compressor
	./22_PPM_compressor test.ppm 22_test.cppm > output/22_PPM_compressor_testppm_ntestcppm.txt
	./22_PPM_compressor 22_test.cppm > output/22_PPM_compressor_ntestcppm_ntestppm.txt
fi

# 23_anti_aliasing.cpp
echo ==============================================================
head -1 23_anti_aliasing.cpp
clang++ -std=c++20 23_anti_aliasing.cpp -o 23_anti_aliasing
echo ./23_anti_aliasing
./23_anti_aliasing > output/23_anti_aliasing.txt

# 24_bloom.cpp
echo ==============================================================
head -1 24_bloom.cpp
clang++ -std=c++20 24_bloom.cpp -o 24_bloom
echo ./24_bloom
./24_bloom > output/24_bloom.txt

# 25_lens.cpp
echo ==============================================================
head -1 25_lens.cpp
clang++ -std=c++20 25_lens.cpp -o 25_lens
echo ./25_lens
./25_lens > output/25_lens.txt

if check_zlib; then
	# 26_easing_animation.cpp
	head -1 26_easing_animation.cpp
	clang++ -std=c++20 -lz 26_easing_animation.cpp -o 26_easing_animation
	echo ./26_easing_animation
	./26_easing_animation > output/26_easing_animation.txt
fi

# 27_image_to_patterns.cpp
echo ==============================================================
head -1 27_image_to_patterns.cpp
clang++ -std=c++20 27_image_to_patterns.cpp -o 27_image_to_patterns
echo ./27_image_to_patterns
./27_image_to_patterns > output/27_image_to_patterns.txt

# 28_vintage_photo.cpp
echo ==============================================================
head -1 28_vintage_photo.cpp
clang++ -std=c++20 28_vintage_photo.cpp -o 28_vintage_photo
echo ./28_vintage_photo
./28_vintage_photo > output/28_vintage_photo.txt

# test.cpp & ppmpp.hpp
echo ==============================================================
head -1 test.cpp
clang++ -std=c++20 test.cpp -o test
echo ./test
./test > output/ppmpp-hpp_test.txt

# Clean Up
echo ==============================================================
echo CLEAN UP
echo mv 01_tuples builds/
mv 01_tuples builds/
echo mv 02_index builds/
mv 02_index builds/
echo mv 03_calcsize builds/
mv 03_calcsize builds/
echo mv 04_helloworld builds/
mv 04_helloworld builds/
echo mv 04_helloworld.ppm ppm/
mv 04_helloworld.ppm ppm/
echo mv 04_helloworld_square.ppm ppm/
mv 04_helloworld_square.ppm ppm/
echo mv 05_randomwalker builds/
mv 05_randomwalker builds/
echo mv 05_randomwalker.ppm ppm/
mv 05_randomwalker.ppm ppm/
echo mv 06_xor_and_or_texture builds/
mv 06_xor_and_or_texture builds/
echo mv 06_and_texture.ppm ppm/
mv 06_and_texture.ppm ppm/
echo mv 06_or_texture.ppm ppm/
mv 06_or_texture.ppm ppm/
echo mv 06_xor_texture.ppm ppm/
mv 06_xor_texture.ppm ppm/
echo mv 07_clouds_marble_wood_texture builds/
mv 07_clouds_marble_wood_texture builds/
echo mv 07_cloud_texture.ppm ppm/
mv 07_cloud_texture.ppm ppm/
echo mv 07_marble_texture.ppm ppm/
mv 07_marble_texture.ppm ppm/
echo mv 07_wood_texture.ppm ppm/
mv 07_wood_texture.ppm ppm/
echo mv 08_light_and_color builds/
mv 08_light_and_color builds/
echo mv 08_add_for_brighter.ppm ppm/
mv 08_add_for_brighter.ppm ppm/
echo mv 08_bgr_image.ppm ppm/
mv 08_bgr_image.ppm ppm/
echo mv 08_blended_two_images2.ppm ppm/
mv 08_blended_two_images2.ppm ppm/
echo mv 08_blended_two_images.ppm ppm/
mv 08_blended_two_images.ppm ppm/
echo mv 08_double_as_dark.ppm ppm/
mv 08_double_as_dark.ppm ppm/
echo mv 08_grb_image.ppm ppm/
mv 08_grb_image.ppm ppm/
echo mv 08_greyscale_image.ppm ppm/
mv 08_greyscale_image.ppm ppm/
echo mv 08_negative_image.ppm ppm/
mv 08_negative_image.ppm ppm/
echo mv 08_onefive_as_dark.ppm ppm/
mv 08_onefive_as_dark.ppm ppm/
echo mv 08_r_zero_b_image.ppm ppm/
mv 08_r_zero_b_image.ppm ppm/
echo mv 08_rg_zero_image.ppm ppm/
mv 08_rg_zero_image.ppm ppm/
echo mv 08_rrb_image.ppm ppm/
mv 08_rrb_image.ppm ppm/
echo mv 08_sub_for_darker.ppm ppm/
mv 08_sub_for_darker.ppm ppm/
echo mv 08_twice_as_bright.ppm ppm/
mv 08_twice_as_bright.ppm ppm/
echo mv 08_zero_gb_image.ppm ppm/
mv 08_zero_gb_image.ppm ppm/
echo mv 09_lines builds/
mv 09_lines builds/
echo mv 09_angle_lines builds/
mv 09_angle_lines builds/
echo mv 09_random_lines.ppm ppm/
mv 09_random_lines.ppm ppm/
echo mv 09_damped_sine.ppm ppm/
mv 09_damped_sine.ppm ppm/
echo mv 09_angle_lines.ppm ppm/
mv 09_angle_lines.ppm ppm/
echo mv 10_curved_dots builds/
mv 10_curved_dots builds/
echo mv 10_curved_lines builds/
mv 10_curved_lines builds/
echo mv 10_curved_dots.ppm ppm/
mv 10_curved_dots.ppm ppm/
echo mv 10_curved_lines.ppm ppm/
mv 10_curved_lines.ppm ppm/
echo mv 11_rectangles builds/
mv 11_rectangles builds/
echo mv 11_rectangles_from_file builds/
mv 11_rectangles_from_file builds/
echo mv 11_rectangles.ppm ppm/
mv 11_rectangles.ppm ppm/
echo mv 11_squint_with_your_eyes.ppm ppm/
mv 11_squint_with_your_eyes.ppm ppm/
echo mv 12_circle builds/
mv 12_circle builds/
echo mv 12_wedge builds/
mv 12_wedge builds/
echo mv 12_circles_from_file builds/
mv 12_circles_from_file builds/
echo mv 12_circle.ppm ppm/
mv 12_circle.ppm ppm/
echo mv 12_wedge.ppm ppm/
mv 12_wedge.ppm ppm/
echo mv 12_squint_with_your_eyes.ppm ppm/
mv 12_squint_with_your_eyes.ppm ppm/
echo mv 13_triangles builds/
mv 13_triangles builds/
echo mv 13_triangles_from_file builds/
mv 13_triangles_from_file builds/
echo mv 13_trangles.ppm ppm/
mv 13_trangles.ppm ppm/
echo mv 13_squint_with_your_eyes.ppm ppm/
mv 13_squint_with_your_eyes.ppm ppm/
echo mv 14_animation05 builds/
mv 14_animation05 builds/
echo mv 14_animation11 builds/
mv 14_animation11 builds/
echo mv 14_animation12 builds/
mv 14_animation12 builds/
echo mv 14_animation13 builds/
mv 14_animation13 builds/
echo mv 15_draw_font builds/
mv 15_draw_font builds/
echo mv 15_weekchart builds/
mv 15_weekchart builds/
echo mv 15_radarchart builds/
mv 15_radarchart builds/
echo mv 15_read_p6_ppm builds/
mv 15_read_p6_ppm builds/
echo mv 15_radarchart.ppm ppm/
mv 15_radarchart.ppm ppm/
echo mv 15_weekchart.ppm ppm/
mv 15_weekchart.ppm ppm/
echo mv 15_read_p6_ppm_.ppm ppm/
mv 15_read_p6_ppm_.ppm ppm/
echo mv 15_read_p6_ppm.ppm ppm/
mv 15_read_p6_ppm.ppm ppm/
echo mv 15_font.ppm ppm/
mv 15_draw_font_procent.ppm ppm/
mv 15_draw_font_0.ppm ppm/
mv 15_draw_font_1.ppm ppm/
mv 15_draw_font_2.ppm ppm/
mv 15_draw_font_3.ppm ppm/
mv 15_draw_font_4.ppm ppm/
mv 15_draw_font_5.ppm ppm/
mv 15_draw_font_6.ppm ppm/
mv 15_draw_font_7.ppm ppm/
mv 15_draw_font_8.ppm ppm/
mv 15_draw_font_9.ppm ppm/
mv 15_draw_font_questionmark.ppm ppm/
mv 15_draw_font_A.ppm ppm/
mv 15_draw_font_B.ppm ppm/
mv 15_draw_font_C.ppm ppm/
mv 15_draw_font_D.ppm ppm/
mv 15_draw_font_E.ppm ppm/
mv 15_draw_font_F.ppm ppm/
mv 15_draw_font_G.ppm ppm/
mv 15_draw_font_H.ppm ppm/
mv 15_draw_font_I.ppm ppm/
mv 15_draw_font_J.ppm ppm/
mv 15_draw_font_K.ppm ppm/
mv 15_draw_font_L.ppm ppm/
mv 15_draw_font_M.ppm ppm/
mv 15_draw_font_N.ppm ppm/
mv 15_draw_font_O.ppm ppm/
mv 15_draw_font_P.ppm ppm/
mv 15_draw_font_Q.ppm ppm/
mv 15_draw_font_R.ppm ppm/
mv 15_draw_font_S.ppm ppm/
mv 15_draw_font_T.ppm ppm/
mv 15_draw_font_U.ppm ppm/
mv 15_draw_font_V.ppm ppm/
mv 15_draw_font_W.ppm ppm/
mv 15_draw_font_X.ppm ppm/
mv 15_draw_font_Y.ppm ppm/
mv 15_draw_font_Z.ppm ppm/
echo mv 16_draw_bar_chart builds/
mv 16_draw_bar_chart builds/
echo mv 16_draw_donut_chart builds/
mv 16_draw_donut_chart builds/
echo mv 16_draw_investment builds/
mv 16_draw_investment builds/
echo mv 16_draw_market_size builds/
mv 16_draw_market_size builds/
echo mv 16_draw_pie_chart builds/
mv 16_draw_pie_chart builds/
echo mv 16_draw_table builds/
mv 16_draw_table builds/
echo mv 16_draw_bar_chart.ppm ppm/
mv 16_draw_bar_chart.ppm ppm/
echo mv 16_draw_donut_chart.ppm	ppm/
mv 16_draw_donut_chart.ppm ppm/
echo mv 16_draw_investment.ppm ppm/
mv 16_draw_investment1.ppm ppm/
mv 16_draw_investment2.ppm ppm/
mv 16_draw_investment3.ppm ppm/
mv 16_draw_investment4.ppm ppm/
mv 16_draw_investment5.ppm ppm/
mv 16_draw_investment6.ppm ppm/
mv 16_draw_investment7.ppm ppm/
mv 16_draw_investment8.ppm ppm/
mv 16_draw_investment9.ppm ppm/
mv 16_draw_investment10.ppm ppm/
mv 16_draw_investment11.ppm ppm/
echo mv 16_draw_market_size.ppm ppm/
mv 16_draw_market_size1.ppm ppm/
mv 16_draw_market_size2.ppm ppm/
mv 16_draw_market_size3.ppm ppm/
mv 16_draw_market_size4.ppm ppm/
mv 16_draw_market_size5.ppm ppm/
mv 16_draw_market_size6.ppm ppm/
mv 16_draw_market_size7.ppm ppm/
mv 16_draw_market_size8.ppm ppm/
mv 16_draw_market_size9.ppm ppm/
mv 16_draw_market_size10.ppm ppm/
mv 16_draw_market_size11.ppm ppm/
mv 16_draw_market_size12.ppm ppm/
mv 16_draw_market_size13.ppm ppm/
mv 16_draw_market_size14.ppm ppm/
mv 16_draw_market_size15.ppm ppm/
echo mv 16_draw_pie_chart.ppm ppm/
mv 16_draw_pie_chart.ppm ppm/
echo mv 16_draw_table.ppm ppm/
mv 16_draw_table.ppm ppm/
echo mv 17_colab_peacock builds/
mv 17_colab_peacock builds/
echo mv 17_colab_peacock.ppm ppm/
mv 17_colab_peacock.ppm ppm/
echo mv 18_concentric_hexagons builds/
mv 18_concentric_hexagons builds/
echo mv 18_concentric_hexagons.ppm ppm/
mv 18_concentric_hexagons.ppm ppm/
echo mv 18_concentric_filled_hexagons.ppm ppm/
mv 18_concentric_filled_hexagons.ppm ppm/
echo mv 18_concentric_textured_hexagons.ppm ppm/
mv 18_concentric_textured_hexagons.ppm ppm/
echo mv 19_triangle_grid builds/
mv 19_triangle_grid builds/
echo mv 19_triangle_grid.ppm ppm/
mv 19_triangle_grid.ppm ppm/
echo mv 20_color_quantizer builds/
mv 20_color_quantizer builds/
echo mv 20_colorquantizer_quantized.txt ppm/
mv 20_colorquantizer_quantized.txt ppm/
echo mv 20_colorquantizer_quantized.dat ppm/
mv 20_colorquantizer_quantized.dat ppm/
echo mv 20_colorquantizer_quantized.ppm ppm/
mv 20_colorquantizer_quantized.ppm ppm/
echo mv 21_exchange_colors builds/
mv 21_exchange_colors builds/
echo mv 21_exchangecolors.ppm ppm/
mv 21_exchangecolors.ppm ppm/
if check_zlib; then
	echo mv 22_sprite_animation_tester builds/
	mv 22_sprite_animation_tester builds/
	echo mv 22_sprite.png ppm/
	mv 22_sprite.png ppm/
	echo mv 22_PPM_compressor builds/
	mv 22_PPM_compressor builds/
	echo mv 22_test.cppm ppm/
	mv 22_test.cppm ppm/
	echo mv 22_test.ppm ppm/
	mv 22_test.ppm ppm/
    echo ==============================================================
fi
echo mv 23_anti_aliasing builds/
mv 23_anti_aliasing builds/
echo mv 23_anti_aliasing.ppm ppm/
mv 23_anti_aliasing.ppm ppm/
echo mv 24_bloom builds/
mv 24_bloom builds/
echo mv 24_bloom.ppm ppm/
mv 24_bloom.ppm ppm/
echo mv 25_lens builds/
mv 25_lens builds/
echo mv 25_lens.ppm ppm/
mv 25_lens.ppm ppm/
if check_zlib; then
	echo mv 26_easing_animation builds/
	mv 26_easing_animation builds/
	echo mv 26_animated_circle.png ppm/
	mv 26_animated_circle.png ppm/
    echo ==============================================================
fi
echo mv 27_image_to_patterns builds/
mv 27_image_to_patterns builds/
echo mv 27_output.ppm ppm/
mv 27_output.ppm ppm/
echo mv 28_vintage_photo builds/
mv 28_vintage_photo builds/
echo mv 28_0_output_input_original.ppm ppm/
mv 28_0_output_input_original.ppm ppm/
echo mv 28_1_output_addSchmutz.ppm ppm/
mv 28_1_output_addSchmutz.ppm ppm/
echo mv 28_2_output_applyGamma.ppm ppm/
mv 28_2_output_applyGamma.ppm ppm/
echo mv 28_3_output_addNoise.ppm ppm/
mv 28_3_output_addNoise.ppm ppm/
echo mv 28_4_output_applyCircularVignette.ppm ppm/
mv 28_4_output_applyCircularVignette.ppm ppm/
echo mv 28_5_output_applyGamma.ppm ppm/
mv 28_5_output_applyGamma.ppm ppm/
echo mv 28_6_output_applySepia.ppm ppm/
mv 28_6_output_applySepia.ppm ppm/
echo mv test builds/
mv test builds/
echo mv blur2_0.ppm ppm/
mv blur2_0.ppm ppm/
echo mv blur2_1.ppm ppm/
mv blur2_1.ppm ppm/
echo mv blur2_2.ppm ppm/
mv blur2_2.ppm ppm/
echo mv blur2_3.ppm ppm/
mv blur2_3.ppm ppm/
echo mv blur2_4.ppm ppm/
mv blur2_4.ppm ppm/
echo mv blur2_5.ppm ppm/
mv blur2_5.ppm ppm/
echo mv test2_copy.ppm ppm/
mv test2_copy.ppm ppm/
echo mv test2_2.ppm ppm/
mv test2_2.ppm ppm/
echo mv test2_3.ppm ppm/
mv test2_3.ppm ppm/
echo mv test2_4.ppm ppm/
mv test2_4.ppm ppm/
echo mv test2_5.ppm ppm/
mv test2_5.ppm ppm/
echo mv test2_6.ppm ppm/
mv test2_6.ppm ppm/
echo mv test2_7.ppm ppm/
mv test2_7.ppm ppm/
echo mv 28_inputLens.ppm ppm/
mv 28_inputLens.ppm ppm/
echo mv blur2_0Bloom.ppm ppm/
mv blur2_0Bloom.ppm ppm/
echo mv blur2_0downscale.ppm ppm/
mv blur2_0downscale.ppm ppm/
echo mv blur2_0upscale.ppm ppm/
mv blur2_0upscale.ppm ppm/
echo mv test2_2AA.ppm ppm/
mv test2_2AA.ppm ppm/

echo Finished.