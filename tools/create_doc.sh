#!/bin/bash

## create_doc.sh
##
## wrapper around 'lv2toweb'.
## read LV2 URIs from stdin, create documentation (incl. screenshot)
## for each of the URIs and create index html page
##
## example usage:
##   lv2ls | grep meter | grep -v _gtk | ./gendoc

################################################################################

### output dir ###
: ${OUTDIR=/tmp/xhtml}

### options ###
# $KEEPOUTDIR # if set do not delete $OUTDIR
# $REPLACEIMAGES # if not set keep existing screenshots

### executables ###
: ${LV2TOWEB=../lv2toweb}
: ${JALVGTK="GTK2_RC_FILES=/home/rgareus/src/git/ardour/build/gtk2_ardour/ardour3_ui_dark.rc LD_LIBRARY_PATH=/home/rgareus/tmp/lv2/lib /home/rgareus/tmp/lv2/bin/jalv.gtk"}
: ${SCROT=scrot}
: ${JNOISE=jnoise}
: ${JACKD=jackd}

################################################################################

function connect_ports {
  PLUGINPORTS=$(
		JACK_DEFAULT_SERVER=lv2screeny jack_lsp -pt \
		| awk 'BEGIN{P=""; I=0; A=0;} /^\t32 bit float mono audio/{A=1} /^\t.*properties:.*input/{I=1} /^[^\t]/{if (P != "" && I==1 && A==1) {print P}; I=0; A=0; if ($0 ~/system/) {P=""} else {P=$0}}' \
		)
	PCNT=0
	SIFS=$IFS
	IFS=$'\n'
	for port in $PLUGINPORTS; do
		if test $PCNT -eq 0; then
			JACK_DEFAULT_SERVER=lv2screeny jack_connect jnoise:white "$port"
		else
			JACK_DEFAULT_SERVER=lv2screeny jack_connect jnoise:pink "$port"
		fi
		PCNT=$(( ($PCNT + 1) % 2 ))
	done
	IFS=$SIFS
}

function genscreenshot {
	URL=$1
	OUT=$2
	if test -f "$OUT" -a -z "$REPLACEIMAGES"; then
		return
	fi
	eval "(JACK_DEFAULT_SERVER=lv2screeny ${JALVGTK} \"$URL\" >/dev/null 2>$JALVERR) &"
	JALVPID=$!
	sleep 3
	connect_ports
	scrot --silent --delay 1 --focused "$OUT"
	disown $JALVPID
	kill -KILL $JALVPID
	grep -qe "UI Type:.*external" $JALVERR || convert "$OUT" -crop +0+27 +repage "$OUT"
}

function gendoc {
	while read uri; do
		FN=$(echo $uri | sed 's/[^a-zA-Z0-9]/_/g')
		genscreenshot "$uri" "$OUTDIR/${FN}.png"
		$LV2TOWEB -i index.html -s "${FN}.png" "$uri" > "$OUTDIR/${FN}.html"
		echo "$uri" >> $INDEXFN
	done
	$LV2TOWEB -t $INDEXFN > "$OUTDIR/index.html"
}

function cleanup {
	disown $JNOISEPID
	kill -TERM $JNOISEPID
	disown $JACKPID
	kill -TERM $JACKPID
	rm $JALVERR
	rm $INDEXFN
}

################################################################################

# launch a dummy jack-server for jalv/screenshots
$JACKD -r -s -n lv2screeny -d dummy &
JACKPID=$!
sleep 2

# jnoise to feed into audio-ports
JACK_DEFAULT_SERVER=lv2screeny $JNOISE &
JNOISEPID=$!

# used by jalv to check for external-ui's
JALVERR=$(mktemp)
INDEXFN=$(mktemp)

# TODO use xnest/xephyr for screenshots

trap cleanup INT QUIT TERM EXIT

test -z "$KEEPOUTDIR" && rm -rf "$OUTDIR"
mkdir -p "$OUTDIR"

gendoc

