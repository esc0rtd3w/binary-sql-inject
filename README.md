# SQL Injection on Binary MD5 Hash
## The Goal
Perform an injection attach on a webiste using a binary MD5 hash to sanitize
form input. Because the PHP is hashing to raw binary rather than a string
hexadecimal representation, PHP can interpret those bytes as any character, even
dangerous ones. To exploit this, we need to find out what input provides the
binary md5 hash that represents ASCII characters needed to perform an injection.

So, the solution tries random MD5 hashes until one is found that produces binary
that, when interpreted as standard ASCII, will successfully perform an injection
attack.

The major challenge is finding an injection short enough to find in a reasonable
time. Shortest injection I can conjur is:
'||'1

This was changed from '||1# after hint from Will Blair. See below.

I ultimately produced the injection string with the C++ code. I included my
original attempt in Python for completeness and because I think the Python
implementation has come cool features, it's just too slow.

## SQL Choice Details
The "||" is advantagous because it doesn't require a trailing space, but "or"
does. This doesn't seem to be documented, but is true in practice. The "1" can
be anything non-nil (not 0).

The "#" is specific to MySql comments. It's extra beautiful because not only is
is already a char shorter than "-", it doesn't require a trailing space either.
Again, not documented that I see but obvious in practice.

There's lots of flexibility that can be added as well, in the sense that these
characters don't necessarily need to start the string or be without spaces.

## Time Complexity
2^40 bits / 4e6 guesses per second / 3600 sec per hour = 76 hours.
Allowing all digits 1-9 gives a 9x speed increase. Not counting flexibility added
by spaces / random characters as allowed.

UPDATE EDIT: Will Blair had a good hint.
Any string that starts with a number will be treated as a number. Same number of
characters, more flexibility.
'||'1

## Running
Compile with:
g++ inject.cpp -lssl -lcrypto -O3 -o inject

C++ solution was successful in 10 min running 16 instances concurrently. It did
50-100 times more checks per second than the Python implementation.

Found input string:
9fcef3897afe2acc3e7438ce14f5b6a3