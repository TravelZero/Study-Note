#!/bin/bash
echo "please input file name"
read -p "file name:" filename
test -e $filename && echo "$filename exist" || echo "$filename no exist"
echo "please input two string"
read -p "string1:" string1 
read -p "string2:" string2
[ "$string1" == "$string2" ] && echo "string1" == "string2" || echo "string1" != "string2"

if [ "$string1" == "$string2" ]; then
	echo "string1" == "string2"
fi	

if [ "$string1" == "y" ] || [ "$string1" == "Y" ]; then
	echo "input Y"
else
	echo "input n"
fi

if [ "$string1" == "y" ] || [ "$string1" == "Y" ]; then
	echo "input Y"
elif [ "$string1" == "n" ] || [ "$string1" == "N" ]; then
	echo "input n"
else
	echo "cmd err"
fi

case $1 in
	"a")
		echo "parm is:a"
		;;
	"b")
		echo "parm is:b"
		;;
	*)
		echo "can't identify!!"
		;;
esac

function help(){
	echo "this is help cmd"
}

case $1 in
	"-h")
		help
		;;
	*)
		echo "can't identify!!"
		;;
esac

print(){
	echo "param 1:$1"
	echo "param 2:$2"
}

print a b 