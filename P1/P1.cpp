#include <ctime>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cctype>
#include "MyLip.h"

using namespace std;
const string FileNameClient = "Client.txt";

void ATMShowMainMenue();
void Login();
void ShowQucikWithdrawScreen();


enum CheckATMMainMenue
{
	QuickWithdraw = 1, NormalWithdraw, Deposit = 3, CheckBalance = 4, Logout = 5
};


struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelet = false;
	
};

sClient CurrentClient;


sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string>vString;
	vString = MyLip::SplitStringToVector(Line, Seperator);
	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);
	return Client;
}

vector<sClient> LoadDataFromFile(string FileName)
{
	vector<sClient> vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string line;
		sClient Client;
		while (getline(MyFile, line))
		{
			Client = ConvertLineToRecord(line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return vClient;
}




string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}
vector<sClient> SaveClientDateToFile(string FileName, vector<sClient> vCleint)
{
	fstream Myfile;
	string DataLine;
	Myfile.open(FileName, ios::out);
	if (Myfile.is_open())
	{
		for (sClient C : vCleint)
		{
			if (C.MarkForDelet == false)
			{
				DataLine = ConvertRecordToLine(C);
				Myfile << DataLine << endl;
			}

		}
		Myfile.close();
	}
	return vCleint;
}



void GoBackToATMMainMenue()
{
	cout << "\n\n\nPress Any Key To Go Back To Main Menue... ";
	system("pause>0");
	system("cls");
	ATMShowMainMenue();

}
short ReadNumber()
{

	short Num = 0;
	cin >> Num;
	return Num;
}



bool DepositeBalanceToClientByAccount(string AccountNumber, double Amount, vector<sClient> vClient)
{
	char Answer = 'n';
	cout << "\n\n Are You Sure Wou Want Perform This Transaction? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientDateToFile(FileNameClient, vClient);
				cout << "\n\nDone Successfully. New Balance is: " << C.AccountBalance;
				return true;
			}
		}
	}
	return false;
}
short ReadQuickWithdrawOption()
{
	short Choice = 0;
	while (Choice < 1 || Choice>9)
	{
		cout << "\nChoose what to do from [1] to [9] ? ";
		cin >> Choice;
	}
	return Choice;
}
short GetQucikWithdrawDrawAmount(short QucikWithdrawDrawOption)
{
	switch (QucikWithdrawDrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 4:
		return 100;
	case 5:
		return 200;
	case 6:
		return 400;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

void PerfromQuickWithdrawOption(short QucikWithdrawDrawOption)
{
	if (QucikWithdrawDrawOption == 9)
		return;

	short WithdrawDrawBalance = GetQucikWithdrawDrawAmount(QucikWithdrawDrawOption);

	if (WithdrawDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make anotherchoice.\n";
			cout << "Press Anykey to continue...";
		system("pause>0");
		ShowQucikWithdrawScreen();
		return;
	}
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	DepositeBalanceToClientByAccount(CurrentClient.AccountNumber, WithdrawDrawBalance * -1, vClient);
	CurrentClient.AccountBalance -= WithdrawDrawBalance;
	
}
void ShowQucikWithdrawScreen()
{
	

	cout << "==================================================\n";
	cout << "\t\tQuck Withdraw\n";
	cout << "==================================================\n";
	cout << "\t[1] 20  \t[2] 50\n";
	cout << "\t[3] 100 \t[4] 200\n";
	cout << "\t[5] 400 \t[6] 600\n";
	cout << "\t[7] 800 \t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "==================================================\n";
	cout << "Your Balance is: " << CurrentClient.AccountBalance;
	
	PerfromQuickWithdrawOption(ReadQuickWithdrawOption());

}


int ReadNormalWithdrawOption()
{
	int Amount = 0;
	cout << "Enter an amount multiplt of 5's ? ";
	cin >> Amount;
	while (Amount % 5 != 0)
	{
		cout << "Enter an amount multiplt of 5's ? ";
		cin >> Amount;
	}
	return Amount;

}
void PerfromNormalWithdrawOption()
{
	int Amount = ReadNormalWithdrawOption();
	
	if (Amount > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make anotherchoice.\n";
		cout << "Press Anykey to continue...";
		system("pause>0");
		ATMShowMainMenue();
		return;
	}
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	DepositeBalanceToClientByAccount(CurrentClient.AccountNumber, Amount * -1, vClient);
	CurrentClient.AccountBalance -= Amount;


}
void ShowNormalWithdrawScreen()
{
	cout << "==================================================\n";
	cout << "\t\tNormal Withdraw\n";
	cout << "==================================================\n";
	PerfromNormalWithdrawOption();
}

double ReadDeposit()
{
	double Amount = 0;
	cout << "Enter a positive Depsit Amount? ";
	cin >> Amount;

	while (Amount <= 0)
	{
		out << "Enter a positive Depsit Amount? ";
		cin >> Amount;
	}

	return Amount;
}
void PerfromDepositOption()
{
	double Amount = ReadDeposit();
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	DepositeBalanceToClientByAccount(CurrentClient.AccountNumber, Amount, vClient);
	CurrentClient.AccountBalance += Amount;
}

void ShowDepositScreen()
{
	cout << "==================================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "==================================================\n";
	PerfromDepositOption();
}

void CheckBalnceScreen()
{
	cout << "==================================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "==================================================\n";
	cout << "Your Balance is: " << CurrentClient.AccountBalance;
}


void CheckATM(CheckATMMainMenue ATMMainMenue)
{
	switch (ATMMainMenue)
	{
	case CheckATMMainMenue::QuickWithdraw:
	{
		system("cls");
		ShowQucikWithdrawScreen();
		GoBackToATMMainMenue();
		break;
	}
	case CheckATMMainMenue::NormalWithdraw:
	{
		system("cls");
		ShowNormalWithdrawScreen();
		GoBackToATMMainMenue();
		break;
	}
	case CheckATMMainMenue::Deposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToATMMainMenue();
		break;
	}
	case CheckATMMainMenue::CheckBalance:
	{
		system("cls");
		CheckBalnceScreen();
		GoBackToATMMainMenue();
		break;
	}
	case CheckATMMainMenue::Logout:
	{
		system("cls");
		Login();
		break;
	}
	}
}


void ATMShowMainMenue()
{
	system("cls");
	cout << "==================================================\n";
	cout << "\t\tATM Mian Menue Screen\n";
	cout << "==================================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "==================================================\n";
	cout << "Choos What Do you Want To Do? [1 to 5]? ";
	CheckATM(CheckATMMainMenue(ReadNumber()));


}


bool FindClientByAccountNumberAndPinCode(string Accountnumber, string PinCode, sClient& Client)
{
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);

	for (sClient C : vClient)
	{
		if (C.AccountNumber == Accountnumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;

	return false;
}

void Login()
{
	bool LoginFild = false;

	string AccountNumber, PinCode;
	do
	{
		system("cls");

		cout << "------------------------------\n";
		cout << "      Login Screen\n";
		cout << "______________________________\n\n";

		if (LoginFild)
		{
			cout << "Invlaid Account Number/PinCode!\n";
		}
		cout << "Enter Account Number? ";
		cin >> AccountNumber;
		cout << "Enter PinCode? ";
		cin >> PinCode;

		LoginFild = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFild);
	ATMShowMainMenue();

}

int main()
{
	Login();

	system("pause>0");
}






//sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
//{
//	sClient Client;
//	vector<string>vString;
//	vString = MyLip::SplitStringToVector(Line, Seperator);
//	Client.AccountNumber = vString[0];
//	Client.PinCode = vString[1];
//	Client.Name = vString[2];
//	Client.Phone = vString[3];
//	Client.AccountBalance = stod(vString[4]);
//	return Client;
//}
//
//
//
//string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
//{
//	string stClientRecord = "";
//	stClientRecord += Client.AccountNumber + Seperator;
//	stClientRecord += Client.PinCode + Seperator;
//	stClientRecord += Client.Name + Seperator;
//	stClientRecord += Client.Phone + Seperator;
//	stClientRecord += to_string(Client.AccountBalance);
//	return stClientRecord;
//}
//
//string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
//{
//	string stUserRescord = "";
//	stUserRescord += User.UserName + Seperator;
//	stUserRescord += User.Password + Seperator;
//	stUserRescord += to_string(User.Permissions);
//	return stUserRescord;
//}
//
//vector<sClient> LoadDataFromFile(string FileName)
//{
//	vector<sClient> vClient;
//	fstream MyFile;
//	MyFile.open(FileName, ios::in);
//	if (MyFile.is_open())
//	{
//		string line;
//		sClient Client;
//		while (getline(MyFile, line))
//		{
//			Client = ConvertLineToRecord(line);
//			vClient.push_back(Client);
//		}
//		MyFile.close();
//	}
//	return vClient;
//}
//
//
//
//
//
//void AddDataLineToFile(string FileName, string DataLine)
//{
//	fstream MyFile;
//	MyFile.open(FileName, ios::out | ios::app);
//	if (MyFile.is_open())
//	{
//		MyFile << DataLine << endl;
//		MyFile.close();
//	}
//}











