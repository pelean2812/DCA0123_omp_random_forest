#!/bin/bash
#SBATCH --job-name=PaScal_job
#SBATCH --output=PaScal_job%j.out
#SBATCH --error=PaScal_job%j.err
#SBATCH --cpus-per-task=128
#SBATCH --time=0-0:10
#SBATCH --partition=amd-512

pascalanalyzer -t aut -c 1,2,4,8,16,32,64,128 -i 88,175,350,699,1398,2796,5592 -o randomforest.json ./teste2

