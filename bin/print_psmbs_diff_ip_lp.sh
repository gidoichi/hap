#!/bin/bash

# 指定したnに対してip<lp=n(n-1)/2となるpsmbを出力
echo -e "teams\t #hap\t b-slots\t obj(IP)\t ojb(LP)"

Homedir=$(cd $(dirname $0)/../ && pwd)
Dir_tmp="$Homedir/tmp"
Dir_tool="$Homedir/tool"

nteams=$1
cnt=0
"$Dir_tool/permutation_combination.sh" $(($nteams-2)) $(($nteams/2-1)) c |
    sed "s/\./ /g" |
    while read line; do
        cnt=$((cnt+1))
        echo -ne "$nteams\t $cnt\t ("
        echo $line | awk '{for(i=1; i<NF+1; ++i){printf "%d,",$i+1}}' | sed 's/,$//'
        echo -ne ")\t "
        echo $nteams >"$Dir_tmp/hap.txt"
        "$Homedir/bin/print_psmb" $nteams $line >>"$Dir_tmp/hap.txt"
        cat "$Dir_tmp/hap.txt" | "$Homedir/bin/hap2ip" >"$Dir_tmp/hap_ip.lp"
        ip=$(cd "$Dir_tmp"; cplex.exe -c "set logfile *" "read hap_ip.lp" "optimize" |
                 grep "Objective =" |
                 rev | cut -d ' ' -f 1 | rev |
                 awk '{print $1+0}')
        if [ $ip -ne $(($nteams*($nteams-1)/2)) ]; then
            cat "$Dir_tmp/hap.txt" | "$Homedir/bin/hap2lp" >"$Dir_tmp/hap_lp.lp"
            lp=$(cd "$Dir_tmp"; cplex.exe -c "set logfile *" "read hap_lp.lp" "optimize" |
                 grep "Objective =" |
                 rev | cut -d ' ' -f 1 | rev |
                 awk '{print $1+0}')
            echo -e "$ip\t $lp"
        else
            echo -e "$ip\t -"
        fi
    done
