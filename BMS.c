/********************************************
 * ENTRY PAGE FOR BANKING MANAGEMENT SYSTEM
 * ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "coreData.h"

int main(void) {
        short int state = ENTRY, choice;
	unsigned long long int accNo = 0;

        while(1) {
                switch(state) {
                        case ENTRY:             // ENTRY MENU
                                printf("1.Create Account\n");
                                printf("2.Login\n");
                                printf("Choose an Option: ");
                                scanf("%hd", &choice);

                                if(choice == 1) {
                                        createAccount();
                                } else if(choice == 2) {
                                        accNo = loginPage();
					if(accNo) {
						state = MAIN;
						defaultPage(accNo);
					}
				}
				break;
                        case MAIN:              // MAIN MENU
                                printf("1.Show Balance\n");
                                printf("2.Amount Transfer\n");
                                printf("3.LogOut\n");
                                printf("Choose an Option: ");
                                scanf("%hd", &choice);

				if(choice == 1) {
					        printf("\n\t--------------------------------------------------------------------------\n");
        					printf("\t| \t\t\tAvailable Balance is %ld\t\t\t |\n", showBalance(accNo));
					        printf("\t--------------------------------------------------------------------------\n\n");

					printf("Available Balance is %ld\n", showBalance(accNo));
				} else if(choice == 2) {
					transferAmount(accNo);
					defaultPage(accNo);
				} else if(choice == 3) {
					printf("Exiting the Program\n");
					state = 0;
				}
				break;
			}
			if(state == 0) {
				break;
			}
	}
	return 0;
}
/*****************************
 * PROG TO CREATE NEW ACC
 * REQUEST DETAILS
 * CREATE ACC-NO
 * CHECK IF ACC-NO IS UNIQUE
 * WRITE TO LOGIN FILE
 * WRITE TO ACC FILE
 * **************************/
int createAccount(void) {
	loginData login;
	homeData  homePg = {.balance = 10000};
	userInfo  user;
	FILE *fptr;
	time_t t;
	long long int accNo = 0;
	char accFile[12], choice;

	printf("To create an account Enter 'y' else enter 'n': ");
	scanf("%*c%c", &choice);
	if(choice == 'n') {
		printf("Returning to main menu\n");
		return 0;
	}
	/*****************
	 * CREATE ACC-NO
	 * USING RAND() 
	 * 8 DIGIT ACC-NO
	 * **************/
	if((fptr = fopen(LOGIN, "ab+")) == NULL) {
		printf("Unable to create file\n");
		return 0;
	}
	/****************************
	 * create accno
	 * check if unique
	 * repeat until achieved
	 * assign to new user
	 * **************************/
	while(1) {
		srand((unsigned)time(&t));
		accNo = rand() % ACDIV;
		while(fread(&login, sizeof(login), 1, fptr) == 1) {
			if(login.accNo == accNo) {
				accNo = 0;
				break;
			}
		}
		if(accNo) {
			printf("%lld\n", accNo);
			break;
		}
		//fseek(fptr, 0L, SEEK_SET);
		rewind(fptr);
	}
	fclose(fptr);
	login.accNo = accNo;
	/**************************
	 * COLLECT USER DATA
	 * ***********************/
	printf("Enter First Name: ");
	fscanf(stdin, "%32s", homePg.first_name);flushBuffer();
	printf("Enter Last Name: ");
	fscanf(stdin, "%32s", homePg.last_name);flushBuffer();
	printf("Enter Date of Birth(dd-mm-yyyy): ");
	scanf("%hd%*c%hd%*c%hd", &user.day, &user.month, &user.year);flushBuffer();
	printf("Enter Age: ");
	scanf("%2hd", &homePg.age);flushBuffer();
	printf("Enter your Gender: ");
	fscanf(stdin, "%c", &user.gender);flushBuffer();
	printf("Enter your Address: ");
	fscanf(stdin, "%32s", user.address);flushBuffer();
	printf("Enter Aadhar number: ");
	fscanf(stdin, "%16s", user.aadhar);flushBuffer();
	printf("Enter PAN number: ");
	fscanf(stdin, "%16s", user.PAN);flushBuffer();
	printf("Enter your e-mail address: ");
	fscanf(stdin, "%32s", user.email);flushBuffer();
	printf("Enter your mobile number: ");
	scanf("%10lld", user.phone);flushBuffer();
	printf("Enter your Password: ");
	fscanf(stdin, "%32s", login.lpwd);flushBuffer();
	printf("Enter Transaction password: ");
	fscanf(stdin, "%32s", login.tpwd);flushBuffer();
	/***************************
	 * CREATE FILE NAME
	 * *************************/
	sprintf(accFile, "%lld", login.accNo);
	strcat(accFile, ".dat");

	if((fptr = fopen(accFile, "wb")) == NULL) {
                printf("Unable to create file\n");
                return 0;
        }
    	fwrite(&homePg, sizeof(homePg), 1, fptr);
	fwrite(&user, sizeof(user), 1, fptr);
	fclose(fptr);
	/***************************
	 * Write data to files
	 * ************************/
	if((fptr = fopen(LOGIN, "ab+")) == NULL) {
                printf("Unable to open file\n");
                return 0;
        }
	fseek(fptr, 0L, SEEK_END);
	strcpy(login.fileName, accFile);
	fwrite(&login, sizeof(login), 1, fptr);
	fclose(fptr);
	return 0;
}
/*****************************************
 * PROGRAM TO LOG INTO BANK ACCOUNT
 * ***************************************/
