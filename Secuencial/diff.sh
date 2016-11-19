#!/bin/bash
echo "Secuencial:"
./ARCfmtut_seq -i ../Solucion_4096/entrada/imagen4096.img -o maxmin.txt -u 1
echo "Paralelo:"
../Paralela/ARCfmtut_par -i ../Solucion_4096/entrada/imagen4096.img -o maxmin.txt -u 1