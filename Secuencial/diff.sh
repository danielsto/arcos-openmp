#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )

cd "$parent_path"

diff ../Solucion_4096/salida/histogram4096.txt histogram.txt
diff ../Solucion_4096/salida/maxmin4096.txt maxmin.txt
diff ../Solucion_4096/salida/mask_out4096.img mask_out.img
diff ../Solucion_4096/salida/rot_out4096.img rot_out.img
diff ../Solucion_4096/salida/cir_out4096.img circle_out.img