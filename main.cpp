#include <cstdlib>
#include <iostream>
#include <string>
#include<openssl/ssl.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
using namespace std; 

//create a Transaction struct as basic node
struct Transaction {
	Transaction* prev;
	int amount;
	string sender;
	string receiver;
	string nonce;
	string hash;
	Transaction(Transaction* prev, int amount, string sender, string receiver) {
		this->prev = prev;
		this->amount = amount;
		this->sender = sender;
		this->receiver = receiver;
		this->nonce = "0";
	}
};
//link the Transction nodes
class BlockChain
{
private:
	Transaction* head;
public:
	BlockChain() {
		this->head = nullptr;
	}
	bool isEmpty();
	void Add(int amount, string sender, string receiver);
	//
	char randomNonce();
	string charToHex(unsigned char ch[], int len);
	//
	int getBalance(string person);
	void printChain();
	void printChain(Transaction* head);
};

//add a new Transaction as head node
void BlockChain::Add(int amount, string sender, string receiver) {
	this->head = new Transaction(this->head, amount, sender, receiver);
	//find a nonce making last digit of h 0
	char ch[3] = { 0 };
	char str[100];
	unsigned char md[32];
	char temp[3] = { 0 };
	sprintf_s(str, "%d", amount);
	strcat_s(str, sender.c_str());
	strcat_s(str, receiver.c_str());
	while (1) {
		srand(time(NULL));
		ch[0] = char(rand() % 26 + 97);
		ch[1] = char(rand() % 26 + 97);
		strcat_s(str, ch);
		SHA256((unsigned char*)str, strlen(str), md);
		sprintf_s(temp, "%02x", md[31]);
		if (temp[1] == '0') {
			break;
		}
	}
	this->head->nonce = ch;
	//caculate hash in this node
	if (this->head->prev == NULL) {
		head->hash = "NULL";
	}
	else {
		unsigned char md[32];
		char str[100];
		sprintf_s(str, "%d", head->prev->amount);
		strcat_s(str, head->prev->sender.c_str());
		strcat_s(str, head->prev->receiver.c_str());
		strcat_s(str, head->prev->hash.c_str());
		strcat_s(str, head->prev->nonce.c_str());
		SHA256((unsigned char*)str, strlen(str), md);
		head->hash = charToHex(md, 32);
	}
}
// get the balance of the input person
int BlockChain::getBalance(string person) {
	Transaction* node;
	node = head;
	//initial 50$
	int balance = 50;
	while (node) {
		if (person == node->receiver) {
			balance =balance +  node->amount;
		}
		if (person == node->sender) {
			balance = balance - node->amount;
		}
		node = node->prev;
	}
	return balance;
}
//traverse the blockchain from the oldest transaction to the newest and print it
void BlockChain::printChain() {
	printChain(this->head);
}
void BlockChain::printChain(Transaction* head) {
	if (head->prev) {
		printChain(head->prev);
	}
		cout << "Amount:" << head->amount << endl;
		cout << "Sender:" << head->sender << endl;
		cout << "Receiver:" << head->receiver << endl;
		cout << "Nonce:" << head->nonce << endl;
		cout << "Hash:" << head->hash << endl;
}

//used in Add function, to convert unsigned char array to a hexdecimal string
string  BlockChain::charToHex(unsigned char ch[], int len) {
	char temp[3] = { 0 };
	char buf[65] = { 0 };
	for (int i = 0; i < len; i++) {
		sprintf_s(temp, "%02x", ch[i]);
		strcat_s(buf, temp);
	}
	return buf;
}

//declare some dependent functions
bool isInt(string str);
/*
*/
bool isInt(string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		int tmp = (int)str[i];
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}


//
void main() {
	BlockChain chain;
	int amount;
	string sender;
	string receiver;
	string option;
	while (1) {
		cout << "Welcome to the transaction-chain application...\n";
		cout << "1 ) Add a transaction to the chain .\n";
		cout << "2 ) Find Balance of a person .\n";
		cout << "3 ) Print the chain .\n";
		cout << "4 ) Exit .\n";
		cout << "Which operation do you want to make? ( 1 , 2 , 3 , 4 ) :" << endl;
		cin >> option;
		if (option == "1") {
			cout << "Integer of the money:";
			//AmountInputCheck();
			string str;
			cin >> str;
			while (!(isInt(str))) {
				cin.clear();
				cin.ignore();
				cout << "Type of the input is illegal, please enter again:";
				cin >> str;
			}
			amount = stoi(str);
			cout << "Sender name:";
			cin >> sender;
			cout << "Receiver name:";
			cin >> receiver;
			chain.Add(amount, sender, receiver);
			cin.clear();
		}
		else if (option == "2") {
			string person;
			cout << "Person name:";
			cin >> person;
			cout << chain.getBalance(person)<<endl;
			cin.clear();
		}
		else if (option == "3") {
			chain.printChain();
		}
		else if (option == "4") {
			break;
		}
		else {
			cout << "Wrong operation!"<<endl;
		}

	}
}


