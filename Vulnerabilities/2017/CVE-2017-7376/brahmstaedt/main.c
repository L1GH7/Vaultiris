#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>

int main(int argc, char **argv) {

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    xmlDocPtr doc; /* the resulting document tree */

    const char* filename = "buffer_overflow.xml";
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filename);
	return -1;
    }
    int res = xmlXIncludeProcess(doc);
    if (res != 1) {
        fprintf(stderr, "Failed to process include %s\n", filename);
        return -1;
    } else {
        printf("Success!\n");
    }
    xmlFreeDoc(doc);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
