#include "reflect-lib.hpp"

#include <string>
#include <sstream>
#include <memory.h>
#include <stdlib.h>

#define STRNCPY(field, value) \
        strncpy(field, value, sizeof(field))




typedef struct settlement_info
{
    char        bankCode[10];
    char        accountNum[16];
    double      amount;

   MAPPER(
       ("bank_code",               bankCode,   10)
       ("settlement_account_num",  accountNum, 16)
       ("settlement_amt",          amount)
   )
} settlement_info_t;

typedef struct deposit_details {
    char        type[10];
    double      increaseAmount;
    double      decreaseAmount;

    MAPPER(
        ("deposit_requirement_type",    type,   10)
        ("increase_amt",                increaseAmount)
        ("decrease_amt",                decreaseAmount)
    )
} deposit_details_t;

typedef struct withdraw
{
    char                accountNum[16];
    char                currenyCode[5];
    unsigned int        transferAmount;
    char                withDrawReasonCode[5];
    unsigned int        settlementInfoWithdrawCnt;
    settlement_info_t  *settlment_info;
    
    unsigned int        consignmentGuarantee;
    unsigned int        depositRequirementChangeDetailsCnt;
    deposit_details_t  *depositDetails;
 
    char                optrEmailYn[1];
    char                optrEmailAddress[255];
    char                optrSmsYn[1];
    unsigned long       optrMobileNum;

    char                apvrEmailYn[1];
    char                apvrEmailAddress[255];
    char                apvrSmsYn[1];
    unsigned long       apvrMobileNum;


    MAPPER(
        ("account_num",                             accountNum, 16)
        ("currency_code",                           currenyCode, 5)
        ("trans_amt",                               transferAmount)
        ("withdraw_reason_code",                    withDrawReasonCode, 5)
        ("settlement_info_withdraw_cnt",            settlementInfoWithdrawCnt)
        ("settlement_info_withdraw",                settlment_info, settlementInfoWithdrawCnt)
        ("consignment_guarantee",                   consignmentGuarantee)
        ("deposit_requirement_change_details_cnt",  depositRequirementChangeDetailsCnt)
        ("deposit_requirement_change_details",      depositDetails, depositRequirementChangeDetailsCnt)
        ("optr_email_yn",                           optrEmailYn, 1)
        ("optr_email_address",                      optrEmailAddress, 255)
        ("optr_sms_yn",                             optrSmsYn, 1)
        ("optr_mobile_num",                         optrMobileNum)
        ("apvr_email_yn",                           apvrEmailYn, 1)
        ("apvr_email_address",                      apvrEmailAddress, 255)
        ("apvr_sms_yn",                             apvrSmsYn, 1)
        ("apvr_mobile_num",                         apvrMobileNum)
    )
} withdraw_t;

static withdraw_t *
init_withdraw() {
    withdraw_t *withdraw = (withdraw_t *)calloc(1, sizeof(withdraw_t));

    STRNCPY(withdraw->accountNum, "01-123-123123-123");
    STRNCPY(withdraw->currenyCode, "KR");
    withdraw->transferAmount = 1000000;

    STRNCPY(withdraw->withDrawReasonCode, "REASON1");
    withdraw->settlementInfoWithdrawCnt = 3;
    withdraw->settlment_info = (settlement_info_t *)calloc(withdraw->settlementInfoWithdrawCnt, sizeof(settlement_info_t));

    STRNCPY(withdraw->settlment_info[0].bankCode, "KB");
    STRNCPY(withdraw->settlment_info[0].accountNum, "123-123-1234");
    withdraw->settlment_info[0].amount = 500000;

    STRNCPY(withdraw->settlment_info[1].bankCode, "WOORI");
    STRNCPY(withdraw->settlment_info[1].accountNum, "090-0012-0987");
    withdraw->settlment_info[1].amount = 300000;

    STRNCPY(withdraw->settlment_info[2].bankCode, "SH");
    STRNCPY(withdraw->settlment_info[2].accountNum, "0901232-0987");
    withdraw->settlment_info[2].amount = 200000;

    withdraw->consignmentGuarantee = 0;
    withdraw->depositRequirementChangeDetailsCnt = 2;

    withdraw->depositDetails = (deposit_details_t *)calloc(withdraw->depositRequirementChangeDetailsCnt, sizeof(deposit_details_t));

    STRNCPY(withdraw->depositDetails[0].type, "1la");
    withdraw->depositDetails[0].increaseAmount = 100;
    withdraw->depositDetails[0].decreaseAmount = 0;

    STRNCPY(withdraw->depositDetails[1].type, "12v");
    withdraw->depositDetails[1].increaseAmount = 9999;
    withdraw->depositDetails[1].decreaseAmount = 888;

    STRNCPY(withdraw->optrEmailYn, "Y");
    STRNCPY(withdraw->optrEmailAddress, "hong@gildong.com");
    STRNCPY(withdraw->optrSmsYn, "Y");
    withdraw->optrMobileNum = 821011112222;

    STRNCPY(withdraw->apvrEmailYn, "Y");
    STRNCPY(withdraw->apvrEmailAddress, "admin@approver.com");
    STRNCPY(withdraw->apvrSmsYn, "Y");
    withdraw->apvrMobileNum = 821088889999;

    return withdraw;
}

int main()
{
    withdraw_t* withdraw = init_withdraw();
    const char *withdraw_json = convert_to_json(withdraw);

    fprintf(stderr, "\n\n===================  struct -> json =================== \n");
    printf("converted json string ==> \n%s\n<==\n", withdraw_json);


    withdraw_t test2;
    memset(&test2, 0x00, sizeof(test2));

    test2.settlment_info = (settlement_info_t *)calloc(10, sizeof(settlement_info));
    test2.depositDetails = (deposit_details_t *)calloc(10, sizeof(deposit_details_t));

 
    convert_from_json(withdraw_json, test2);

    fprintf(stderr, "%s\n", withdraw->accountNum);
    fprintf(stderr, "%lu\n", withdraw->apvrMobileNum);

    free(test2.settlment_info);
    free(test2.depositDetails);

}
