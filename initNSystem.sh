echo "initializing nSystem"
echo ""


cd nsystem64-beta2/src
echo "cleaning nSystem make files"
make clean
echo "recompiling nSystem"
make

cd ../..
echo "exporting NSYSTEM var"
export NSYSTEM="$(pwd)/nsystem64-beta2/"
echo "nSystem exported"

