[bits 32]

global x86_rand2
x86_rand2:
	mov eax, 7     ; set EAX to request function 7
	mov ecx, 0     ; set ECX to request subfunction 0
	cpuid
	shr ebx, 18    ; the result we want is in EBX...
	and ebx, 1     ; ...test for the flag of interest... if this is set to 1, the rnd is good

	mov ecx, 100   ; number of retries
.retry:
    rdseed eax
    jc .done      ; carry flag is set on success
    loop .retry
.fail:
    ; no random number available
	;ebx is 0
.done:
    ; random number is in EAX
	;ebx is 1
	mov [edx], ebx  ; store success in the location pointed to by EDX
	mov [edi], eax  ; store random value in the location pointed to by EDI
	ret

;TRY 2
;section .text
;    global x86_rand

; Function: x86_rand
; Description: Generates a 32-bit random number using RDRAND instruction.
; Input: None
; Output: EAX (random number), EDX = 0 (if failure)
; Returns: CF=1 if successful, CF=0 otherwise.
;x86_rand:
;    ; RDRAND attempts to generate random number
;    rdrand eax
;    jc .success   ; If CF=1, success
;    xor eax, eax  ; Zero EAX on failure
;    ret
;.success:
;    ; Random number in EAX
;    ret

global x86_rand
x86_rand:
	xor eax, eax
	xor ecx, ecx
	inc eax
	cpuid
	bt ecx, 30 ; carry flag now indicates support for RDRAND
	;RDRAND reads a random number into a 16-, 32-, or 64-bit register. It might not be successful, however, and sets the carry flag to indicate success. Since no guarantees exist how quickly a new random number might become available, it is necessary to cap the number of retries, such as like this:

	mov ecx, 100
.retry:
	rdrand eax
	jc .success
	loop .retry
.failure:
	  ; handle failed attempt
.success:
	  ret