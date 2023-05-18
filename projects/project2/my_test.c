#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Loading Document ";
   int data_lines = 8, i;
   char data[20][MAX_STR_SIZE + 1] = {
        "The first course you need to take",
	"is cmsc131.  This course will be",
	"followed by cmsc132 (which is also based on Java).",
	"",
	"The next course you will take is cmsc216.",
	"This course relies on C.",
	"",
	"Ruby and Ocaml will be covered in cmsc330"
   };

   init_document(&doc, doc_name);
   for(i = 0; i< MAX_PARAGRAPHS; i++)
      add_paragraph_after(&doc, i);
   load_document(&doc, data, data_lines);
   print_document(&doc);
   return 0;
}