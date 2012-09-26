/*
 * File:   init_fini.c
 * Author: willow
 *
 * Created on 26 вер 2012, 1:04:52
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "datafile.h"

/*
 * CUnit Test Suite
 */

const char COMMON_FILENAME[] = "/home/willow/prj/amax/amax-calculations/commons/2012.comm";
const char LOCATION_FILENAME[] = "/home/willow/prj/amax/amax-calculations/archive/2012/UA/d9d95558.dat";

pDatafile common_datafile;
pDatafile location_datafile;

pDatafile make_datafile (enum datafile_type df_type, const char* filename) {
    pDatafile result = 0;
    FILE* fh = fopen (filename, "rb");
    if (!fh)
        return 0;
    char *buffer;
    if (fh != NULL) {
        fseek(fh, 0L, SEEK_END);
        long size = ftell(fh);
        rewind(fh);
        buffer = malloc(size);
        if (buffer != NULL) {
            fread(buffer, size, 1, fh);
            fclose(fh);
            fh = NULL;

            result = datafile_init (df_type, size, buffer);

            free(buffer);
        }
        if (fh != NULL) fclose(fh);
    }
    return result;
}

int init_suite(void) {
    common_datafile = make_datafile (DFT_COMMON, COMMON_FILENAME);
    location_datafile = make_datafile (DFT_LOCATION, LOCATION_FILENAME);
    return 0;
}

int clean_suite(void) {
    datafile_fini (common_datafile);
    datafile_fini (location_datafile);
    return 0;
}

void test_common_header() {
    CU_ASSERT_PTR_NOT_NULL_FATAL(common_datafile);
    pHeaderCommon hdr = common_datafile->hdr_common_;
    CU_ASSERT_PTR_NOT_NULL_FATAL(hdr);
    CU_ASSERT_PTR_NULL(common_datafile->hdr_location_);
    CU_ASSERT_EQUAL(hdr->start_year_, 2012);
    CU_ASSERT_EQUAL(hdr->start_month_, 1);
    CU_ASSERT_EQUAL(hdr->start_day_, 1);
    CU_ASSERT_EQUAL(hdr->day_count_, 366);
    CU_ASSERT_EQUAL(hdr->custom_data_len_, 0);
    CU_ASSERT_EQUAL(hdr->custom_data_[0], 0);
    CU_ASSERT_EQUAL(common_datafile->data_len_, 19792 - 8);
}

void test_location_header() {
    CU_ASSERT_PTR_NOT_NULL_FATAL(location_datafile);
    pHeaderLocation hdr = location_datafile->hdr_location_;
    CU_ASSERT_PTR_NOT_NULL_FATAL(hdr);
    CU_ASSERT_PTR_NULL(location_datafile->hdr_common_);
    CU_ASSERT_EQUAL(hdr->start_year_, 2012);
    CU_ASSERT_EQUAL(hdr->start_month_, 1);
    CU_ASSERT_EQUAL(hdr->start_day_, 1);
    CU_ASSERT_EQUAL(hdr->day_count_, 366);
    CU_ASSERT_EQUAL(hdr->custom_data_len_, 0);
    CU_ASSERT_EQUAL(hdr->custom_data_[0], 0);
    //CU_ASSERT_EQUAL(location_datafile->data_len_, 19792 - 6);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_TestInfo test_array1[] = {
        {"test_common_header", test_common_header},
        {"test_location_header", test_location_header},
        CU_TEST_INFO_NULL,
    };

    CU_SuiteInfo suites[] = {
        { "init_fini", init_suite, clean_suite, test_array1},
        CU_SUITE_INFO_NULL,
    };

    CU_ErrorCode error = CU_register_suites(suites);
    
    if (error != CUE_SUCCESS) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
