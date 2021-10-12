#!/bin/bash
# setting time
TIME='0.1'
if [ $# -ge 1 ]; then TIME=$1; fi

# getting input
IFS=''
readarray -t lines
total_pos=0
for i in "${!lines[@]}"; do
	len=$(echo ${lines[$i]} | wc -m)
	j=0
	while [ $j -lt $(($len - 1)) ] ; do
		pos_i[$total_pos]=$i
		pos_j[$total_pos]=$j
		total_pos=$(($total_pos + 1))
		j=$(($j + 1))
	done
done

# randomising sequence
IFS=$'\n'
rand_seq=( $(seq 0 $(($total_pos - 1)) | shuf) )

# initialising colors
tput setab 7
tput clear
for i in $(seq 10 19); do
	color=$(seq 0 20 254 | shuf -r -n 3)
	tput initc $i $color
done

# printing
for pos_id in ${rand_seq[*]}; do
	sleep $TIME
	tput setaf $(seq 10 19 | shuf -n 1)
	tput cup ${pos_i[$pos_id]} ${pos_j[$pos_id]}
	echo ${lines[${pos_i[$pos_id]}]} | tail -c +$((${pos_j[$pos_id]} + 1)) | head -c 1
done

# finishing
tput cup ${#lines[*]} 0
sleep 10
tput reset # reset colors etc.
