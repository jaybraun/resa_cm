$PARSE_TOOLS/pgkeywd.run < orders.syn > orders.tmp
sort orders.tmp > orders.tmp2
uniq orders.tmp2 > orders.tmp
cp orders.tmp orders.key
$PARSE_TOOLS/pgexpnd.run < orders.syn > orders.txt
$PARSE_TOOLS/pgtable.run orders > orders.jnk
rm orders.key
rm orders.txt
rm orders.jnk
rm orders.tm*
