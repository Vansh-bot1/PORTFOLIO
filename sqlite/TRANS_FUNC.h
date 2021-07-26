#include <stdio.h>
#include <sqlite3.h>
#include<string>
#include<iostream>
using namespace std;

void disp_COMP_TRANS(sqlite3 *db,string symb)
{
    sqlite3_stmt *stmt;
    string str="select * from TRANSACTIONS where STOCK='"+symb+"';";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    int ID=-1;
    int qty=0;
    int bs=0;
    float price=0;
    string s;
    cout<<"\n\t\t\t*****--LIST OF TRANSACTIONS OF COMPANY "<<symb<<"--*****"<<endl;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        ID=sqlite3_column_int(stmt, 0);
        qty=sqlite3_column_int(stmt, 2);
        bs=sqlite3_column_int(stmt, 3);
        price=sqlite3_column_double(stmt, 4);
        if(bs==0)
            s="SELL";
        else
            s="BUY";
        cout<<"\n\t\tINVESTOR ID ==>> "<<ID<<endl;
        cout<<"\t\tTRADED QUANTITY ==>> "<<qty<<endl;
        cout<<"\t\tORDER TYPE ==>> "<<s<<endl;
        cout<<"\t\t"<<s<<" PRICE ==>> Rs."<<price<<endl;
    }
    sqlite3_finalize(stmt);
}
void disp_INV_TRANS(sqlite3 *db,int ID)
{
    sqlite3_stmt *stmt;
    string str="select * from TRANSACTIONS where INV_ID="+to_string(ID)+";";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    const unsigned char *Stock;
    int qty=0;
    int bs=0;
    float price=0;
    string s;
    cout<<"\n\t\t\t*****--LIST OF TRANSACTIONS OF INVESTOR ID  "<<ID<<"--*****"<<endl;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        Stock=sqlite3_column_text(stmt, 1);
        qty=sqlite3_column_int(stmt, 2);
        bs=sqlite3_column_int(stmt, 3);
        price=sqlite3_column_double(stmt, 4);
        if(bs==0)
            s="SELL";
        else
            s="BUY";
        cout<<"\n\t\tSTOCK SYMBOL ==>> "<<Stock<<endl;
        cout<<"\t\tTRADED QUANTITY ==>> "<<qty<<endl;
        cout<<"\t\tORDER TYPE ==>> "<<s<<endl;
        cout<<"\t\t"<<s<<" PRICE ==>> Rs."<<price<<endl;
    }
    sqlite3_finalize(stmt);
}
void disp(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    string str="select * from TRANSACTIONS;";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    const unsigned char *Stock;
    int ID=-1;
    int qty=0;
    int bs=0;
    float price=0;
    string s;
    cout<<"\n\t\t\t*****--LIST OF TRANSACTIONS--*****"<<endl;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        ID=sqlite3_column_int(stmt, 0);
        Stock=sqlite3_column_text(stmt, 1);
        qty=sqlite3_column_int(stmt, 2);
        bs=sqlite3_column_int(stmt, 3);
        price=sqlite3_column_double(stmt, 4);
        if(bs==0)
            s="SELL";
        else
            s="BUY";
        cout<<"\n\t\tINVESTOR ID ==>> "<<ID<<endl;
        cout<<"\t\tSTOCK SYMBOL ==>> "<<Stock<<endl;
        cout<<"\t\tTRADED QUANTITY ==>> "<<qty<<endl;
        cout<<"\t\tORDER TYPE ==>> "<<s<<endl;
        cout<<"\t\t"<<s<<" PRICE ==>> Rs."<<price<<endl;
    }
    sqlite3_finalize(stmt);
}
