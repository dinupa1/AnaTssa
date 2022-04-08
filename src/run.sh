#!/bin/bash

source ../setup.sh

echo

jspi_AN_array=(1.0 0.5 0.2 0.1 0.05)
background_AN_array=(0.5 0.2 0.1 0.05 0.02)
spin_array=(0.0 pi)

jpsi_data="jpsi_data.root"
background_data="drell_yan_data.root"

# create data for signal and background

echo "*** create jpsi & background events ***"

for (( i=0; i<1; i++ ))
do
	for (( j=0; j<2; j++ ))
	do
		jpsi_outfile="jpsi_${i}_${j}.root"
		jpsi_tree="A_N = ${jspi_AN_array[i]} , spin = ${spin_array[j]}"
		jpsi_macro="make_tree.cc(\"$jpsi_data\", \"$jpsi_outfile\", \"$jpsi_tree\", ${spin_array[j]}, ${jspi_AN_array[i]})"
		root -q -b "$jpsi_macro"

		#echo $jpsi_macro

		background_outfile="drell_yan_${i}_${j}.root"
		background_tree="A_N = ${background_AN_array[i]} , spin = ${spin_array[j]}"
		background_macro="make_tree.cc(\"$background_data\", \"$background_outfile\", \"$background_tree\", ${spin_array[j]}, ${background_AN_array[i]})"
		root -q -b "$background_macro"

		#echo $background_macro

	done
done

# create data for training responce matrix

echo "creating data for training response matrix"

root -q -b 'train_tree.cc("drell_yan_data.root", "train_data.root", "data for training response matrix")'

echo "*** done ***"