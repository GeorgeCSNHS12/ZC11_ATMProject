// Project Title : ATM++
// Members: George William M. Sison
//          Aristotle Y. Alfonso
//          Lanz Louie M. Nieva
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <conio.h>
using namespace std;

#define clear "cls"
#define delay "pause > nul"
const string BANK_ID = "69694242", BORDER = "=============================================", INDENT = "\t\t", OFFSET = "\n\t\t";
const int MAINTAINING_BALANCE = 100;

struct Depositor {
    string aNumber, lName, fName, mName, cNumber, pin;
    int age = 0;
    double balance = 0.0;
};
struct Utility {
    string formatValue(double val) { // Formatting Money Value to 2 Decimal Places
        ostringstream st; st << setprecision(2) << fixed << val;
        string temp = st.str(), fVal = temp.substr(temp.length() - 3, temp.length());
        for(int i = 1, j = temp.length() - 3; j-- > 0; i++) {
            fVal = temp[j] + fVal;
            fVal = ((j != 0 && i % 3 == 0) ? "," : "") + fVal;
        }
        return ("P" + fVal);
    }
    string formatAccNumber(Depositor *client) { // Formatting Client's Account Number
        string str = client->aNumber, temp = "";
        for(int i = 0; i++ < str.length();) {
            temp += str[i - 1];
            temp += i % 4 == 0 && i != 16 ? "-" : "";
        }
        return temp;
    }
    long long formatAccNumber(string aNumber) { // Formatting Account Number into Number
        long long num = 0;
        stringstream st; st << aNumber; st >> num;
        return num;
    }
    string formatName(Depositor *client) { // Formatting Client's Complete Name
        ostringstream st; st << client->lName << ", " << client->fName << " " << client->mName[0] << ".";
        return st.str();
    }
    string formatInformation(Depositor *client) { // Formatting and Printing the Complete Information of the Client
        ostringstream st; st << OFFSET << "Account Number: " << this->formatAccNumber(client) << OFFSET << "Name: " << this->formatName(client) <<
        OFFSET << "Age: " << client->age << OFFSET << "Contact #: " << client->cNumber << OFFSET << "Balance: " << this->formatValue(client->balance);
        return st.str();
    }
    string formatInformation(Depositor client) { // Formatting and Printing the Sign-Up Information of the Client
        ostringstream st; st << OFFSET << "Account Number: " << this->formatAccNumber(&client) << OFFSET << "Name: " << this->formatName(&client) <<
        OFFSET << "Age: " << client.age << OFFSET << "Contact #: " << client.cNumber << OFFSET << "Pin: " << client.pin;
        return st.str();
    }
};
struct AtmMachine {
    Utility &utils;
    Depositor *client;

    double inquiry() { // Simple Inquiry of the Client
        return this->client->balance;
    }
    void withdraw(int money) { // Withdrawal of the Client
        this->client->balance -= money;
    }
    void deposit(double money) { // Deposit of the Client
        this->client->balance += money;
    }
    void deposit(Depositor &receiver, double money) { // Deposit of the Client with Specific Receiver
        withdraw(money);
        receiver.balance += money;
    }
    void changePin(string newPin) { // Change Pin of the Client
        this->client->pin = newPin;
    }
    void loadMobile(int money) { // Mobile Loading of the Client
        this->withdraw(money);
    }
    void verifyTransfer(Depositor &current, string title) { // Printing the Information of the Receiver
        cout << title << this->utils.formatInformation(&current);
    }
    void fundTransfer(Depositor &receiver, double money) { // Transfer of Money of the CLient
        deposit(receiver, money);
        cout << OFFSET << BORDER << OFFSET << "After Transaction:" << OFFSET << BORDER << OFFSET;
        verifyTransfer(receiver, "Receipient:");
        cout << "\n" << OFFSET;
        verifyTransfer(*(this->client), "Sender:");
        cout << OFFSET << BORDER << "\n";
    }
};

// =====================================================================
vector<Depositor> clients;
vector<string> assets;

