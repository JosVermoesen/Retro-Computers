 name test1.c
 text
 bss
 global _i1
_i1 rmb 2
 global _i2
_i2 rmb 2
 global _i3
_i3 rmb 2
 global _i4
_i4 rmb 2
 global _c1
_c1 rmb 1
 global _c2
_c2 rmb 1
 global _c3
_c3 rmb 1
 global _c4
_c4 rmb 1
 global _p1
_p1 rmb 2
 global _p2
_p2 rmb 2
 global _array
_array rmb 1000
 text
 global _main
_main pshs u,y,x
 leay 4,s
 leas -L2,s
* Auto -6 a
* Auto -8 b
* Auto -10 c
* Auto -11 x
* Auto -12 y
* Auto -13 z
* Auto -15 pc1
* Auto -17 pc2
* Begin expression - 11
 ldd #0
 std -6,y
L3
* Begin expression - 11
 ldd -6,y
 cmpd #1000
 lbge L4
* Begin expression - 12
 ldd -6,y
 ldx #_array
 clr d,x
L5
* Begin expression - 12
 inc -5,y
 lbne L3
 inc -6,y
 lbra L3
L4
* Begin expression - 13
 ldx #_array
 stx -15,y
L6
* Begin expression - 13
 ldx #_array+1000
 cmpx -15,y
 lble L7
* Begin expression - 14
 ldb [-15,y]
 lbeq L9
* Begin expression - 15
 jsr _abort
L9
L8
* Begin expression - 16
 ldx -15,y
 leax 1,x
 stx -15,y
 lbra L6
L7
* Begin expression - 17
 ldb #0
 stb _c2
 stb _c1
* Begin expression - 18
 ldd #0
 std -8,y
L10
* Begin expression - 18
 ldd -8,y
 cmpd #1000
 lbge L11
* Begin expression - 19
 ldd -8,y
 ldx #_array
 leax d,x
 ldb _c1
 inc _c1
 stb 0,x
L12
* Begin expression - 19
 inc -7,y
 lbne L10
 inc -8,y
 lbra L10
L11
* Begin expression - 20
 ldx #_array
 stx -17,y
L13
* Begin expression - 20
 ldx #_array+1000
 cmpx -17,y
 lble L14
* Begin expression - 21
 ldb _c2
 inc _c2
 cmpb [-17,y]
 lbeq L16
* Begin expression - 22
 jsr _abort
L16
L15
* Begin expression - 23
 ldx -17,y
 leax 1,x
 stx -17,y
 lbra L13
L14
* Begin expression - 23
 ldd #100
 std _i3
L2 equ 17
L1
 leas -4,y
 puls x,y,u,pc
 global _abort
_abort pshs u,y,x
 leay 4,s
 leas -L18,s
L18 equ 4
L17
 leas -4,y
 puls x,y,u,pc
 end
