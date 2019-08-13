#!/bin/bash
rm -rf ./building
mkdir -p ./building/files
cd ./building

function mapper(){
	for e in `ls $1`
	do
		f=$1"/"$e
		if [ -f $f ]
		then
			cp -af $f ./$e
			f=$1"/files/"$e"/_body.hpp"
			if [ -f $f ]
				mkdir ./files/$e
				cc -E $f -D $* -o ./files/$e/_body.hpp
			fi
		fi
	done
}
mapper ../forinclude/elc

cd ..
rm -rf ./forinclude ./building/_template
mv ./building ./forinclude