void getAssets() { // Getting all the Assets Required for the Program to Run
    ifstream fin("assets.txt");
    for(string temp = "", cont = ""; !fin.eof();) {
        getline(fin, temp);
        if(temp[0] == '~' && cont != "") {
            assets.push_back(cont);
            cont = ""; continue;
        }
        cont += (temp + "\n");
    }
}

void initializeAllDepositors() { // Getting all the Information of the Depositors from a TextFile
    system(clear);
    ifstream fin("depositors.txt");
    for(Depositor client; fin >> client.aNumber;) {
        fin.ignore();
        getline(fin, client.lName);
        getline(fin, client.fName);
        getline(fin, client.mName);
        fin >> client.age >> client.cNumber >> client.balance >> client.pin;
        clients.push_back(client);
    }
    fin.close();
}

void saveAllDepositors() { // Saving all the Information of the Depositors to a TextFile
    system(clear);
    ofstream fout("depositors.txt");
    for(int i = 0; i < clients.size(); i++) {
        fout << clients[i].aNumber << "\n" << clients[i].lName << "\n" << clients[i].fName << "\n" << clients[i].mName << "\n" <<
        clients[i].age << "\n" << clients[i].cNumber << "\n" << setprecision(2) << fixed << clients[i].balance << "\n" << clients[i].pin <<
        (i + 1 == clients.size() ? "" : "\n");
    }
    fout.close();
}

// =====================================================================

bool getDepositor(AtmMachine *atm, string aNumber, string pin) { // Querying in Login if the Account Number and Pin Combination Exist!
    for(int h = clients.size() - 1, l = 0, m; h >= l;) {
        m = (h + l) / 2;
        if(clients[m].aNumber == BANK_ID + aNumber && clients[m].pin == pin) {
            atm->client = &clients[m];
            return true;
        } else if(atm->utils.formatAccNumber(BANK_ID + aNumber) > atm->utils.formatAccNumber(clients[m].aNumber))
            l = m + 1;
        else h = m - 1;
    }
    return false;
}

bool checkOtherDepositor(AtmMachine *atm, int *client, string aNumber) { // Checking if the Query of the Client of Another Existing Client is Valid
    for(int h = clients.size() - 1, l = 0, m; h >= l;) {
        m = (h + l) / 2;
        if(clients[m].aNumber == BANK_ID + aNumber) {
            *client = m;
            return true;
        } else if(atm->utils.formatAccNumber(BANK_ID + aNumber) > atm->utils.formatAccNumber(clients[m].aNumber))
            l = m + 1;
        else h = m - 1;
    }
    return false;
}

bool checkOtherDepositor(string num) { // Checking if the Query of the Client of an Existing Mobile Number is Valid
    for(int i = 0; i++ < clients.size();)
        if(clients[i - 1].cNumber == num)
            return true;
    return false;
}

