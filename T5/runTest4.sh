echo "initializing nSystem"
echo ""

cd ..
cd nsystem64-beta2/src
echo "cleaning nSystem make files"
make clean
echo "recompiling nSystem"
make

cd ../..

cd T5
echo "cleaning test4"
make cleanall
make test-t4
echo "cleaning test5"
make test-t5
echo ""
echo ""
echo "runing test"
echo ""
echo ""
echo ""
./test-t4
echo ""
echo ""
echo "runing test"
echo ""
echo ""
echo ""
./test-t5
