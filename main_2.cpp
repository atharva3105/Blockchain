#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "sha256.h"
using namespace std;

// forward declaration

class blockchain;
class LocalBchain;
class PublicBchain;

// class block

class block
{

    string hash;
    string prevHash;
    string sender;
    string receiver;
    int amount;
    long int nonce;
    bool proof;
    block *next;
    friend class blockchain;
    friend class LocalBchain;
    friend class PublicBchain;
    friend ostream &operator<<(ostream &output, const block &B);

public:
    block()
    {
        this->sender = " ";
        this->receiver = " ";
        this->amount = 0;
        this->nonce = 1;
        this->proof = false;
        this->next = NULL;
        this->prevHash = " ";
        this->hash = " ";
    }
    block(string send, string recv, int amt, string prvhash = "0000")
    {
        transform(send.begin(), send.end(), send.begin(), ::toupper);
        transform(recv.begin(), recv.end(), recv.begin(), ::toupper);
        this->sender = send;
        this->receiver = recv;
        this->amount = amt;
        this->nonce = 1;
        this->proof = false;
        this->next = NULL;
        this->prevHash = prvhash;
        this->hash = sha256(string(*this));
    }
    void mine_block()
    {
        while (this->hash.substr(0, 4) != "0000")
        {
            nonce++;
            this->hash = sha256(string(*this));
        }
        if (this->hash.substr(0, 4) == "0000" && this->prevHash.substr(0, 4) == "0000")
        {
            this->proof = true;
        }
    }
    void writeInFile()
    {
    }
    operator string() const
    {
        string data = "NONCE =" + to_string(this->nonce) + "\n" + "Sender:" + this->sender + "\n" + "receiver:" + this->receiver + "\n" + "Amount:" + to_string(this->amount) + "\n" + "Previous Hash:" + this->prevHash + "\n";
        return data;
    }
};
ostream &operator<<(ostream &output, const block &B)
{
    output << string(B)
           << "Block Hash: " << B.hash << endl;
    return output;
}

// Abstract class BLOCKCHAIN CLASS

class blockchain
{

public:
    virtual void display()
    {
    }
    virtual void search(string, string)
    {
    }
};

// LOCAL BLCOKCHAIN CLASS

class LocalBchain : public blockchain
{
    vector<block *> BchainVec;

public:
    // LocalBchain()
    // {

    // }
    bool isempty()
    {
        if (BchainVec.empty())
            return true;
        return false;
    }
    void add_payment(string send, string recv, float amt)
    {
        if (isempty())
        {

            BchainVec.push_back(new block(send, recv, amt));
        }
        else
        {
            block *last;
            last = BchainVec.back();
            BchainVec.push_back(new block(send, recv, amt, last->hash));
        }
    }

    void mineChain()
    {

        if (isempty())
        {
            cout << "***Blockchain is empty***" << endl;
        }
        else
        {
            if (isChainValid())
            {
                cout << "Blockchain is already mined" << endl;
            }
            else
            {
                cout << "Mining..." << endl;
                int size = BchainVec.size();
                for (int i = this->isValid(); i < size; i++)
                {

                    BchainVec[i]->mine_block();

                    if (i + 1 != size)
                    {
                        BchainVec[i + 1]->prevHash = BchainVec[i]->hash;
                    }
                }
            }
        }
    }
    bool isChainValid()
    {
        if (this->isValid() == BchainVec.size())
            return true;
        return false;
    }
    void display()
    {
        if (isempty())
        {
            cout << "***Blockchain is empty***" << endl;
        }
        else
        {
            int i = 1;
            // cout << "Blockchain :" << endl;
            for (block *const &temp : BchainVec)
            {
                cout << "*****BLOCK" << i << "*****" << endl;
                cout << (*temp) << endl;
                i++;
            }
        }
    }
    int isValid()
    {
        int valid = 0;
        int size = BchainVec.size();

        for (int i = 0; i < size; i++)
        {
            if (BchainVec[i]->proof)
            {
                if (i != 0)
                {
                    if (BchainVec[i]->prevHash == BchainVec[i - 1]->hash)
                    {
                    }
                    else
                        return i;
                }
            }
            else
                return i;
        }
        return size;
    }

