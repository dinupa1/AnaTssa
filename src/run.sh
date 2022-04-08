#!/bin/bash

source ../setup.sh

echo

jspi_AN_array=(1.0 0.5 0.2 0.1 0.05)
back_AN_array=(0.5 0.2 0.1 0.05 0.02)
spin_array=(0.0 pi)

jpsi_data="jpsi_data.root"
back_data="drell_yan_data.root"

# create data for signal and background

echo "*** create jpsi & background events ***"

for (( i=0; i<2; i++ ))
do
	for (( j=0; j<2; j++ ))
	do
		jpsi_outfile="jpsi_${i+1}_${j}.root"
		jpsi_tree="A_N = ${jspi_AN_array[i]} , spin = ${spin_array[j]}"
		jpsi_macro="make_tree.cc(\"$jpsi_data\", \"$jpsi_outfile\", \"$jpsi_tree\", ${spin_array[j]}, ${jspi_AN_array[i]})"
		root -q -b "$jpsi_macro"

		# echo $jpsi_macro

		back_outfile="drell_yan_${i+1}_${j}.root"
		back_tree="A_N = ${back_AN_array[i]} , spin = ${spin_array[j]}"
		back_macro="make_tree.cc(\"$back_data\", \"$back_outfile\", \"$back_tree\", ${spin_array[j]}, ${back_AN_array[i]})"
		root -q -b "$back_macro"

		# echo $back_macro

	done
done

create data for training responce matrix

echo "creating data for training response matrix"

root -q -b 'train_tree.cc("drell_yan_data.root", "train_data.root", "data for training response matrix")'

echo "*** done ***"