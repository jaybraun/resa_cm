recc0 > rin_unsorted.txt

sort rin_unsorted.txt | sed 's/ \$ALSP_UNIT_/ /g' |  sed 's/ \$ALSP_SONAR_/ /g' | sed 's/ \$EMITTER_/ /g' | sed 's/ \$WEAP_/ /g' > rin_sorted.txt
rm rin_unsorted.txt

recc1

sort RESAobjects.txt > RESAobjects.csv
sort RESAinteractions.txt | uniq > RESAinteractions.csv
rm *.txt

cat MELobjects.csv RESAobjects.csv | sed 's/\./_/g' | sed 's/-/_/g' | sed 's/\//_/g' | sort | uniq > allobjects.csv1
cat MELinteractions.csv RESAinteractions.csv | sed 's/\./_/g' | sed 's/-/_/g' | sed 's/\//_/g' | sort | uniq > allinteractions.csv1


sed 's/,,,,/, , , ,/g' allobjects.csv1 | sed 's/,,,/, , ,/g' | sed 's/,,/, ,/g' > allobjects.csv
sed 's/,,,,/, , , ,/g' allinteractions.csv1 | sed 's/,,,/, , ,/g' | sed 's/,,/, ,/g' > allinteractions.csv
rm *.csv1

echo '?xml version="1.0" encoding="UTF-8" standalone="no" ?>' > RESA.xml
echo '<enumerations xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="FEDERATE_ENUMERATION_SCHEMA.xsd">' >> RESA.xml

echo '  <header>' >> RESA.xml
echo '    <file>RESA.xml</file>' >> RESA.xml
echo '    <POC>Jay Braun</POC>' >> RESA.xml
echo '    <POC_Email>Jay.E.Braun@jpl.nasa.gov</POC_Email>' >> RESA.xml
echo '    <Purpose>RESA Enumerations</Purpose>' >> RESA.xml
echo "    <Date>`date`</Date>" >> RESA.xml
echo '    <Model>RESA</Model>' >> RESA.xml
echo '    <Schema_Version>5.3</Schema_Version>' >> RESA.xml
echo '  </header>' >> RESA.xml

recc2

echo '</enumerations>' >> RESA.xml

rm RESA*.csv
rm all*.csv
