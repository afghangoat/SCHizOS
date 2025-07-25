#!/usr/bin/bash

set -e

#print usage
if [$# -le 1]; then
	echo "Usage: generate_isrs.sh <isrc_gen.c> <isrs_gen.inc>"
	exit 1
fi

#vars
ISRS_GEN_C=$1
ISRS_GEN_ASM=$2

ISRS_WITH_ERROR_CODE="8 10 11 12 13 14 17 21" #error codes

#Generate C file
echo "//Autogenerated, uses NASM macro from isr.asm" > $ISRS_GEN_C
echo "#include \"idt.h\"" >> $ISRS_GEN_C
echo "#include \"gdt.h\"" >> $ISRS_GEN_C
echo "" >> $ISRS_GEN_C

for i in $(seq 0 255); do
    echo "void __attribute((cdecl)) i686_ISR${i}();" >> $ISRS_GEN_C
done

echo "" >> $ISRS_GEN_C
echo "void i686_ISR_InitializeGates(){" >> $ISRS_GEN_C

for i in $(seq 0 255); do
    echo "    i686_IDT_SetGate(${i}, i686_ISR${i}, i686_GDT_CODE_SEGMENT, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);" >> $ISRS_GEN_C
done


echo "}" >> $ISRS_GEN_C

#Generate ASM file
echo ";Autogenerated, uses NASM macro from isr.asm" > $ISRS_GEN_ASM

#ISR_NOERROR
for i in $(seq 0 255); do
	if echo "$ISRS_WITH_ERROR_CODE" | grep -q "\b${i}\b"; then
		echo "ISR_ERRORCODE ${i}" >> $ISRS_GEN_ASM
	else
		echo "ISR_NOERROR ${i}" >> $ISRS_GEN_ASM
	fi
done
