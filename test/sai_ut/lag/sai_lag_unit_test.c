#include <stdio.h>
#include "sai.h"

const char* test_profile_get_value(
    _In_ sai_object_id_t lag_id,
    _In_ const char* variable)
{
    return 0;
}

int test_profile_get_next_value(
    _In_ sai_object_id_t lag_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

int main()
{
    sai_status_t              status;
    sai_lag_api_t             *lag_api;
    sai_object_id_t           lag_oids[2];
    sai_object_id_t           lag_member_oids[4];
    // sai_attribute_t           attrs[2];

    status = sai_api_initialize(0, &test_services);

    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query LAG API, status=%d\n", status);
        return 1;
    }

    // Create LAG#1
    status = lag_api->create_lag(&lag_oids[0], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG#1, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER#1 {LAG_ID:LAG#1, PORT_ID:PORT#1}
    status = lag_api->create_lag_member(&lag_member_oids[0], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#1, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER#2 {LAG_ID:LAG#1, PORT_ID:PORT#2}
    status = lag_api->create_lag_member(&lag_member_oids[1], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#2, status=%d\n", status);
        return 1;
    }

    // Create LAG#2
    status = lag_api->create_lag(&lag_oids[1], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG#2, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER#3 {LAG_ID:LAG#2, PORT_ID:PORT#3}
    status = lag_api->create_lag_member(&lag_member_oids[2], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#3, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER#4 {LAG_ID:LAG#2, PORT_ID:PORT#4}
    status = lag_api->create_lag_member(&lag_member_oids[3], 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#4, status=%d\n", status);
        return 1;
    }

    // Get LAG#1 PORT_LIST [Expected: (PORT#1, PORT#2)]
    status = lag_api->get_lag_attribute(lag_oids[0], 0, NULL); // TODO

    // Get LAG#2 PORT_LIST [Expected: (PORT#3, PORT#4)]
    status = lag_api->get_lag_attribute(lag_oids[1], 0, NULL); // TODO

    // Get LAG_MEMBER#1 LAG_ID [Expected: LAG#1]
    status = lag_api->get_lag_member_attribute(lag_member_oids[0], 0, NULL); // TODO

    // Get LAG_MEMBER#3 PORT_ID [Expected: PORT#3]
    status = lag_api->get_lag_member_attribute(lag_member_oids[2], 0, NULL); // TODO

    // Remove LAG_MEMBER#2
    status = lag_api->remove_lag_member(lag_member_oids[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER#2, status=%d\n", status);
        return 1;
    }

    // Get LAG#1 PORT_LIST [Expected: (PORT#1)]
    status = lag_api->get_lag_attribute(lag_oids[0], 0, NULL); // TODO

    // Remove LAG_MEMBER#3
    status = lag_api->remove_lag_member(lag_member_oids[2]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER#3, status=%d\n", status);
        return 1;
    }

    // Get LAG#2 PORT_LIST [Expected: (PORT#4)]
    status = lag_api->get_lag_attribute(lag_oids[1], 0, NULL); // TODO

    // Remove LAG_MEMBER#1
    status = lag_api->remove_lag_member(lag_member_oids[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER#1, status=%d\n", status);
        return 1;
    }

    // Remove LAG_MEMBER#4
    status = lag_api->remove_lag_member(lag_member_oids[3]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER#4, status=%d\n", status);
        return 1;
    }

    // Remove LAG#2
    status = lag_api->remove_lag(lag_oids[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG#2, status=%d\n", status);
        return 1;
    }

    // Remove LAG#1
    status = lag_api->remove_lag(lag_oids[0]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG#1, status=%d\n", status);
        return 1;
    }

    status = sai_api_uninitialize();

    return 0;
}
