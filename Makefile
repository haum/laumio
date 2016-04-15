all:
	echo "ATM, only 'indent' and 'mrproper' commands are supported"

indent:
	indent -kr --no-tabs *.ino

mrproper:
	rm *~
