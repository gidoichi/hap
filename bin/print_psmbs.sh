#!/bin/bash

# チーム数nのpsmbの表を全て作成する．
# @param $1 チーム数

Homedir=$(dirname $0)
nteam=$1
"$Homedir/../tool/permutation_combination.sh" $(($nteam-2)) $(($nteam/2-1)) c |
    sed "s/\./ /g" |
    while read line; do
        echo "($line)"
        "$Homedir/print_psmb" $nteam $line
    done
