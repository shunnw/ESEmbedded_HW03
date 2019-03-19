HW03
===
This is the hw03 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw03 dir, use:

	* `make` to build.

	* `make clean` to clean the ouput files.

4. Extract `gnu-mcu-eclipse-qemu.zip` into hw03 dir. Under the path of hw03, start emulation with `make qemu`.

	See [Lecture 02 ─ Emulation with QEMU] for more details.

5. The sample is a minimal program for ARM Cortex-M4 devices, which enters `while(1);` after reset. Use gdb to get more details.

	See [ESEmbedded_HW02_Example] for knowing how to do the observation and how to use markdown for taking notes.

# Build Your Own Program

1. Edit main.c.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C Standard library functions like `printf`, `malloc`, etc.

# HW03 Requirements

1. How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

2. Modify main.c to observe what you found.

3. You have to state how you designed the observation (code), and how you performed it.

	Just like how you did in HW02.

3. If there are any official data that define the rules, you can also use them as references.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

[Lecture 02 ─ Emulation with QEMU]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#Emulation-with-QEMU
[ESEmbedded_HW02_Example]: https://github.com/vwxyzjimmy/ESEmbedded_HW02_Example

--------------------

- [ ] **If you volunteer to give the presentation next week, check this.**

--------------------

**★★★ Please take your note here ★★★**

# 1.實驗題目
+ 改寫`main.c`，觀察C語言的函式如何傳遞與回傳參數。

# 2.實驗步驟
+ 在`main.c`中設計兩個函式：`addtwo()`輸入兩個變數時資料如何傳遞，而`countfive()`輸入五個變數，而在主程式`reset_handler()`呼叫以上兩個函式作數值計算，並回傳計算結果至主程式。
+ 根據 Procedure Call Standard for the ARM Architecture(AAPCS)


### main.c
```c
int addtwo(int a, int b)
{
	return a+b;
}

int countfive(int a, int b, int c, int d, int e)
{
	return a+(b*c)+d-e;
}

void reset_handler(void)
{
	addtwo(1,2);
	countfive(1,2,3,4,5);
	while (1)
		;
}
```

