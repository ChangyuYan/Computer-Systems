/* 
 * CS:APP Data Lab 
 * 
 * <Changyu Yan 304-566-451>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y)
{
    /*
     bitXor returns 1 when the corresponding bits in x and y differ
     from each other.
     So ~(x & y) returns 1 when neither bits are 1
     ~( (~x) & (~y) returns 1 when neither bits are 0
     */
  return (~(x & y)) & (~( (~x) & (~y)));
}

/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x)
{
    /*
     We need to check the leading bit.
     If the leading bit is 1, we negate the bits and add 1 to it.
     If the leadering bit is 0, we do nothing.
     */
    
    int result;
    int leadingDigit;
    
    result = (x >> 31) ^ x;
    // At this point, we either add 1 or not,
    // depending on if the leading digit is 1 or 0
    
    leadingDigit = ( x >> 31 ) & 1; // Either 1 or 0
    
    return result + leadingDigit;
}

/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x)
{
    int t_min;
    t_min = x + 1; // making t_min 1000..00 iff x is MAX
    x = x + t_min; // Making x 1111....1 iff x is MAX
    // ~x will only be 0, iff original x is MAX
    return !( ~x  + !t_min);
}

/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)
{
    // If x <= y, then -x + y >= 0
    // i.e., (~x) + 1 + y >= 0
    // Now, how do we implement >= 0?
    // We check the if the leading digit is 1 or 0
    // if 1, return 0
    // if 0, return 1
    
    int res;
    int checker;
    int xLeading;
    int yLeading;
    
    res =(~x) + 1 + y;
    res = (res >> 31) & 1;

    // But we also need to check overflow
    xLeading = x & (1 << 31);
    yLeading = y & (1 << 31);
    checker = xLeading ^ yLeading; // returns 1, if the are different
    checker = (checker >> 31) & 1;
    
    // If checker is 1, meaning the addition of -x and y HAVE caused overflow
    // Then we can conclude that one of them is positive
    // the other number is negative
    // so we just need to check if x is negative
    
    // otherwise, if checker failed, meaning there is no overflow
    // then checking res is enough
    
    return (!res & !checker) | ((xLeading >> 31) & checker);
}

/* 
 * byteSwap - swaps the nth byte and the mth byte
                           3  2  1  0
 *  Examples: byteSwap(0x 12 34 56 78, 1, 3) =
                       0x 56 34 12 78
 
 *            byteSwap(0x DE AD BE EF, 0, 2) =
                       0x DE EF BE AD
 
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m)
{
    // 1 byte = 8 bits, so moving n byte is moving 8 * n bits
    // and 8 * n == n << 3
    
    int nx;
    int mx;
    int temp;
    int filter;
    n = n << 3;
    m = m << 3;
    nx = (x >> n) & 0xFF; // Extract the bytes
    mx = (x >> m) & 0xFF; // Extract the bytes
    temp = (nx << m) | (mx << n);
    filter = (0xFF << n) | (0xFF << m);
    
    return (x & ~filter) | temp;
}



/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n)
{
    int filter1;
    int filter2;
    int nx;
    
    filter1 = (~0) << (32 + (~n + 1));
    nx = (x & filter1) >> (32 + (~n + 1));
    filter2 = ~((~0) << (n));
    return (x << n) | (nx & filter2);
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x)
{
    int leadingIsZero;
    leadingIsZero = ( x >> 31) ^ (~0); // equals 1, if leading digit is 0
    return !(((x + leadingIsZero) & x) + !x);
}


/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x)
{
    int filter;
    filter = 0xAA | 0xAA << 8 | 0xAA << 16 | 0xAA << 24;
    // If x fits the condition, then x | filter will give all 1's
    return ! (~(x | filter));
}

/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x)
{
    // I found a trick using xor to check parity:
    // https://www.geeksforgeeks.org/compute-parity-number-using-xor-table-look/

    // So we can just keep breaking x into 2 parts
    // And XOR these two parts
    
    x = x ^ (x >> 16);
    x = x ^ (x >> 8);
    x = x ^ (x >> 4);
    x = x ^ (x >> 2);
    x = x ^ (x >> 1);
    
    return x & 1;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x)
{
    int result;
    int first;
    int second;
    int filter1;
    int filter2;
    
    filter1 = 0x11 | (0x11 << 8); // looks like 0001 0001 0001 0001
    filter2 = filter1 | (filter1 << 16);
    // So we check every 4 bit
    result = x & filter2;
    result += (x >> 1) & filter2;
    result += (x >> 2) & filter2;
    result += (x >> 3) & filter2;
    // Not we need to convert result into the real count
    result = ( result + ( result >> 16) ) & ( 0xF | (0xF << 4) | (0xF << 8)| (0xF << 12));
    // We deal with them separately, not together, to avoid dealing
    // with carrying
    first = ( result >> 8 ) & 0xFF;
    second = result & 0xFF;
    first = (first & 0xF ) + ( (first >> 4) & 0xF );
    second = (second & 0xF ) + ( (second >> 4) & 0xF );
    
    return (first & 0x3F) + ( second & 0x3F);
}

/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n)
{
    // Want to make sure n is not 0
    return ((1 & (!!n) ) << 31) >> (~0 + n);
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x)
{
    return 2;
}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x)
{
    return 2; 
}
