#!/usr/bin/python
'''
This solution attempted before the C++ solution. This has some cool features, but
is just too slow in practice.
'''

from pymd5 import md5
import os
import random
import re

length = 10
val = 'foofoofoo'
out = 'bazbazbaz'
regex = ".*'\s*\|\|[\s-]*'\s*[1-9]"

letters = [ chr( x ) for x in range( 97, 123 ) ]
letters.extend( range( 10 ) );

# some tests...
print 'None? %s' % re.search( regex, ' "  || 1 # ', re.I | re.S )
print 'None? %s' % re.search( regex, " '  || 0 - ", re.I | re.S )
print 'Match? %s' % re.search( regex, "'||  '4#'!@#$#%^ ", re.I | re.S )
# i don't know why you can have random dashes but it works
print 'Match? %s' % re.search( regex, "jklol'|| - -'4# !@#$#%^ ", re.I | re.S )
print 'Match? %s' % re.search( regex, " '  || ' 989  '!@#$#%^ ", re.I | re.S )
print 'Match? %s' % re.search( regex, "'||'1", re.I | re.S )

count = 1
while count:
    val = ''.join( str( random.choice( letters ) ) for l in range( length ) )
    out = str( md5( val ).digest() )
    if re.search( regex, out, re.I | re.S ):
        print '%d: %s produces md5 string %s' %(count - 1, val, out)
        break
    count += 1
