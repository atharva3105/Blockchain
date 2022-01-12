#include <iostream>
#include <string>
#include <fstream>
#include "sha256.h"
using namespace std;

class block
{
public:
    string hash;
    string prevHash;
    string sender;
    string recevier;
    float amount;
    long int nonce;
    bool proof;
    block *next;
};

class blockchin
{
    int difficulty;

public:
    block *Gen_block, *last_block;

    blockchin(int dif)
    {
        difficulty = dif;
        Gen_block = NULL;
        last_block = NULL;
    }
    void addPayment(string sender, string receiver, float amount)
    {

        block *new_block = new block;
        new_block = creatblock(sender, receiver, amount);
        if (Gen_block == NULL)
        {
            new_block->prevHash = "0000";
            new_block->hash = sha256(toStr(new_block));
            Gen_block = new_block;
            last_block = new_block;
        }
        else
        {
            new_block->prevHash = last_block->hash;
            new_block->hash = sha256(toStr(new_block));
            last_block->next = new_block;
            last_block = new_block;
        }
    }
    void showChain()
    {
        block *temp = Gen_block;
        while (temp != NULL)
        {

            cout << toStr(temp) << endl;
            cout << "Block Hash:" << temp->hash << endl;
            cout << endl;
            // cout << "Proof Of Work:" << temp->proof << endl;

            temp = temp->next;
        }
    }

    struct block *creatblock(string sender, string receiver, float amount)
    {
        block *new_block;
        new_block->sender = sender;
        new_block->recevier = receiver;
        new_block->amount = amount;
        new_block->nonce = 1;
        new_block->hash = "0";
        new_block->proof = false;
        new_block->next = NULL;
        return new_block;
    }
    string toStr(struct block *b)
    {
        string data = "NONCE =" + to_string(b->nonce) + "\n" + "Sender:" + b->sender + "\n" + "Recevier:" + b->recevier + "\n" + "Amount:" + to_string(b->amount) + "\n" + "Previous Hash:" + b->prevHash;
        return data;
    }

    // void mine_block(struct block *b)
    // {
    // }
    // bool isChainValid()
    // {
    // }
};

int main()
{

    return 0;
}