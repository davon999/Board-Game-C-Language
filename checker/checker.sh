#!/bin/bash

# Exec name
EXEC_NAME=main

NUM_TESTS=7
POINTS_TEST=(
    10
    10
    10
    20
    20
    30
)

points=0

function compare_files {
	flag_diff=0
    diff -Z -q $1 $2 > /dev/null 2>&1
	if [ $? -eq 0 ];
	then
		flag_diff=1
	else
		flag_diff=0
	fi
}

function add_points {
    if [ "$flag_diff" -eq 1 ]; then
        printf "Test %2d ................................. (+%2dpt)\n" $(($1 + 1)) $((POINTS_TEST[$1]))
        (( points += POINTS_TEST[$1] ))
    else
        printf "Test %2d ................................. (+%2dpt)\n" $(($1 + 1)) $((0))
    fi
}

rm -f "Pas_1/test_1_1.csv"
rm -f "Pas_1/test_1_2.txt"
rm -f "Pas_1/test_1_3.csv"
rm -f "Pas_2/test_2_1.txt"
rm -f "Pas_2/test_2_2.txt"
rm -f "Pas_3/test_3.txt"
rm -f "./Upload_Tema_Moodle"

make clean # Clean
make       # Build
# Run to write the output (directly with Valgrind)
valgrind --leak-check=full \
--show-leak-kinds=all \
--track-origins=yes \
--verbose \
--log-file=valgrind-out.txt \
--error-exitcode=1 \
./main
valgrind_exit_code=$?

compare_files "Pas_1/test_1_1.csv" "Pas_1/ref_test_1_1.csv"
add_points 0

compare_files "Pas_1/test_1_2.txt" "Pas_1/ref_test_1_2.txt"
add_points 1

compare_files "Pas_1/test_1_3.csv" "Pas_1/ref_test_1_3.csv"
add_points 2

compare_files "Pas_2/test_2_1.txt" "Pas_2/ref_test_2_1.txt"
add_points 3

compare_files "Pas_2/test_2_2.txt" "Pas_2/ref_test_2_2.txt"
add_points 4

compare_files "Pas_3/test_3.txt" "Pas_3/ref_test_3.txt"
add_points 5

if [ "$points" -ge 10 ]; then
	if [ "$valgrind_exit_code" -eq 1 ]; then
		echo "S-au detectat scurgeri de memorie, NU veți primi 10 puncte in plus!"
	else
		echo "Nu s-au detectat scurgeri de memorie, veți primi 10 puncte in plus."
        points=$(($points+10))
	fi
fi

# printf "NOTA FINALA ..................................... %3spt\n" $(($points))
# echo

echo -e "Observatie: 10pt vor fi adaugate ulterior pentru documentatie"
echo -e "\nTotal: $points/120pt"
echo

zip -r Upload_Tema_Moodle.zip * >/dev/null && echo "Arhivat cu succes!" || echo "Arhivarea a esuat!"
echo
