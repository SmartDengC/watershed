; Listing generated by Microsoft (R) Optimizing Compiler Version 19.23.28105.4 

include listing.inc

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

PUBLIC	?qInitResources_qt_1@@YAHXZ			; qInitResources_qt_1
PUBLIC	?qCleanupResources_qt_1@@YAHXZ			; qCleanupResources_qt_1
EXTRN	atexit:PROC
EXTRN	__CxxFrameHandler4:PROC
_BSS	SEGMENT
?dummy@?A0x158fa51a@@3Uinitializer@1@A DB 01H DUP (?)	; `anonymous namespace'::dummy
_BSS	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??0initializer@?A0x158fa51a@@QEAA@XZ DD imagerel ??0initializer@?A0x158fa51a@@QEAA@XZ
	DD	imagerel ??0initializer@?A0x158fa51a@@QEAA@XZ+24
	DD	imagerel $unwind$??0initializer@?A0x158fa51a@@QEAA@XZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??1initializer@?A0x158fa51a@@QEAA@XZ DD imagerel ??1initializer@?A0x158fa51a@@QEAA@XZ
	DD	imagerel ??1initializer@?A0x158fa51a@@QEAA@XZ+20
	DD	imagerel $unwind$??1initializer@?A0x158fa51a@@QEAA@XZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??__Edummy@?A0x158fa51a@@YAXXZ DD imagerel ??__Edummy@?A0x158fa51a@@YAXXZ
	DD	imagerel ??__Edummy@?A0x158fa51a@@YAXXZ+33
	DD	imagerel $unwind$??__Edummy@?A0x158fa51a@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??__Fdummy@?A0x158fa51a@@YAXXZ DD imagerel ??__Fdummy@?A0x158fa51a@@YAXXZ
	DD	imagerel ??__Fdummy@?A0x158fa51a@@YAXXZ+21
	DD	imagerel $unwind$??__Fdummy@?A0x158fa51a@@YAXXZ
pdata	ENDS
CRT$XCU	SEGMENT
?dummy$initializer$@?A0x158fa51a@@3P6AXXZEA DQ FLAT:??__Edummy@?A0x158fa51a@@YAXXZ ; `anonymous namespace'::dummy$initializer$
CRT$XCU	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??__Fdummy@?A0x158fa51a@@YAXXZ DD 010401H
	DD	04204H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??__Edummy@?A0x158fa51a@@YAXXZ DD 010401H
	DD	04204H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$ip2state$??1initializer@?A0x158fa51a@@QEAA@XZ DB 02H
	DB	00H
	DB	00H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$cppxdata$??1initializer@?A0x158fa51a@@QEAA@XZ DB 060H
	DD	imagerel $ip2state$??1initializer@?A0x158fa51a@@QEAA@XZ
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??1initializer@?A0x158fa51a@@QEAA@XZ DD 010919H
	DD	04209H
	DD	imagerel __CxxFrameHandler4
	DD	imagerel $cppxdata$??1initializer@?A0x158fa51a@@QEAA@XZ
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??0initializer@?A0x158fa51a@@QEAA@XZ DD 010901H
	DD	04209H
xdata	ENDS
; Function compile flags: /Odtp
;	COMDAT ??__Fdummy@?A0x158fa51a@@YAXXZ
text$yd	SEGMENT
??__Fdummy@?A0x158fa51a@@YAXXZ PROC			; `anonymous namespace'::`dynamic atexit destructor for 'dummy'', COMDAT
	sub	rsp, 40					; 00000028H
	lea	rcx, OFFSET FLAT:?dummy@?A0x158fa51a@@3Uinitializer@1@A ; `anonymous namespace'::dummy
	call	??1initializer@?A0x158fa51a@@QEAA@XZ	; `anonymous namespace'::initializer::~initializer
	add	rsp, 40					; 00000028H
	ret	0
