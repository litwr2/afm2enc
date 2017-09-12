TTF=${1%%.ttf}
case $# in
1) if [ ! -f $TTF.ttf ]; then echo $1 not found; exit; fi;;
*) echo USAGE: ttfallinst ttf-file; exit;;
esac
ttf2afm $TTF | sed '/; N  ; /d' >$TTF.afm
afmtoenc $TTF -p -d all
TTC=$TTF-all
for i in *-all[0-9]*.enc
do
  k=${i%%.enc}
  afm2tfm $TTF -T $i -v $k
  echo $k '<'$i '<'$TTF.ttf >>$TTC.4map
  vptovf $k
  rm $k.vf $TTF.tfm
done
pdftex $TTC-dump.tex
