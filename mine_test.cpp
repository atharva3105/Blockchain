#include <iostream>
#include <string>
#include "sha256.h"
using namespace std;

struct block
{

    string hash;
    string prevHash;
    string sender;
    string recevier;
    float amount;
    long int nonce;
    bool proof;
    block *next;
};
string toStr(struct block *b)
{
    string data = "NONCE =" + to_string(b->nonce) + "\n" + "Sender:" + b->sender + "\n" + "Recevier:" + b->recevier + "\n" + "Amount:" + to_string(b->amount) + "\n" + "Previous Hash:" + b->prevHash;
    return data;
}

int main()
{
    struct block b;
    b.amount = 0.123;
    b.hash = "0";
    b.nonce = 1;
    b.prevHash = "0000";
    b.recevier = "rohit";
    b.sender = "atharva";
    string data = sha256(toStr(&b));

    while (data.substr(0, 4) != "0000")
    {
        b.nonce++;
        data = sha256(toStr(&b));
    }
    b.hash = data;
    cout << toStr(&b) << endl;
    cout << "Block Hash:" << b.hash << endl;

    return 0;
}