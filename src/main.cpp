/* 
 * Copyright (c) 2011 Matthew Iselin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

// When VC++ compiles in debug mode, it will set _DEBUG. This sets DEBUG
// in order to avoid the VC++-ism.
#ifdef _DEBUG
#define DEBUG
#endif

#include <iostream>
#include <string>

#include <string.h>

#include "tibasic.h"

#ifdef _WIN32
#include "Shlwapi.h"
#endif

using namespace std;

/// Helper function to convert a string to uppercase.
char* strtoupper(char* str)
{
	for( size_t i = 0; i < strlen( str ); i++ )
	{
		if( ! ( isupper( str[i] ) ) && isalpha( str[i] ) )
			str[i] = _toupper( str[i] );
	}
	return str;
}

/// Logs output from the build
void log(LogSeverity severity, const char *out)
{
    cout << severityToString(severity) << ": " << out << endl;
}

void stripExtension(const char *in, char *out, size_t len)
{
    if(strrchr(in, '.') == NULL)
        return;
    strncpy(out, in, len);
    *strrchr(out, '.') = 0;
}

int main( int argc, char* argv[] )
{
	// check for valid number of arguments
	if((argc < 2) || (argv[1] == NULL))
	{
		// Inform the user
        log(Error, "Usage: tibasic.exe [options] filename\nOptions:\n\t-v:\tVerbose mode");
		return 1;
	}

    Compiler compiler; Compiler *pCompiler = &compiler;

    string inFile, outFile;

    bool bDecompile = false;

    // Parse arguments
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "-o") && !outFile.length())
        {
            // Output filename
            if((i + 1) >= argc)
            {
                log(Error, "-o requires a parameter (output filename).");
                return 1;
            }
            outFile = argv[i + 1];
        }
        else if(!strcmp(argv[i], "-v"))
            pCompiler->verbosity(true);
        else if(!strcmp(argv[i], "-d"))
            bDecompile = true;
        else if(!inFile.length())
            inFile = argv[i];
    }

    // If no output was given, rename the input with .8xp instead of .tib and
    // use that as the output.
    if(!outFile.length())
    {
        char *tmp = new char[inFile.length()];
        stripExtension(inFile.c_str(), tmp, inFile.length());

        outFile = tmp;
        if(bDecompile)
            outFile += ".tib";
        else
            outFile += ".8xp";

        delete [] tmp;
    }

    // Make sure we have tokens to work with!
    initialiseTokens();

    // Compile time!
    if(inFile.length() && outFile.length())
    {
        bool res = false;
        if(bDecompile)
            res = pCompiler->decompile(inFile, outFile);
        else
            res = pCompiler->compile(inFile, outFile);

        if(!res)
        {
            log(Error, "Compilation failed.");
            return 1;
        }
    }
    else
    {
        log(Error, "Either an input or output filename was not given.");
        return 1;
    }

    return 0;
}
