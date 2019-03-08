#!/bin/sh

######################################################################
#
# PRINT_HAPS_DIFF_IP_LP_SAT_ALPHA_SH : 条件alphaを満たすHAPに対し，
#                                      z_IPとz_LPを計算して出力
#
# Written by Shinichi Yanagido (s173875v@st.go.tuat.ac.jp) on 2018-11-27
#
######################################################################


######################################################################
# Initial Configuration
######################################################################

# === Initialize shell environment ===================================
set -u
umask 0022
export LC_ALL=C
type command >/dev/null 2>&1 && type getconf >/dev/null 2>&1 &&
    export PATH="$(command -p getconf PATH)${PATH+:}${PATH-}"
export UNIX_STD=2003  # to make HP-UX conform to POSIX

# === Define the functions for printing usage and exiting ============
print_usage_and_exit () {
    cat <<-USAGE 1>&2
	Usage   : ${0##*/}
	USAGE
    exit 1
}
exit_trap() {
    set -- ${1:-} $?  # $? is set as $1 if no argument given
    trap '-' EXIT HUP INT QUIT PIPE ALRM TERM
    [ -d "${Tmp:-}" ] && rm -rf "${Tmp%/*}/_${Tmp##*/_}"
    exit $1
}
error_exit() {
    ${2+:} false && echo "${0##*/}: $2" 1>&2
    exit $1
}

# === Detect home directory of this app. and define more =============
Homedir="$(d=${0%/*}/; [ "_$d" = "_$0/" ] && d='./'; cd "$d.."; pwd)"
PATH="$Homedir/bin:$Homedir/tool:$PATH"       # for additional command
. "$Homedir/conf/COMMON.SHLIB" # read common configuration


######################################################################
# Argument Parsing
######################################################################

# === Print usage and exit if one of the help options is set =========
case "$# ${1:-}" in
    '1 -h'|'1 --help'|'1 --version') print_usage_and_exit;;
esac

# === Initialize parameters ==========================================
nteams=''
cnt=0
ip=''
lp=''

# === Validate argument ==============================================
nteams=$1


######################################################################
# Main Routine
######################################################################

# === ヘッダの出力 ===================================================
echo "teams	#hap	hap	obj(IP)	ojb(LP)"

# === prepare a temporary directory ==================================
trap 'exit_trap' EXIT HUP INT QUIT PIPE ALRM TERM
Tmp=$(mktemp -d -t "_${0##*/}.$$.XXXXXXXXXXX") || error_exit 1 'Failed to mktemp'

# === 条件alphaを満たし列の入れ替えをしたHAPに対してIP・LPを解く
while read line; do

    # --- HAPの番号を記録
    cnt=$((cnt+1))
    echo -n "$nteams	$cnt	($line)	"

    # --- IPを解く
    echo "$nteams $line" | hap2ip >"$Tmp/hap_ip.lp";
    ip=$(cd "$Tmp"; cplex.exe -c "set logfile *" "read hap_ip.lp" "optimize" |
             grep "Objective =" |
             rev | cut -d ' ' -f 1 | rev |
             awk '{print $nteams+0}')

    # --- LPを解く
    if [ $ip -ne $(($nteams*($nteams-1)/2)) ]; then
        # --- z_IPがn(n-1)/2を達成していなければ，z_LPも計算
        echo "$nteams $line" | hap2lp >"$Tmp/hap_lp.lp"
        lp=$(cd "$Tmp"; cplex.exe -c "set logfile *" "read hap_lp.lp" "optimize" |
                 grep "Objective =" |
                 rev | cut -d ' ' -f 1 | rev |
                 awk '{print $nteams+0}')
        echo "$ip	$lp"
        if [ $ip -ne $lp ]; then
            # --- z_IP!=z_LP となるものは標準エラーに出力
            echo "$cnt $nteams $line" >&2
        fi

    else
        # --- z_IP=n(n-1)/2ならz_IP=z_LPとして出力
        echo "$ip	$ip"
    fi
done


######################################################################
# Finish
######################################################################

exit_trap 0
