uVM
===

A simple virtual machine written in C with a C# compiler

Notes:
--------- Opcodes ---------
PUSH = 0x0
RET = 0x1
ADD = 0x2
LCALL = 0x3
SUB = 0x4
MUL = 0x5
DIV = 0x6
IJMP = 0x7
ALLOC = 0x8
RARG = 0x9
RLOC = 0xA
MKARR = 0xB
SETELEM = 0xC
GETELEM = 0xD
SETVAR = 0xE
GETVAR = 0xF
---------------------------

--------- Operand ---------
Every operand shall be 64 bits in size, unless it's a DT_LONG or DT_ULONG.
High 32 bits of the operand shall be an encoded "type container" and low 32 bits shall be the value of operand.
If operand is of type DT_LONG or DT_ULONG the value of the operand shall be 0, and the 64 bits following should be the value.
High 16 bits of type container shall be the operand's main type and low 16 bits shall be the sub type (DT_POINTER, DT_ARRAY)
If mainType and subType is not equal it must be an array initializer or a global memory allocation operand.
---------------------------

---------- Arrays ---------
The MKARR instruction allocates memory for the array in global memory block and pushes a pointer to it's base to stack.
The MKARR instruction pops a value off the stack as size for array upper boundary.
The MKARR instruction operand shall be a 32 bit value with high 16 bits for array element type and low 16 bits (subType) must be DT_ARRAY.
The SETELEM instruction requires 3 items on stack; DT_INTEGER elementIndex, (element type) value, DT_ARRAY arrayPointer.
Type of value and array element type must be the same.
---------------------------

---------- Return ---------
If function return type is DT_VOID there is no return value in current FunctionContext otherwise value on top of stack is popped and stored in return value in current FunctionContext.
---------------------------

-------- Arithmetic -------
Arithmetic on DT_STRING is not allowed even though it primitively is a DT_INTEGER.
There are no built-in overflow checks, correct type is critical.
---------------------------

-------- Parameters -------
All parameters are initialized and set in callee FunctionContext before instructions are executed.
Parameters are index based and type sensitive.
---------------------------

----- Function Stack ------
Every entry on stack is 64 bits in size and contains a value and a TypeContainer.
If a value of size >32 bits is pushed onto stack it's value is allocated in global memory block and it's pointer is pushed instead of value. 
When a value of size >32 bits is pushed onto stack it's subType is automatically changed to DT_POINTER.
If a entry with subtype DT_POINTER is popped from stack it is automatically dereferenced unless explicitly popped as a pointer.
A standard primitive type (non-array, non-pointer) pushed on stack must have the same mainType and subType.
---------------------------

-------- Variables --------
SETVAR and GETVAR operands shall be 32 bits in size and of type DT_INTEGER and contain the index for the variable requested.
---------------------------