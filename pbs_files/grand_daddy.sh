#!/bin/bash
J1=$(qsub mmm_20.pbs)
J2=$(qsub -W depend=afterany:$J1 mmm_16.pbs)
J3=$(qsub -W depend=afterany:$J2 mmm_12.pbs)
J4=$(qsub -W depend=afterany:$J3 mmm_8.pbs)
J5=$(qsub -W depend=afterany:$J4 mmm_4.pbs)
qsub -W depend=afterany:$J5 mmm_2.pbs