??__Fdummy@?A0x158fa51a@@YAXXZ ENDP			; `anonymous namespace'::`dynamic atexit destructor for 'dummy''
text$yd	ENDS
; Function compile flags: /Odtp
;	COMDAT ??__Edummy@?A0x158fa51a@@YAXXZ
text$di	SEGMENT
??__Edummy@?A0x158fa51a@@YAXXZ PROC			; `anonymous namespace'::`dynamic initializer for 'dummy'', COMDAT
; File E:\Data_Code\qt\qt_opencv\src\qt_1\qt_1\x64\Debug\rcc\qrc_qt_1.cpp
; Line 45
	sub	rsp, 40					; 00000028H
	lea	rcx, OFFSET FLAT:?dummy@?A0x158fa51a@@3Uinitializer@1@A ; `anonymous namespace'::dummy
	call	??0initializer@?A0x158fa51a@@QEAA@XZ	; `anonymous namespace'::initializer::initializer
	lea	rcx, OFFSET FLAT:??__Fdummy@?A0x158fa51a@@YAXXZ ; `anonymous namespace'::`dynamic atexit destructor for 'dummy''
	call	atexit
	add	rsp, 40					; 00000028H
	ret	0
??__Edummy@?A0x158fa51a@@YAXXZ ENDP			; `anonymous namespace'::`dynamic initializer for 'dummy''
text$di	ENDS
; Function compile flags: /Odtp
;	COMDAT ??1initializer@?A0x158fa51a@@QEAA@XZ
_TEXT	SEGMENT
this$ = 48
??1initializer@?A0x158fa51a@@QEAA@XZ PROC		; `anonymous namespace'::initializer::~initializer, COMDAT
; File E:\Data_Code\qt\qt_opencv\src\qt_1\qt_1\x64\Debug\rcc\qrc_qt_1.cpp
; Line 44
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 40					; 00000028H
	call	?qCleanupResources_qt_1@@YAHXZ		; qCleanupResources_qt_1
	npad	1
	add	rsp, 40					; 00000028H
	ret	0
??1initializer@?A0x158fa51a@@QEAA@XZ ENDP		; `anonymous namespace'::initializer::~initializer
_TEXT	ENDS
; Function compile flags: /Odtp
;	COMDAT ??0initializer@?A0x158fa51a@@QEAA@XZ
_TEXT	SEGMENT
this$ = 48
??0initializer@?A0x158fa51a@@QEAA@XZ PROC		; `anonymous namespace'::initializer::initializer, COMDAT
; File E:\Data_Code\qt\qt_opencv\src\qt_1\qt_1\x64\Debug\rcc\qrc_qt_1.cpp
; Line 43
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 40					; 00000028H
	call	?qInitResources_qt_1@@YAHXZ		; qInitResources_qt_1
	mov	rax, QWORD PTR this$[rsp]
	add	rsp, 40					; 00000028H
	ret	0
??0initializer@?A0x158fa51a@@QEAA@XZ ENDP		; `anonymous namespace'::initializer::initializer
_TEXT	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
?qCleanupResources_qt_1@@YAHXZ PROC			; qCleanupResources_qt_1
; File E:\Data_Code\qt\qt_opencv\src\qt_1\qt_1\x64\Debug\rcc\qrc_qt_1.cpp
; Line 38
	mov	eax, 1
; Line 39
	ret	0
?qCleanupResources_qt_1@@YAHXZ ENDP			; qCleanupResources_qt_1
_TEXT	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
?qInitResources_qt_1@@YAHXZ PROC			; qInitResources_qt_1
; File E:\Data_Code\qt\qt_opencv\src\qt_1\qt_1\x64\Debug\rcc\qrc_qt_1.cpp
; Line 32
	mov	eax, 1
; Line 33
	ret	0
?qInitResources_qt_1@@YAHXZ ENDP			; qInitResources_qt_1
_TEXT	ENDS
END