    void EditData(int Block_NO, string send, string recv, float amt)
    {
        int i = 0;
        int size = BchainVec.size();
        if (Block_NO <= size)
        {
            for (int i = 0; i < size; i++)
            {
                if (i + 1 == Block_NO)
                {
                    if (amt == -99)

                    {
                        BchainVec[i]->amount = BchainVec[i]->amount;
                    }
                    else
                    {
                        BchainVec[i]->amount = amt;
                    }
                    if (send == "NULL")
                    {
                        BchainVec[i]->sender = BchainVec[i]->sender;
                    }
                    else
                    {
                        BchainVec[i]->sender = send;
                    }
                    if (recv == "NULL")
                    {
                        BchainVec[i]->receiver = BchainVec[i]->receiver;
                    }
                    else
                    {
                        BchainVec[i]->receiver = recv;
                    }
                    BchainVec[i]->hash = sha256(string(*BchainVec[i]));
                    BchainVec[i]->proof = false;
                }
                if (i + 1 > Block_NO && i != 0)
                {

                    BchainVec[i]->prevHash = BchainVec[i - 1]->hash;
                    BchainVec[i]->hash = sha256(string(*BchainVec[i]));
                }
            }
            cout << "DATA CHANGED" << endl;
        }
        else
        {
            cout << "***BLOCKCHAIN ONLY HAVE " << size << " BLOCKS***" << endl;
        }
    }
    void search(string s, string r)
    {
        if (isempty())
        {
            cout << "***Blockchain is empty***" << endl;
        }
        else
        {
            int i = 1;
            bool found = false;
            transform(s.begin(), s.end(), s.begin(), ::toupper);
            transform(r.begin(), r.end(), r.begin(), ::toupper);
            for (block *&temp : BchainVec)
            {
                if (s != "NULL" && r != "NULL")
                {
                    if (temp->receiver == r && temp->sender == s)
                    {
                        cout << (*temp) << endl;
                        found = true;
                    }
                }
                else
                {
                    if (temp->receiver == r || temp->sender == s)
                    {
                        cout << (*temp) << endl;
                        found = true;
                    }
                }
            }
            if (!found)
            {
                cout << "***NO MATCH FOUND***" << endl;
            }
        }
    }
    void addToPublicBC()
    {
        if (isempty())
        {
            cout << "***Blockchain is empty***" << endl;
        }
        else
        {
            if (!isChainValid())
            {
                cout << "***BLOCKCHAIN IS NOT VALID , PROOF OF WORK NOT FOUND***" << endl;
                cout << "***MINE THE BLOCKCHAIN ***" << endl;
            }
            else
            {
                fstream f;

                f.open("BC.dat", ios::app | ios::in | ios::binary | ios::out);
                if (!f.is_open())
                {
                    cout << "***ERROR OCCURED WHILE OPENING FILE***" << endl;
                }
                else
                {
                    for (block *&temp : BchainVec)
                    {
                        f.write((char *)temp, sizeof(*temp));
                    }
                    cout << "BLOCKCHAIN ADDED TO PUBLIC BLOCK-CHAIN" << endl;
                    BchainVec.clear(); // emptying the local blockchain
                    f.close();
                }
            }
        }
    }
};

// PUBLIC BLOCKCHAIN CLASS