void transactionSegments(AtmMachine *atm, char cmd) { // Segment/Microservices of the Transactions
    if(toupper(cmd) == '1') { // Inquiry of the Client
        system(clear);
        cout << "\n" << assets[0] << OFFSET << BORDER << OFFSET << "      Your Current Balance: " <<
        atm->utils.formatValue(atm->inquiry()) << OFFSET << BORDER << "\n\n";
        system(delay); system(clear);
    } else if(toupper(cmd) == '2') { // Withdrawal of the CLient
        for(int money = 0; true;) {
            if(atm->inquiry() <= 0) {
                cout << OFFSET << BORDER << OFFSET << "You cannot withdraw yet in your account" << OFFSET << "because you have:" << OFFSET << 
                "[" << atm->utils.formatValue(0) << "] Balance!\n\n";
                system(delay); break;
            }
            system(clear);
            cout << "\n" << assets[0] << "\n" << assets[5] << OFFSET << "==> : ", cin >> money;
            if(money == -1) { system(clear); break; }
            if(atm->inquiry() - money < MAINTAINING_BALANCE) {
                cout << OFFSET << BORDER << OFFSET << "Insufficient Balance!\n\n"; system(delay);
                continue;
            } else if((int) money % 100 != 0 || money <= 100) {
                cout << OFFSET << BORDER << OFFSET << "Requested Amount of Money is Not Valid!" << OFFSET << "Amount must be a whole denomination greater than or equal to P100." <<
                OFFSET << "Ex. [P100, P200, P300,...]\n\n"; system(delay);
                continue;
            }
            cout << OFFSET << BORDER << OFFSET << "Are You Sure You Want to Withdraw" << OFFSET << "[" << atm->utils.formatValue(money) << "]" << OFFSET <<
            "From Your Account?(Yes[1]/No[0]):\n" << OFFSET << "==> : ", cmd = getch();
            if(cmd != '1') continue;
            atm->withdraw(money);
            cout << OFFSET << BORDER << OFFSET << "Money has been Withdrawn!\n\n"; system(delay);
            system(clear); break;
        }
    } else if(toupper(cmd) == '3') { // Deposit of the CLient
        for (double money = 0.0; true;) {
            system(clear);
            cout << "\n" << assets[0] << "\n" << assets[6] << OFFSET << "==> : ", cin >> money;
            if(money == -1) { system(clear); break; }
            cout << OFFSET << BORDER << OFFSET << "Are You Sure You Want to Deposit" << OFFSET << "[" << atm->utils.formatValue(money) << "]" << OFFSET <<
            "To Your Account?(Yes[1]/No[0]):\n" << OFFSET << "==> : ", cmd = getch();
            if(cmd != '1') continue;
            atm->deposit(money);
            cout << OFFSET << BORDER << OFFSET << "Money has been Deposited!\n\n"; system(delay);
            system(clear); break;
        }
    } else if(toupper(cmd) == '4') { // Change Pin of the Client
        for(string oldPin, newPin, verifyPin; true;) {
            system(clear);
            cout << "\n" << assets[0] << "\n" << assets[7] << OFFSET << "     Ex: 123456" << OFFSET << "[1]==> : ", cin >> oldPin;
            if(oldPin == "-1") break;
            if(oldPin != atm->client->pin) {
                cout << OFFSET << BORDER << OFFSET << "Old Pin did not Match!\n\n"; system(delay);
                continue;
            }
            cout << INDENT << "[2]==> : ", cin >> newPin;
            if(newPin == "-1") break;
            cout << INDENT << "[3]==> : ", cin >> verifyPin;
            if(newPin.length() != 6) {
                cout << OFFSET << BORDER << OFFSET << "Incorrect Pin Input!\n\n"; system(delay);
                continue;
            } else if(newPin != verifyPin) {
                cout << OFFSET << BORDER << OFFSET << "New Pin did not Match!\n\n"; system(delay);
                continue;
            }
            cout << "\n" << OFFSET << BORDER << OFFSET << "Are You Sure You Want to Change Your" << OFFSET << "Account Pin?(Yes[1]/No[0]):\n" << OFFSET << "==> : ", cmd = getch();
            if(cmd != '1') continue;
            atm->changePin(newPin);
            cout << OFFSET << BORDER << OFFSET << "Account Pin has been Changed!\n\n"; system(delay);
            system(clear); break;
        }
    } else if(toupper(cmd) == '5') { // Mobile Load of the CLient
        for(string cNumber = ""; true;) {
            system(clear);
            cout << "\n" << assets[0] << "\n" << assets[8] << OFFSET << "  Ex: 09127898442" << OFFSET << "==> : ", cin >> cNumber;
            if(cNumber == "-1") { system(clear); break; }
            if(cNumber.length() != 11) {
                cout << OFFSET << BORDER << OFFSET << "Invalid Mobile Number!\n\n"; system(delay);
                continue;
            } else if(!checkOtherDepositor(cNumber)) {
                cout << OFFSET << BORDER << OFFSET << "Mobile Number does not Exist!\n\n"; system(delay);
                continue;
            }
            for(int money = 0; true;) {
                system(clear);
                cout << "\n" << assets[0] << "\n" << assets[10] << OFFSET << "==> : ", cin >> money;
                if(money == -1) { system(clear); break; }
                if(atm->inquiry() - money < MAINTAINING_BALANCE) {
                    cout << OFFSET << BORDER << OFFSET << "Insufficient Balance!\n\n"; system(delay);
                    continue;
                }
                cout << OFFSET << BORDER << OFFSET << "Are You Sure You Want to Transfer" << OFFSET << "[" << atm->utils.formatValue(money) << "]" << OFFSET <<
                "From Your Account?(Yes[1]/No[0]):\n" << OFFSET << "==> : ", cmd = getch();
                if(cmd != '1') continue;
                atm->loadMobile(money);
                cout << OFFSET << BORDER << OFFSET << "Successful Reload!\n\n"; system(delay);
                break;
            }
            system(clear); break;
        }
    } else if(toupper(cmd) == '6') { // Transfer of Money of the CLient
        int receiverIndex;
        for(string aNumber = ""; true;) {
            if(atm->inquiry() <= 0) {
                cout << OFFSET << BORDER << OFFSET << "You cannot transfer money yet from your" << OFFSET << "account because you have:" << OFFSET <<
                "[" << atm->utils.formatValue(0) << "] Balance!\n\n";
                system(delay); break;
            }
            system(clear);
            cout << "\n" << assets[0] << "\n" << assets[9] << OFFSET << "  Ex: 84231100" << OFFSET << "==> : ", cin >> aNumber;
            if(aNumber == "-1") { system(clear); break; }
            if(!checkOtherDepositor(atm, &receiverIndex, aNumber)) {
                cout << OFFSET << BORDER << OFFSET << "Account Number does not Exist!\n\n"; system(delay);
                continue;
            } else if(BANK_ID + aNumber == atm->client->aNumber) {
                cout << OFFSET << BORDER << OFFSET << "You can't Transfer to your own Account!\n\n"; system(delay);
                continue;
            }
            for(double money = 0.0; true;) {
                system(clear);
                cout << "\n" << assets[0] << OFFSET << BORDER << OFFSET << "Transfer Preview:" << OFFSET << BORDER << OFFSET;
                atm->verifyTransfer(clients[receiverIndex], "Receipient:");
                cout << OFFSET << BORDER << "\n\n" << assets[10] << OFFSET << "==> : ", cin >> money;
                if(money == -1) { system(clear); break; }
                if(atm->inquiry() - money < MAINTAINING_BALANCE) {
                    cout << OFFSET << BORDER << OFFSET << "Insufficient Balance!\n\n"; system(delay);
                    continue;
                }
                cout << OFFSET << BORDER << OFFSET << "Are You Sure You Want to Transfer" << OFFSET << "[" << atm->utils.formatValue(money) << "]" << OFFSET <<
                "From Your Account?(Yes[1]/No[0]):\n" << OFFSET << "==> : ", cmd = getch();
                if(cmd != '1') continue;
                system(clear);
                cout << "\n" << assets[0];
                atm->fundTransfer(clients[receiverIndex], money);
                cout << OFFSET << BORDER << OFFSET << "Successful Transfer!\n\n"; system(delay);
                break;
            }
            system(clear); break;
        }
    }
}

