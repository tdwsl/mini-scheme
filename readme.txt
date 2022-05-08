ABOUT

This is a simple interpreter for (a subset of) scheme, made so I could figure
out building a lisp interpreter. It's probably my 4th or 5th attempt at a
lisp interpreter. I don't really know a lot about lisp, scheme or functional
programming and this interpreter is pretty incomplete.

Here is a current list of (sort of) implemented functions:
display
newline
+
-
/
*
modulo
vector
vector-ref
string-ref
string=?
=
>
<
>=
<=
define
let
set!
begin
if
read
exit
length

COMPILING

To compile, just use compile.sh, or take a look at it if you aren't using
gcc. This program just uses the c standard library, so all you really need
is a c compiler.