class PublicBchain : public blockchain
{
public:
    void display()
    {
        block temp;

        fstream f;

        f.open("BC.dat", ios::app | ios::out | ios::binary | ios::in);
        if (!f.is_open())
        {
            cout << "***ERROR WHILE OPENING FILE***" << endl;
        }
        else
        {
            int i = 1;
            while ((f.read((char *)&temp, sizeof(temp))))
            {
                cout << "*****BLOCK" << i << "*****" << endl;
                cout << (temp) << endl;
                i++;
            }
            f.close();
        }
    }
    void search(string s, string r)
    {
        block temp;

        fstream f;

        f.open("BC.dat", ios::app | ios::out | ios::binary | ios::in);
        if (!f.is_open())
        {
            cout << "***ERROR WHILE OPENING FILE***" << endl;
        }
        else
        {
            int i = 1;
            bool found = false;
            transform(s.begin(), s.end(), s.begin(), ::toupper);
            transform(r.begin(), r.end(), r.begin(), ::toupper);
            while ((f.read((char *)&temp, sizeof(temp))))
            {
                if (s != "NULL" && r != "NULL")
                {
                    if (temp.receiver == r && temp.sender == s)
                    {
                        cout << (temp) << endl;
                        found = true;
                    }
                }
                else
                {
                    if (temp.receiver == r || temp.sender == s)
                    {
                        cout << (temp) << endl;
                        found = true;
                    }
                }
            }
            if (!found)
            {
                cout << "***NO MATCH FOUND***" << endl;
            }
            f.close();
        }
    }
};
int main()
{

    LocalBchain lb;
    PublicBchain pb;
    blockchain *b;
    int ch, amt, n, bn;
    string r, s;
    while (ch != 7)
    {
        cout << endl;
        system("pause");
        system("cls");
        cout << "***MENU***" << endl;
        cout << "1. ADD PAYMENT " << endl;
        cout << "2. DISPLAY BLOCKCHAIN " << endl;
        cout << "3. MINE BLOCKCHAIN " << endl;
        cout << "4. SEARCH " << endl;
        cout << "5. EDIT DATA IN LOCAL BLOCKCHAIN " << endl;
        cout << "6. ADD BLOCKCHIAN TO PUBLIC BLOCKCHAIN " << endl;
        cout << "7. EXIT " << endl;
        cin >> ch;
        switch (ch)
        {
        case 1:
            cout << "ENTER RECEIVER NAME :" << endl;
            cin >> r;
            cout << "ENTER SENDER NAME :" << endl;
            cin >> s;
            cout << "ENTER AMOUNT :" << endl;
            cin >> amt;
            lb.add_payment(s, r, amt);
            cout << "BLOCK ADDED" << endl;

            break;
        case 2:
            cout << "    ***DISPLAY***" << endl;
            cout << "1. LOCAL BLOCKCHAIN:" << endl;
            cout << "2. PUBLIC BLOCKCHAIN:" << endl;
            cin >> n;
            if (n == 2)
                b = &pb;
            else
                b = &lb;
            b->display();

            break;
        case 3:
            lb.mineChain();

            break;
        case 4:
            cout << "    ***SEARCH***" << endl;
            cout << "1. LOCAL BLOCKCHAIN:" << endl;
            cout << "2. PUBLIC BLOCKCHAIN:" << endl;
            cin >> n;
            if (n == 2)
                b = &pb;
            else
                b = &lb;
            cout << "1. SEARCH WITH SENDER NAME:" << endl;
            cout << "2. SEARCH WITH RECEIVER NAME:" << endl;
            cout << "3. SEARCH WITH BOTH" << endl;
            cin >> n;
            if (n == 1)
            {
                cout << "ENTER SENDER NAME :" << endl;
                r = "NULL";
                cin >> s;
            }
            else if (n == 2)
            {
                cout << "ENTER RECEIVER NAME :" << endl;
                s = "NULL";
                cin >> r;
            }
            else
            {
                cout << "ENTER RECEIVER NAME :" << endl;
                cin >> r;
                cout << "ENTER SENDER NAME :" << endl;
                cin >> s;
            }
            b->search(s, r);

            break;
        case 5:
            cout << "ENTER BLOCK NUMBER" << endl;
            cin >> amt;
            cout << "1. EDIT SENDER NAME:" << endl;
            cout << "2. EDIT RECEIVER NAME:" << endl;
            cout << "3. EDIT AMOUNT" << endl;
            if (n == 1)
            {
                cout << "ENTER SENDER NAME :" << endl;
                r = "NULL";
                amt = -99;
                cin >> s;
            }
            else if (n == 2)
            {
                cout << "ENTER RECEIVER NAME :" << endl;
                s = "NULL";
                amt = -99;
                cin >> r;
            }
            else
            {
                cout << "ENTER AMOUNT :" << endl;
                s = "NULL";
                r = "NULL";
                cin >> amt;
            }
            lb.EditData(bn, s, r, amt);

            break;
        case 6:
            lb.addToPublicBC();
            break;
        case 7:
            cout << "Exiting . . ." << endl;
            break;

        default:
            cout << "CHOICE DOES NOT EXISTS" << endl;
            break;
        }
    }

    return 0;
}