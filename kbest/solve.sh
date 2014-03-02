#!/bin/bash
##
## Solve a problem list
##
## Usage:
##   ./solve.sh PROBLEMDIR OUTDIR
##
## Params:
##
##		PROBLEMDIR 		Directory containing .lst files.
##						(files containing where to find problems)
##		OUTDIR 			Directory where to store results of calculation.
##						
##

#
# FUNCTIONS
#

echoerr() { echo "$@" 1>&2; }

control_c() { 
	kill -KILL ${PIDS[@]} 2>/dev/null
	exit $?
}

getCpuNum() {
	CPUS=`grep -c ^processor /proc/cpuinfo`
	echo $CPUS
}

usage() {
  [ "$*" ] && echoerr "$0: $*"
  sed -n '/^##/,/^$/s/^## \{0,1\}//p' "$0" 1>&2
  exit 2
}

solve_problem_list_python() {
	K_BEG=$1
	K_INC=$2
	K_END=$3
	listp="$4"
	outp="$5"
	touch $outp 2>/dev/null
	echo "# nvar, b, k, forward_time, backward_time, total_time" > "$outp"
	for k in `seq $K_BEG $K_INC $K_END`;
	do
	  while read prob;
	  do
	    python kbest.py -p -k "$k" "$prob" >> "$outp"
	  done < "$listp"
	done
}

#
# MAIN
#

if [ $# -ne 2 ]; then
  usage 'Missing Parameters!'
  exit -1
fi

trap control_c SIGINT

MAXTHREAD=$(bc -i < <(echo "$(getCpuNum) / 2") | tail -1)		# 50% CPU usage
INDEX=1
NPROC=0
PIDS=()
K_BEGIN=10
K_INCREMENT=100
K_END=1100

OUT_DIR=$(realpath $2)
PROBLEM_DIR=$(realpath $1)
TOTAL=$(ls -1 "$PROBLEM_DIR" | grep '.lst$' | wc -l)

for plist in $(ls -1 "$PROBLEM_DIR" | grep '.lst$' );
do
	echoerr "INFO: Processing problem list $INDEX/$TOTAL"
	plist_path="$PROBLEM_DIR/$plist"
	out_path="$OUT_DIR/$plist.solved"
	solve_problem_list_python "$K_BEGIN" "$K_INCREMENT" "$K_END" "$plist_path" "$out_path" &
	PIDS+=($!)
	((INDEX++))
	((NPROC++))
	if [ $NPROC -ge $MAXTHREAD ]; then
		wait
		NPROC=0
		PIDS=()
	fi
done

wait
