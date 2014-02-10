BMP_Compare
===========

This software compares three bmp image that must be existing before starting the program.
Images sould be named :

	-imgbase.bmp
	-imgnew.bmp
	
To start the software, you should type : 
BMP_Compare <tolerance in percent> <quantity of pixel in perthousand>
eventually followed by "debug" without quotes to enable debug output (to test comparison parameters)



linux 	: gcc (Debian 4.6.3-14) 4.6.3
windows : codeblocks-8.02 + mingw