unsigned long long int loginPage(void) {
	FILE *fptr;
	int count = 3, loginPage = 0;
	unsigned long long int accNo = 0;
	unsigned char lpwd[32];
	loginData login;

	if((fptr = fopen(LOGIN, "rb")) == NULL) {
		printf("Unable to open file\n");
		return 0;
	}
	while(count > 0) {
		printf("Account Number: ");
		scanf("%lld", &accNo);
		printf("Password: ");
		scanf("%s", lpwd);
		while(fread(&login, sizeof(login), 1, fptr) == 1) {
			if(login.accNo == accNo) {
				if(!strcmp(login.lpwd, lpwd)) {
					printf("Login Password\n");
					return accNo;
				}
			}
		}
		printf("Wrong Credentials...Try Again\n");
		rewind(fptr);
		count--;
	}
	fclose(fptr);
	printf("Bank account does not exist\n");

	return 0;
}

long int showBalance(unsigned long long int accNo) {
	FILE *fptr;
	char *fileName;
	loginData login;
	homeData hmDt;

	if((fptr = fopen(LOGIN, "rb")) == NULL) {
		printf("Unable to Open file for reading\n");
		return 0;
	}
	while(fread(&login, sizeof(login), 1, fptr) == 1) {
		if(login.accNo == accNo) {
			fileName = login.fileName;
			break;
		}
	}
	fclose(fptr);
	if((fptr = fopen(fileName, "rb")) == NULL) {
                printf("Unable to Open file for reading\n");
                return 0;
        }
	fread(&hmDt, sizeof(hmDt), 1, fptr);
	fclose(fptr);

	return hmDt.balance;
}

/***********************************************
 * PROGRAM TO TRANSFER MONEY FROM BANK ACCOUNT
 * AUTO-POPULATE REQUIRED DATA
 * REQUEST USER FOR DATA
 * LOCATE ACCOUNT
 * REQUEST TRANSACTION PWD
 * *********************************************/
