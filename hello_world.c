/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <nacl_io/nacl_io.h>
#include <nacl_io/ioctl.h>
#include <sys/mount.h>

int main(int argc, char* argv[]) {
  // mount fs
  int result_pers = mount("", "/html5_persistent", "html5fs", 0,
                          "type=PERSISTENT,expected_size=1048576");
  if (result_pers != 0) {
    fprintf(stderr, "persistent html5 filesystem is not available\n");
  }

  // Write 2.4GB file with 'a' chars
  FILE * pFile;
  int mb = 1024 * 1024;
  char* buffer = malloc(mb);
  memset(buffer,'a',mb);
  pFile = fopen("/html5_persistent/file.bin", "wb");
  if (pFile == NULL) {
    printf("failed to open file\n");
    return 1;
  }
  for (int cnt = 0; cnt < 2400; ++cnt) {
    if (fwrite(buffer , sizeof(char), mb, pFile) != mb) {
      printf("TEST FAILED: failed to write\n");
      return 1;
    }
    printf("size %d MB\n", cnt);
  }
  // Write 1MB b chars at pos 0
  memset(buffer,'b',mb);
  if (fseek( pFile , 0 , SEEK_SET ) != 0) {
    printf("TEST FAILED: failed to seek\n");
    return 1;
  }
  if (fwrite(buffer , sizeof(char), mb, pFile) != mb) {
    printf("TEST FAILED: failed to write\n");
    return 1;
  }
  fclose(pFile);

  // Read char at pos 0
  pFile = fopen("/html5_persistent/file.bin", "rb");
  char c = fgetc (pFile);
  printf("Char at pos 0: %c\n", c);
  if (c != 'b') {
    printf("TEST FAILED, read '%c' but expexted 'b' at pos 0\n", c);
  } else {
    printf("TEST SUCCESS\n");
  }
  fclose(pFile);

  return 0;
}
