#include <stdio.h>
#include "sai.h"

#define LAGS_NUMBER 2
#define LAG_MEMBERS_NUMBER 4
#define PORTS_NUMBER 4

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
    sai_object_id_t           lag_oids[LAGS_NUMBER];
    sai_object_id_t           lag_member_oids[LAG_MEMBERS_NUMBER];
    sai_attribute_t           attrs[2];
    sai_object_id_t           port_list[PORTS_NUMBER];

    status = sai_api_initialize(0, &test_services);

    // Create ports
    for (int32_t ii = 0; ii < PORTS_NUMBER; ii++) {
        status = stub_create_object(SAI_OBJECT_TYPE_PORT, ii, &port_list[ii]);
        if (status != SAI_STATUS_SUCCESS) {
            printf("Failed to create PORT#%d\n", ii);
            return 1;
        }
    }

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
    attrs[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[0].value.oid = port_list[0];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[1].value.oid = lag_oids[0];
    status = lag_api->create_lag_member(&lag_member_oids[0], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#1, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER#2 {LAG_ID:LAG#1, PORT_ID:PORT#2}
    attrs[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[0].value.oid = port_list[1];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[1].value.oid = lag_oids[0];
    status = lag_api->create_lag_member(&lag_member_oids[1], 2, attrs);
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
    attrs[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[0].value.oid = port_list[2];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[1].value.oid = lag_oids[1];
    status = lag_api->create_lag_member(&lag_member_oids[2], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#3, status=%d\n", status);
        return 1;
    }

    // Create LAG_MEMBER#4 {LAG_ID:LAG#2, PORT_ID:PORT#4}
    attrs[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attrs[0].value.oid = port_list[3];
    attrs[1].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attrs[1].value.oid = lag_oids[1];
    status = lag_api->create_lag_member(&lag_member_oids[3], 2, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#4, status=%d\n", status);
        return 1;
    }

    // Get LAG#1 PORT_LIST [Expected: (PORT#1, PORT#2)]
    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = port_list;
    status = lag_api->get_lag_attribute(lag_oids[0], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG#1 PORT_LIST, status=%d\n", status);
        return 1;
    }
    printf("LAG#1 PORT_LIST: ");
    for (int32_t ii = 0; ii < attrs[0].value.objlist.count; ii++) {
        // Could be done via sai_value_to_str() but in this case it's easier to do using for-loop
        printf("0x%lX ", attrs[0].value.objlist.list[ii]);
    }
    printf("\nExpected: 0x%lX 0x%lX\n", port_list[0], port_list[1]);

    // Get LAG#2 PORT_LIST [Expected: (PORT#3, PORT#4)]
    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = port_list;
    status = lag_api->get_lag_attribute(lag_oids[1], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG#1 PORT_LIST, status=%d\n", status);
        return 1;
    }
    printf("LAG#2 PORT_LIST: ");
    for (int32_t ii = 0; ii < attrs[0].value.objlist.count; ii++) {
        printf("0x%lX ", attrs[0].value.objlist.list[ii]);
    }
    printf("\nExpected: 0x%lX 0x%lX\n", port_list[2], port_list[3]);

    // Get LAG_MEMBER#1 LAG_ID [Expected: LAG#1]
    attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    status = lag_api->get_lag_member_attribute(lag_member_oids[0], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG_MEMBER#1 LAG_ID, status=%d\n", status);
        return 1;
    }
    if (lag_oids[0] != attrs[0].value.oid) {
        printf("Test Failed. ER: LAG_MEMBER#1 LAG_ID == LAG#1, AR: 0x%lX != 0x%lX\n", attrs[0].value.oid, lag_oids[0]);
        return 1;
    }
    printf("LAG_MEMBER#1 LAG_ID: 0x%lX\n", attrs[0].value.oid);

    // Get LAG_MEMBER#3 PORT_ID [Expected: PORT#3]
    attrs[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    status = lag_api->get_lag_member_attribute(lag_member_oids[2], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG_MEMBER#3 PORT_ID, status=%d\n", status);
        return 1;
    }
    if (port_list[2] != attrs[0].value.oid) {
        printf("Test Failed. ER: LAG_MEMBER#3 PORT_ID == PORT#3, AR: 0x%lX != 0x%lX\n", attrs[0].value.oid, port_list[2]);
        return 1;
    }
    printf("LAG_MEMBER#3 PORT_ID: 0x%lX\n", attrs[0].value.oid);

    // Remove LAG_MEMBER#2
    status = lag_api->remove_lag_member(lag_member_oids[1]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER#2, status=%d\n", status);
        return 1;
    }

    // Get LAG#1 PORT_LIST [Expected: (PORT#1)]
    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = port_list;
    status = lag_api->get_lag_attribute(lag_oids[0], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG#1 PORT_LIST, status=%d\n", status);
        return 1;
    }
    printf("LAG#1 PORT_LIST: ");
    for (int32_t ii = 0; ii < attrs[0].value.objlist.count; ii++) {
        printf("0x%lX ", attrs[0].value.objlist.list[ii]);
    }
    printf("\nExpected: 0x%lX\n", port_list[0]);

    // Remove LAG_MEMBER#3
    status = lag_api->remove_lag_member(lag_member_oids[2]);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG_MEMBER#3, status=%d\n", status);
        return 1;
    }

    // Get LAG#2 PORT_LIST [Expected: (PORT#4)]
    attrs[0].id = SAI_LAG_ATTR_PORT_LIST;
    attrs[0].value.objlist.list = port_list;
    status = lag_api->get_lag_attribute(lag_oids[1], 1, attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get LAG#1 PORT_LIST, status=%d\n", status);
        return 1;
    }
    printf("LAG#2 PORT_LIST: ");
    for (int32_t ii = 0; ii < attrs[0].value.objlist.count; ii++) {
        printf("0x%lX ", attrs[0].value.objlist.list[ii]);
    }
    printf("\nExpected: 0x%lX\n", port_list[3]);

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
