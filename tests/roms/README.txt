Test ROMs
=========

6502_functional_test.bin and 6502_functional_test.a65 are Klaus Dormann's
6502 functional test (version 05-jan-2020), copied unchanged from
github.com/Klaus2m5/6502_65C02_functional_tests at commit 7954e2d. They are
GPL-3.0 (license.txt) and are test data, not part of the emulator.

The .bin is the stock build of the .a65: load it at $0000, start at $0400.
Every failing check is a jump to itself; the success trap is at $3469. To
see which check sits at a trap address, look it up in
bin_files/6502_functional_test.lst upstream.

  sha256  fa12bfc761e6f9057e4cc01a665a7b800ff01ae91f598af1e39a1201d01953fd
          6502_functional_test.bin
