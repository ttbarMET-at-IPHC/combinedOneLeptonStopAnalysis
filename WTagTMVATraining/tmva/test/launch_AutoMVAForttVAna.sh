#!/bin/bash

SelBox1="Channel!=2&&NBjets>=2&&Mll>=80&&Mll<=100"
SelBox2="(Mll<80||Mll>100)&&(M3>=150&&M3<=200)"
SelBox3="(Mll<80||Mll>100)&&(M3<150||M3>200)"

SelBox1_lMET=${SelBox1}"&&MET<50"
SelBox2_lMET=${SelBox2}"&&MET<50"
SelBox3_lMET=${SelBox3}"&&MET<50"

SelBox1_hMET=${SelBox1}"&&MET>=50"
SelBox3_hMET=${SelBox2}"&&MET>=50"
SelBox4_hMET=${SelBox3}"&&MET>=50"

#root -l AutoMVAForttVAna.C\(string\(\"ifile_box1.txt\"\),""\)
#root -l AutoMVAForttVAna.C\(string\(\"ifile_box1.txt\"\),\"MVAForttVAna_box1.root\",\"${SelBox1}\"\)


root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box1.txt\"\),\"box1\",\"box1_bis.root\",\"${SelBox1}\"\) > box1.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box2.txt\"\),\"box2\",\"box2.root\",\"${SelBox2}\"\) > box2.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box3.txt\"\),\"box3\",\"box3.root\",\"${SelBox2}\"\) > box3.log 2>&1 &

root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box1.txt\"\),\"box1_lMET\",\"box1_lMET.root\",\"${SelBox1_lMET}\"\) > box1_lMET.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box1.txt\"\),\"box1_hMET\",\"box1_hMET.root\",\"${SelBox1_hMET}\"\) > box1_hMET.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box2.txt\"\),\"box2_lMET\",\"box2_lMET.root\",\"${SelBox2_lMET}\"\) > box2_lMET.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box2.txt\"\),\"box2_hMET\",\"box2_hMET.root\",\"${SelBox2_hMET}\"\) > box2_hMET.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box3.txt\"\),\"box3_lMET\",\"box3_lMET.root\",\"${SelBox3_lMET}\"\) > box3_lMET.log 2>&1 &
root -l -q AutoMVAForttVAna.C\(string\(\"ifile_box3.txt\"\),\"box3_hMET\",\"box3_hMET.root\",\"${SelBox3_hMET}\"\) > box3_hMET.log 2>&1 &
