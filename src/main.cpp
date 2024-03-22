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

#include <string.h>

#include <iostream>
#include <string>

#include "tibasic.h"

#ifdef _WIN32
#include "Shlwapi.h"
#endif

using namespace std;

/// Logs output from the build
void log(LogSeverity severity, const char *out) {
  cout << severityToString(severity) << ": " << out << endl;
}

void stripExtension(const char *in, char *out, size_t len) {
  if (strrchr(in, '.') == NULL) return;
  strncpy(out, in, len);
  *strrchr(out, '.') = 0;
}

int main(int argc, const char *argv[]) {
  // check for valid number of arguments
  if ((argc < 2) || (argv[1] == NULL)) {
    // Inform the user
    log(Error,
        "Usage: tibasic.exe [options] "
        "filename\nOptions:\n\t-d\t\tDecompile\n\t-o filename\tOutput file");
    return 1;
  }

  Compiler compiler;
  Compiler *pCompiler = &compiler;

  string inFile, outFile;

  bool bDecompile = false;

  // Parse arguments
  inFile = argv[argc - 1];  // Last argument is always filename
  for (int i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], "-o") && !outFile.length()) {
      i++;  // Next argument is filename
      // Output filename
      if (i >= argc - 1) {
        log(Error, "-o requires a parameter (output filename).");
        return 1;
      }
      outFile = argv[i];
    } else if (!strcmp(argv[i], "-d"))
      bDecompile = true;
    else {
          log(Error, "Unknown option specified");
      return 1;
    }
  }

  // If no output was given, rename the input with .8xp instead of .tib and
  // use that as the output.
  if (!outFile.length()) {
    char *tmp = new char[inFile.length()];
    stripExtension(inFile.c_str(), tmp, inFile.length());

    outFile = tmp;
    if (bDecompile)
      outFile += ".tib";
    else
      outFile += ".8xp";

    delete[] tmp;
  }

  // Make sure we have tokens to work with!
  initialiseTokens();

  // Compile time!
  if (inFile.length() && outFile.length()) {
    bool res = false;
    if (bDecompile)
      res = pCompiler->decompile(inFile, outFile);
    else
      res = pCompiler->compile(inFile, outFile);

    if (!res) {
      log(Error, "Compilation failed.");
      return 1;
    }
  } else {
    log(Error, "Either an input or output filename was not given.");
    return 1;
  }

  return 0;
}
