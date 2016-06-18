all:
	echo "ATM, only 'indent' and 'mrproper' commands are supported"

indent:
	indent -kr --no-tabs *.ino *.cpp *.h

clean: mrproper

mrproper:
	rm *~
