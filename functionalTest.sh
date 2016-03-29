#!/bin/bash

RDIR=`pwd`
TEST_DIR="$RDIR/testFiles"
TEST_FILES="test1.txt"
REPORT_DIR="$RDIR/testReports"

# Количество повторений
let COUNTER_VAR=50

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
    $RDIR/workThreads 4 $TEST_DIR/$i > $REPORT_DIR/"$i".result.thread
    echo "Start mpi programm for test file $i"
    mpirun -np 4 $RDIR/workMPI $TEST_DIR/$i > $REPORT_DIR/"$i".result.mpi
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

# Многократный запуск для последующего рассчета СКО и т.д.

TEST_FILE=test1.txt

# Подготовка
find -name *.repeate | xargs rm -f

# Последовательная программа
COUNTER=0

echo "Start serial programm repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    $RDIR/workSerial $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.serial.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""

# Параллельная программа с 1 потоком
COUNTER=0

echo "Start pthreads programm with 1 thread repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    $RDIR/workThreads 1 $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.threads.1.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""

# Параллельная программа с 2 потоками
COUNTER=0

echo "Start pthreads programm with 2 thread repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    $RDIR/workThreads 2 $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.threads.2.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""

# Параллельная программа с 4 потоками
COUNTER=0

echo "Start pthreads programm with 2 thread repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    $RDIR/workThreads 4 $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.threads.4.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""

# MPI с 1 рабочим процессом
COUNTER=0

echo "Start pthreads programm with 2 thread repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    mpirun -np 2 $RDIR/workMPI $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.mpi.1.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""

# MPI с 2 рабочими процессами
COUNTER=0

echo "Start pthreads programm with 2 thread repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    mpirun -np 3 $RDIR/workMPI $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.mpi.2.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""

# MPI с 4 рабочими процессами
COUNTER=0

echo "Start pthreads programm with 2 thread repeating..."
while [ $COUNTER -lt $COUNTER_VAR ] ; do
    mpirun -np 5 $RDIR/workMPI $TEST_DIR/$TEST_FILE | head -n 1 >> $REPORT_DIR/result.mpi.4.repeate
    let COUNTER=COUNTER+1 
done
echo "Done"
echo ""