# ---------- COMPILATIE ----------
all: arraySum ompArraySum

arraySum:
	g++ -O2 -fopenmp -o arraySum arraySum.cc

ompArraySum:
	g++ -O2 -fopenmp -o ompArraySum ompArraySum.cpp

# ---------- DATA BESTANDEN ----------
10k.txt:
	python gen_array.py 10000 10k.txt

100k.txt:
	python gen_array.py 100000 100k.txt

1m.txt:
	python gen_array.py 1000000 1m.txt

10m.txt:
	python gen_array.py 10000000 10m.txt

run_seq: arraySum 10k.txt 100k.txt 1m.txt 10m.txt
	./arraySum 10k.txt
	./arraySum 100k.txt
	./arraySum 1m.txt
	./arraySum 10m.txt

run_omp: ompArraySum 10k.txt 100k.txt 1m.txt 10m.txt
	@echo "== Parallelle versie met verschillende aantallen threads =="

	@echo "10k.txt:"
	set OMP_NUM_THREADS=1 && ompArraySum 10k.txt
	set OMP_NUM_THREADS=2 && ompArraySum 10k.txt
	set OMP_NUM_THREADS=4 && ompArraySum 10k.txt
	set OMP_NUM_THREADS=8 && ompArraySum 10k.txt

	@echo "100k.txt:"
	set OMP_NUM_THREADS=1 && ompArraySum 100k.txt
	set OMP_NUM_THREADS=2 && ompArraySum 100k.txt
	set OMP_NUM_THREADS=4 && ompArraySum 100k.txt
	set OMP_NUM_THREADS=8 && ompArraySum 100k.txt

	@echo "1m.txt:"
	set OMP_NUM_THREADS=1 && ompArraySum 1m.txt
	set OMP_NUM_THREADS=2 && ompArraySum 1m.txt
	set OMP_NUM_THREADS=4 && ompArraySum 1m.txt
	set OMP_NUM_THREADS=8 && ompArraySum 1m.txt

	@echo "10m.txt:"
	set OMP_NUM_THREADS=1 && ompArraySum 10m.txt
	set OMP_NUM_THREADS=2 && ompArraySum 10m.txt
	set OMP_NUM_THREADS=4 && ompArraySum 10m.txt
	set OMP_NUM_THREADS=8 && ompArraySum 10m.txt



