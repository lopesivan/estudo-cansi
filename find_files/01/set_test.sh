[ $1 ] && {
	_d=$1
} || {
	echo 1>&2 Sintaxe: $0 in out
	exit 1
}

test -d $_d && rm -rf $_d

mkdir -p $_d/bb/ccc/ddddd/eeeee/uuuu/zzzz

eval \>$_d/bb/ccc/ddddd/eeeee/uuuu/zzzz/a{1..3}.txt\;

eval \>$_d/bb/ccc/ddddd/eeeee/b{1..6}.txt\;

find $_d

exit 0
