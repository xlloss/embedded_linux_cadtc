#!/bin/bash

#pspdftool 'number(x=10pt,y=5pt,start=53,size=10)' ./2-1_Linux_Driver_Debugg.pdf  ./2-1_Linux_Driver_Debugg-p53-pxx.pdf

#pdf merge : pdfunite in-1.pdf in-2.pdf in-n.pdf out.pdf

start_page=$1
in_file=$2
out_file=$3
pspdftool 'number(x=10pt,y=5pt,start='${start_page}',size=10)' ${in_file} ${out_file}
