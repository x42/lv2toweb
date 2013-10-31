#!/bin/bash

OUTDIR=/tmp/xhtml/
REPLACEIMAGES=

LV2TOWEB=../lv2toweb
JALVGTK="GTK2_RC_FILES=/home/rgareus/src/git/ardour/build/gtk2_ardour/ardour3_ui_dark.rc jalv.gtk"
SCROT=scrot

################################################################################

function genscreenshot {
  URL=$1
  OUT=$2
	if test -f "$OUT" -a -z "$REPLACEIMAGES"; then
		return
	fi
	eval "(JACK_DEFAULT_SERVER=lv2screeny ${JALVGTK} \"$URL\" >/dev/null) &"
	JALVPID=$!
	scrot --silent --delay 3 --focused "$OUT"
	disown $JALVPID
	kill -KILL $JALVPID
	convert "$OUT" -crop +0+27 +repage "$OUT"
}

function gendoc {
 URIS=""
 while read url; do
	 FN=$(echo $url | sed 's/[^a-zA-Z0-9]/_/g')
	 genscreenshot "$url" "$OUTDIR/${FN}.png"
	 $LV2TOWEB -i index.html -s "${FN}.png" "$url" > "$OUTDIR/${FN}.html"
	 URIS="$URIS $url" # TODO split on long lines
 done
 $LV2TOWEB -t $URIS > "$OUTDIR/index.html"
}

################################################################################

# launch a dummy jack-server for jalv/screenshots
jackd -r -s -n lv2screeny -d dummy &
JACKPID=$!
sleep 2

# TODO use xnest/xephyr for screenshots

mkdir -p "$OUTDIR"
lv2ls | grep gareus | grep midifilter | gendoc
# gendoc

# terminate jackd
disown $JACKPID
kill -TERM $JACKPID