void transactionBody(AtmMachine *atm, bool &exist) { // Main Transaction body of the System
    char cmd = '*';
    while(exist) {
        system(clear);
        cout << "\n" << assets[0] << "\n" << assets[4] << OFFSET << "==> : ", cmd = getch();
        if(cmd == '0') exist = false;
        else transactionSegments(atm, cmd);
    }
}

void createDepositor(Utility utils) { // Create Depositor for the System
    string verifyPin;
    for(Depositor client; true;) {
        system(clear);
        cin.ignore();
        cout << "\n" << assets[0] << "\n" << assets[2];
        client.aNumber = clients.empty() ? (BANK_ID + "00000001") : to_string(utils.formatAccNumber(clients[clients.size() - 1].aNumber) + 1); 
        cout << OFFSET << "  Ex: Sison" << OFFSET << "==> : ", getline(cin, client.lName); client.lName[0] = toupper(client.lName[0]);
        if(client.lName == "-1") break;
        cout << OFFSET << "  Ex: George William" << OFFSET << "==> : ", getline(cin, client.fName); client.fName[0] = toupper(client.fName[0]);
        if(client.fName == "-1") break;
        cout << OFFSET << "  Ex: Morales" << OFFSET << "==> : ", getline(cin, client.mName); client.mName[0] = toupper(client.mName[0]);
        if(client.mName == "-1") break;
        cout << OFFSET << "  Ex: 18" << OFFSET << "==> : ", cin >> client.age;
        if(client.age == -1) break;
        if(client.age < 18 || client.age > 110) {
            cout << OFFSET << BORDER << OFFSET << "Invalid Age!\n\n";
            system(delay); continue;
        }
        cout << INDENT << BORDER;
        cout << OFFSET << "  Ex: 09127898442" << OFFSET << "==> : ", cin >> client.cNumber;
        if(client.cNumber == "-1") break;
        if(client.cNumber.length() != 11) {
            cout << OFFSET << BORDER << OFFSET << "Invalid Contact Number!\n\n";
            system(delay); continue;
        } else if(checkOtherDepositor(client.cNumber)) {
            cout << OFFSET << BORDER << OFFSET << "Contact Number duplicate is not Allowed!\n\n";
            system(delay); continue;
        }
        cout << OFFSET << "  Ex: 123456" << OFFSET << "==> : ", cin >> client.pin;
        if(client.pin == "-1") break;
        if(client.pin.length() != 6) {
            cout << OFFSET << BORDER << OFFSET << "Invalid Account Pin!\n\n";
            system(delay); continue;
        }
        cout << INDENT << "==> : ", cin >> verifyPin;
        if(client.pin != verifyPin) {
            cout << OFFSET << BORDER << OFFSET << "Pin did not Match!\n\n";
            system(delay); continue;
        }
        system(clear);
        cout << OFFSET << BORDER << OFFSET << "Sign-Up Preview:" << OFFSET << BORDER << INDENT << utils.formatInformation(client) << OFFSET << BORDER << OFFSET;
        char cmd = '*';
        cout << OFFSET << BORDER << OFFSET << "Are You Sure You Want to Submit Your" << OFFSET << "Information Above for Creating Your Bank" <<
        OFFSET << "Account?(Yes[1]/No[0]):\n" << OFFSET << "==> : ", cin >> cmd;
        if(cmd != '1') continue;
        clients.push_back(client);
        cout << OFFSET << BORDER << OFFSET << "Bank Account Creation is Successful!\n\n";system(delay);
        saveAllDepositors();
        system(clear); break;
    }
}

