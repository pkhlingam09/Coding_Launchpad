
#define LOGIN           "loginFile.dat"
#define ENTRY           5
#define MAIN            6
#define ACDIV           100000000

struct loginData {
    unsigned long long int accNo;
    unsigned char lpwd[32];
    unsigned char tpwd[32];
    char fileName[32];
};
typedef struct loginData loginData;
struct homeData {
    unsigned char first_name[32];
    unsigned char last_name[32];
    unsigned short int age;
    long int balance;
};
typedef struct homeData homeData;
struct userInfo {
    unsigned short int day;
    unsigned short int month;
    unsigned short int year;
    unsigned char gender;
    unsigned char address[32];
    unsigned char PAN[16];
    unsigned char aadhar[16];
    unsigned char email[32];
    unsigned long long int phone[10];
    char bank[8];
    char IFSC[8];
};
typedef struct userInfo userInfo;
struct TransferDetails {
        unsigned long long int frmAcc;
        unsigned long long int toAcc;
        char bank[8];
        char IFSC[8];
        unsigned long int amount;
        short int day;
        short int month;
        short int year;
};
typedef struct TransferDetails Transfer;

int createAccount(void);
unsigned long long int loginPage(void);
int transferAmount(unsigned long long int accNo);
long int showBalance(unsigned long long int accNo);
void defaultPage(unsigned long long int accNo);
void *dynamicMemory(size_t sz);
void flushBuffer(void);