### Result from objdump
```
Disassembly of section .mytext:

00000000 <addtwo-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	00000055 	andeq	r0, r0, r5, asr r0

00000008 <addtwo>:
   8:	b480      	push	{r7}			//將r7放入堆疊記憶體中，執行完畢後放回r7，避免影響主程式
   a:	b083      	sub	sp, #12			//sp往下推出12個位置來放入變數
   c:	af00      	add	r7, sp, #0		//r7=sp
   e:	6078      	str	r0, [r7, #4]		//將主程式傳入的r0放入r7+4的記憶體位置
  10:	6039      	str	r1, [r7, #0]		//將主程式傳入的r1放入r7的記憶體位置
  12:	687a      	ldr	r2, [r7, #4]		//從r7+4的記憶體位置取出至r2暫存器
  14:	683b      	ldr	r3, [r7, #0]		//從r7的記憶體位置取出至r3暫存器
  16:	4413      	add	r3, r2			//
  18:	4618      	mov	r0, r3
  1a:	370c      	adds	r7, #12			//sp往上推12個位置回到原位
  1c:	46bd      	mov	sp, r7
  1e:	f85d 7b04 	ldr.w	r7, [sp], #4
  22:	4770      	bx	lr

00000024 <countfive>:
  24:	b480      	push	{r7}			//將r7放入堆疊記憶體中，執行完畢後放回r7，避免影響主程式
  26:	b085      	sub	sp, #20			//sp往下推出20個位置來放入變數
  28:	af00      	add	r7, sp, #0		//r7=sp
  2a:	60f8      	str	r0, [r7, #12]
  2c:	60b9      	str	r1, [r7, #8]
  2e:	607a      	str	r2, [r7, #4]
  30:	603b      	str	r3, [r7, #0]
  32:	68bb      	ldr	r3, [r7, #8]
  34:	687a      	ldr	r2, [r7, #4]
  36:	fb02 f203 	mul.w	r2, r2, r3
  3a:	68fb      	ldr	r3, [r7, #12]
  3c:	441a      	add	r2, r3
  3e:	683b      	ldr	r3, [r7, #0]
  40:	441a      	add	r2, r3
  42:	69bb      	ldr	r3, [r7, #24]
  44:	1ad3      	subs	r3, r2, r3
  46:	4618      	mov	r0, r3
  48:	3714      	adds	r7, #20
  4a:	46bd      	mov	sp, r7
  4c:	f85d 7b04 	ldr.w	r7, [sp], #4
  50:	4770      	bx	lr
  52:	bf00      	nop

00000054 <reset_handler>:
  54:	b580      	push	{r7, lr}		//將r7,lr放入堆疊記憶體中
  56:	b082      	sub	sp, #8			//sp往下推出8個位置來放入變數
  58:	af02      	add	r7, sp, #8		//r7=sp+8
  5a:	2001      	movs	r0, #1			//將1放入r0
  5c:	2102      	movs	r1, #2 			//將2放入r1
  5e:	f7ff ffd3 	bl	8 <addtwo>		//執行副函式addtwo()
  62:	2305      	movs	r3, #5			//將5放入r3
  64:	9300      	str	r3, [sp, #0]		//將r3放入sp記憶體位置
  66:	2001      	movs	r0, #1			//將1放入r0
  68:	2102      	movs	r1, #2			//將2放入r1
  6a:	2203      	movs	r2, #3			//將3放入r2
  6c:	2304      	movs	r3, #4			//將4放入r3
  6e:	f7ff ffd9 	bl	24 <countfive>		//執行副函式countfive()
  72:	e7fe      	b.n	72 <reset_handler+0x1e>

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <reset_handler+0x10d0cd0>
   4:	35312820 	ldrcc	r2, [r1, #-2080]!	; 0xfffff7e0
   8:	392e343a 	stmdbcc	lr!, {r1, r3, r4, r5, sl, ip, sp}
   c:	732b332e 			; <UNDEFINED> instruction: 0x732b332e
  10:	33326e76 	teqcc	r2, #1888	; 0x760
  14:	37373131 			; <UNDEFINED> instruction: 0x37373131
  18:	2029312d 	eorcs	r3, r9, sp, lsr #2
  1c:	2e392e34 	mrccs	14, 1, r2, cr9, cr4, {1}
  20:	30322033 	eorscc	r2, r2, r3, lsr r0
  24:	35303531 	ldrcc	r3, [r0, #-1329]!	; 0xfffffacf
  28:	28203932 	stmdacs	r0!, {r1, r4, r5, r8, fp, ip, sp}
  2c:	72657270 	rsbvc	r7, r5, #112, 4
  30:	61656c65 	cmnvs	r5, r5, ror #24
  34:	00296573 	eoreq	r6, r9, r3, ror r5

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00003041 	andeq	r3, r0, r1, asr #32
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	00000026 	andeq	r0, r0, r6, lsr #32
  10:	726f4305 	rsbvc	r4, pc, #335544320	; 0x14000000
  14:	2d786574 	cfldr64cs	mvdx6, [r8, #-464]!	; 0xfffffe30
  18:	0600344d 	streq	r3, [r0], -sp, asr #8
  1c:	094d070d 	stmdbeq	sp, {r0, r2, r3, r8, r9, sl}^
  20:	14041202 	strne	r1, [r4], #-514	; 0xfffffdfe
  24:	17011501 	strne	r1, [r1, -r1, lsl #10]
  28:	1a011803 	bne	4603c <reset_handler+0x45fe8>
  2c:	22061e01 	andcs	r1, r6, #1, 28
  30:	Address 0x0000000000000030 is out of bounds.
```

# 3.結果與討論
1.參數會存在r0-r3暫存器中，超過四個變數則由sp進行資料傳輸
2.
3.
