#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unit.h"

void unitModuleHeading(char* name) {
    fprintf(stdout,"---> Start Testing Module: %s <---\n",name);
}

void unitTestHeading(char* name) {
    fprintf(stdout,"** Testing: %s **\n",name);
}

void unitModuleFooter(char* name) {
    fprintf(stdout,"---> Finished Testing Module: %s <---\n",name);
}

void unitTestFooter(char* name) {
    fprintf(stdout,"** Finished Testing: %s **\n",name);
}

char* unitGreen(char* src) {
    char* dest = NULL;
    size_t dest_size = (strlen(UNIT_GREEN)+strlen(src)+strlen(UNIT_NORMAL)+1) * sizeof(char);
    dest = (char*) malloc(dest_size);
    snprintf(dest,dest_size,"%s%s%s",UNIT_GREEN,src,UNIT_NORMAL);
    return dest;
}

char* unitRed(char* src) {
    char* dest = NULL;
    size_t dest_size = (strlen(UNIT_RED)+strlen(src)+strlen(UNIT_NORMAL)+1) * sizeof(char);
    dest = (char*) malloc(dest_size);
    snprintf(dest,dest_size,"%s%s%s",UNIT_RED,src,UNIT_NORMAL);
    return dest;
}

void unitAssertFail(char* name) {
    char* formattedText = unitRed(UNIT_FAIL);
    fprintf(stdout,"%s: Assert fail %s\n",name,formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertPass(char* name) {
    char* formattedText =  unitGreen(UNIT_PASS);
    fprintf(stdout,"%s: Assert pass %s\n",name,formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertNotZero(char* name, int value) {
    char* formattedText = NULL;
    if (value != 0) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert not zero: %s\n", name, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertZero(char* name, int value) {
    char* formattedText = NULL;
    if (value == 0) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert zero: %s\n",name, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualChar(char* name, char val1, char val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert zero: %s\n",name, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualShort(char* name, short val1, short val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert %d and %d are equal: %s\n",name, val1, val2, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualInt(char* name, int val1, int val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert '%d' and '%d' are equal: %s\n", name, val1, val2, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualLong(char* name, long val1, long val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert '%ld' and '%ld' are equal: %s\n", name, val1, val2, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualFloat(char* name, float val1, float val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert '%f' and '%f' are equal: %s\n", name, val1, val2, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualDouble(char* name, double val1, double val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert '%f' and '%f' are equal: %s\n", name, val1, val2, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualLongDouble(char* name, long double val1, long double val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert '%Lf' and '%Lf' are equal: %s\n", name, val1, val2, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertEqualVoidP(char* name, void* val1, void* val2) {
    char* formattedText = NULL;
    if (val1 == val2) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert values are equal: %s\n", name,  formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertNotNull(char* name, void* val) {
    char* formattedText = NULL;
    if (val != NULL) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert not null: %s\n",name, formattedText);
    free(formattedText);
    formattedText = NULL;
}

void unitAssertNull(char* name, void* val) {
    char* formattedText = NULL;
    if (val == NULL) {
        formattedText = unitGreen(UNIT_PASS);
    } else {
        formattedText = unitRed(UNIT_FAIL);
    }
    fprintf(stdout, "%s: Assert null: %s\n",name, formattedText);
    free(formattedText);
    formattedText = NULL;
}
