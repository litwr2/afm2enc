TTF=${1%%.ttf}
TTC=$TTF-$2
case $# in
 2) ttf2afm -u $TTF >$TTF.afm;;
 3) ttf2afm $TTF >$TTF.afm;;
 *) echo USAGE: ttfencinst ttf-file encoding [no Unicode flag]; exit;;
esac
afmtoenc -n $2 $TTF -p || exit
afm2tfm $TTF -T $TTC.enc -v $TTC
echo $TTC '<'$TTC.enc '<'$TTF.ttf >$TTC.4map
vptovf $TTC
rm $TTC.vf $TTF.tfm
pdftex $TTC.tex
