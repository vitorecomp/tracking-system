#!/bin/sh


#rodando a v sequencial
#sh ./v_sequential/build.sh
#sh ./v_sequential/run.sh


sudo perf stat -d -B --all-cpus --per-core ../v_sequential.2/result ../../database/personaB.avi 635 425 400 400 230
