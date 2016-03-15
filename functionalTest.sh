#!/bin/sh

RDIR=`pwd`
TEST_DIR="$RDIR/testFiles"
TEST_FILES="test1.txt"
REPORT_DIR="$RDIR/testReports"

# Подотовка директорий

echo "Create report dir $RDIR"

if [ -d $REPORT_DIR ] ; then
    rm -R $REPORT_DIR
fi
mkdir $REPORT_DIR
    
# Сборка задач
make clean
make

# Запуск задач
for i in $TEST_FILES ; do
    echo "Start serial programm for test file $i"
    $RDIR/workSerial $TEST_DIR/$i > $REPORT_DIR/"$i".result.serial
    echo "Start thread programm for test file $i"
    $RDIR/workThreads $TEST_DIR/$i > $REPORT_DIR/"$i".result.thread
    echo "Start mpi programm for test file $i"
    mpirun -np 1 $RDIR/workThreads $TEST_DIR/$i > $REPORT_DIR/"$i".result.mpi
done

# Сравнение результатов
for i in $TEST_FILES ; do
    cat $REPORT_DIR/"$i".result.serial | sort > $REPORT_DIR/"$i".result.serial.tmp
    mv $REPORT_DIR/"$i".result.serial.tmp $REPORT_DIR/"$i".result.serial 
    
    cat $REPORT_DIR/"$i".result.thread | sort > $REPORT_DIR/"$i".result.thread.tmp
    mv $REPORT_DIR/"$i".result.thread.tmp $REPORT_DIR/"$i".result.thread

    cat $REPORT_DIR/"$i".result.mpi | sort > $REPORT_DIR/"$i".result.mpi.tmp
    mv $REPORT_DIR/"$i".result.mpi.tmp $REPORT_DIR/"$i".result.mpi
    
    diff $REPORT_DIR/"$i".result.serial $REPORT_DIR/"$i".result.thread > $REPORT_DIR/"$i".diff.serial.thread
    diff $REPORT_DIR/"$i".result.serial $REPORT_DIR/"$i".result.mpi > $REPORT_DIR/"$i".diff.serial.mpi
done