int transferAmount(unsigned long long int accNo) {
        FILE *fptr, *wfptr;
        loginData login;
        homeData hmDt;
	userInfo userInfo;
    Transfer trdt;
        int count = 3;
    char tpwd[32] = {'\0'}, ch;
        time_t t;
        char *fileName = (char *)dynamicMemory(32);
        // memcopy() fileName pointer

    struct tm *dtTm = localtime(&t);
        trdt.day   = dtTm->tm_mday;
        trdt.month = dtTm->tm_mon;
        trdt.year  = dtTm->tm_year;
        printf("Account to transfer the money: ");
        scanf("%lld", &trdt.toAcc);

        fptr = fopen(LOGIN, "rb");
        //  Find if toAcc exists and save its fileName
        while(fread(&login, sizeof(login), 1, fptr) == 1) {
                if(trdt.toAcc == login.accNo) {
                        strcpy(fileName, login.fileName);
                        break;
                }
        }
        // Check if fileName exists and add remaining details
        if((wfptr = fopen(fileName, "rb")) == NULL) {
                printf("Name of the Bank: ");
                scanf("%s", trdt.bank);
                printf("Enter IFSC Code: ");
                scanf("%s", trdt.IFSC);
        } else {        
		fseek(wfptr, sizeof(hmDt), SEEK_CUR);
		fread(&userInfo, sizeof(userInfo), 1, wfptr);
                strcpy(trdt.bank, userInfo.bank);
                strcpy(trdt.IFSC, userInfo.IFSC);
        }
        fclose(wfptr);
        /* Find if amount to be transferred is 
           less than the balance in from account */
        rewind(fptr);
        while(fread(&login, sizeof(login), 1, fptr) == 1) {
                if(accNo == login.accNo) {
			break;
                }
        }	
	//  Open file related to from account
        if((wfptr = fopen(login.fileName, "rb+")) == NULL) {
		printf("Unable to open from Acc file\n");
		return 0;
	}
        fread(&hmDt, sizeof(hmDt), 1, wfptr);
        while(1) {
                printf("Amount to be transferred: ");
                scanf("%ld", &trdt.amount);
                if(hmDt.balance < trdt.amount) {
                        printf("Low Balance\n");
                        printf("Available balance --> %ld\n", hmDt.balance);
                }
                else {
                        break;
                }
                printf("\n Do you wish to proceed: ");
                scanf("%*c%c", &ch);
                if(ch == 'n') {
                                printf("Transfer aborted\n");
                                return 0;
                }
        }

        /************************************
         * Authenticate Transaction Password
         * **********************************/
        while(1) {
                if(count <= 0) {
                        printf("Maxium number of tries exceeded\n");
                        return 0;
                }
                printf("Enter Transaction Password: ");
                scanf("%s", tpwd);
                if(!strcmp(login.tpwd, tpwd)) {
                        printf("Transaction Successful\n");
                        break;
                } else {
                        printf("Wrong Credentials---Try again\n");
                        count--;
                }
        }
        fclose(fptr);
	/***************************
         * Update Balance in both
         * toAcc and fromAcc
         * *************************/
        // From Acc Balance Update
        hmDt.balance -= trdt.amount;
        fseek(wfptr, -(long)sizeof(hmDt), SEEK_CUR);
        fwrite(&hmDt, sizeof(hmDt), 1, wfptr);
        fclose(wfptr);
        // To Acc Balance Update
        if((wfptr = fopen(fileName, "rb+")) == NULL) {
		printf("Unable to Open file\n");
        	free(fileName);
		return 0;
	}
	fread(&hmDt, sizeof(hmDt), 1, wfptr);
	fseek(wfptr, -(long)sizeof(hmDt), SEEK_CUR);
	hmDt.balance += trdt.amount;
	fwrite(&hmDt, sizeof(hmDt), 1, wfptr);
        fclose(wfptr);

        free(fileName);
        return 0;
}

void defaultPage(unsigned long long int accNo) {
	FILE *fptr;
	loginData login;
	homeData hmData;
	
	if((fptr = fopen(LOGIN, "rb")) == NULL) {
		printf("Unable to Openi Login file\n");
		return;
	}
	while(fread(&login, sizeof(login), 1, fptr) == 1) {
		if(login.accNo == accNo) {
			break;
		}
	}
	fclose(fptr);
	if((fptr = fopen(login.fileName, "rb")) == NULL) {
		printf("Unable to Open the file\n");
		return;
	}
	fread(&hmData, sizeof(hmData), 1, fptr);
	fclose(fptr);
	printf("\n\t----------------------------------------------------------------------------------\n");
	printf("\t| \t\t\t\t\t%lld\t\t\t\t |\n", accNo);
	printf("\t----------------------------------------------------------------------------------\n");
	printf("\t| \t\t%s\t | \t\t%s\t\t | \t%hd\t\t |\n", hmData.first_name, hmData.last_name, hmData.age);
	printf("\t----------------------------------------------------------------------------------\n");
	printf("\t| \t\t\t | \t\t%ld\t\t | \t\t\t |\n", hmData.balance);
	printf("\t----------------------------------------------------------------------------------\n\n");
}

void *dynamicMemory(size_t sz) {
	return calloc(1, sz);
}

void flushBuffer(void) {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

