#!/bin/bash
#
# Build a cross-compiler and toolchain:
#  * binutils
#  * gcc
#  * newlib
#  * gdb
#
# ./src should contain the source code for each of the above.
set -e

declare -r TARGET="arm-none-eabi"
declare -r PROGRAM_PREFIX="${TARGET}-"
declare -r PREFIX="$HOME/local"
declare -r FLAGS="--target=$TARGET --prefix=$PREFIX --program-prefix=$PROGRAM_PREFIX --enable-interwork --enable-multilib --disable-nls --disable-werror"

export CFLAGS="-Wno-unused-value -Wno-shadow"

function build_binutils {
	echo "
#
# Building binutils
#
"
	sleep 1

	cd build
	mkdir -p binutils
	cd binutils
	rm -rf ./*
	configure=`realpath ../../src/binutils*/configure`
	$configure $FLAGS
	make -j 8 all
	make install
	export CFLAGS=""
	cd ../../
}

function build_gcc {
	echo "
#
# Building gcc
#
"
	sleep 1

	cd build
	mkdir -p gcc
	cd gcc
	rm -rf ./*
	configure=`realpath ../../src/gcc*/configure`
	$configure $FLAGS --enable-languages="c,c++" --with-newlib --with-headers=`realpath ../../src/newlib*/newlib/libc/include`
	make -j 8 all-gcc
	make -j 8 all-target-libgcc
	make install-gcc install-target-libgcc
	cd ../../
}

function build_newlib {
	echo "
#
# Building newlib
#
"
	sleep 1

	cd build
	mkdir -p newlib
	cd newlib
	rm -rf ./*

	# newlib expects the compiler to be $TARGET-cc but it doesn't look
	# like installing gcc creates the cc binary. So just create a symlink
	# to gcc.
	if [ ! -f $PREFIX/bin/$TARGET-cc ]; then
		ln -s $PREFIX/bin/$TARGET-gcc $PREFIX/bin/$TARGET-cc
	fi

	configure=`realpath ../../src/newlib*/configure`
	$configure $FLAGS
	make -j 8 all
	make install
	cd ../../
}

function build_gdb {
	echo "
#
# Building gdb
#
"
	sleep 1

	cd build
	mkdir -p gdb
	cd gdb
	rm -rf ./*
	configure=`realpath ../../src/gdb*/configure`
	$configure $FLAGS
	make -j 8 all
	make install
	cd ../../
}

function main {
	build_binutils
	build_gcc
	build_newlib
	build_gdb
}

main