int main() { // Main.. eh
    cin.tie(NULL);
    getAssets();
    initializeAllDepositors();
    Utility utils; AtmMachine atm = {utils};

    int attempt = 0;
    char ch = '*';
    for(string aNumber, pin; true;) {
        system(clear);
        cout << "\n" << assets[0] << "\n" << assets[1] << OFFSET << "==> : ", ch = getch();
        if(ch == '0') break;
        if(ch == '2') { createDepositor(utils); continue; }
        if(ch != '1') continue;
        //
        if(clients.empty()) {
            cout << OFFSET << BORDER << OFFSET << "There is no existing accounts yet in this" << OFFSET << "Bank.\n\n";
            system(delay); continue;
        }
        bool exist = false;
        for(attempt = 3; attempt > 0 && !exist; attempt--) {
            system(clear);
            cout << "\n" << assets[0] << "\n" << assets[3] << OFFSET << "Attempt/s Left:" << attempt << OFFSET << BORDER << OFFSET <<
            "  Ex: 84231100" << OFFSET << "==> : ", cin >> aNumber;
            if(aNumber == "-1") break;
            cout << OFFSET << "  Ex: 123456" << OFFSET << "==> : ", cin >> pin;
            if(pin == "-1") break;
            exist = getDepositor(&atm, aNumber, pin);
            if(!exist) {
                cout << OFFSET << BORDER << OFFSET << "Bank Account does not exist!";
                if(attempt == 1) cout << OFFSET << "You've exceeded the # of Attempts!\n" << OFFSET << "Rejecting Card....\n\n";
                system(delay);
                continue;
            }
        }
        if(exist) transactionBody(&atm, exist);
        //
        system(clear);
        cout << "\n" << assets[0] << OFFSET << BORDER <<  OFFSET << "Releasing Card....\n\n";
        system(delay);
        saveAllDepositors();
    }
    return 0;
}