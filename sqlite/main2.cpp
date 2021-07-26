#include <stdio.h>
#include <sqlite3.h>
#include<string>
#include<iostream>
#include "INV_FUNC.h"
#include "COMP_FUNC.h"
#include "TRANS_FUNC.h"
#include "ADD_TRANS.h"
using namespace std;
void INVESTORS()
{
    int choice=-1;
    menu:
    cout<<"\n\t\t\t---------INVESTORS SECTION---------\n";
    cout<<"\n\t\tEnter your choice\n";
    cout<<"\t\t1.ADD INVESTOR\n";
    cout<<"\t\t2.DELETE INVESTOR\n";
    cout<<"\t\t3.UPDATE INVESTOR'S INFORMATION\n";
    cout<<"\t\t4.DISPLAY INFORMATION AND HOLDINGS\n";
    cout<<"\t\t5.GO BACK\n"<<endl;
    cout<<"\t\tENTER HERE - ";
    cin>>choice;
    sqlite3 *db;
    int rc;
    rc =sqlite3_open("DBMSp.db", &db);
    int ID=-1;
    if(choice==2 || choice==3 || choice==4)
    {
        cout<<"\n\t\tENTER INVESTOR ID : ";
        cin>>ID;
        //check for investors existence
        if(!checkI(db, ID))
        {cout<<"\n\t\t\t!!!!!INVESTOR DOES NOT EXIST!!!!!"<<endl;goto menu;}
    }
    switch(choice)
    {
        case 1:add_INV(db);
            goto menu;
        case 2:
        {
            int n=del_INV(db,ID);
            if(n==0)
                cout<<"\t\t\t!!!!!DELETION FAILED!!!!!"<<endl;
            else
                cout<<"\t\t\t!!!!!DELETION SUCCESSFULL!!!!!"<<endl;
            goto menu;
        }
        case 3:alter_INV(db,ID);
            goto menu;
        case 4:disp_INV(db,ID);
            goto menu;
        case 5:break;
        default:cout<<"\n\t\t!!!!INVALID CHOICE!!!!\n";
            goto menu;
    }
    sqlite3_close(db);
}
void COMPANIES()
{
    int choice=-1;
    menu:
    cout<<"\n\t\t\t---------COMPANIES SECTION---------\n";
    cout<<"\n\t\tEnter your choice\n";
    cout<<"\t\t1.DISPLAY COMPANY'S DATA\n";
    cout<<"\t\t2.VALUATIONS\n";
    cout<<"\t\t3.TOTAL MONEY INVESTED\n";
    cout<<"\t\t4.INVESTOR'S LIST\n";
    cout<<"\t\t5.DISPLAY ALL COMPANY SYMBOLS\n";
    cout<<"\t\t6.GO BACK\n";
    cout<<"\t\tENTER HERE - ";
    cin>>choice;
    string symb;
    sqlite3 *db;
    int rc;
    rc =sqlite3_open("DBMSp.db", &db);
    //check for company existence
    check:
    if(choice==1 ||choice==2||choice==3||choice==4)
    {
        cout<<"\n\tENTER COMPANY SYMBOL : ";
        cin>>symb;
        if(!check(db, symb))
        {cout<<"\n\t\t\t!!!!!INVALID SYMBOL!!!!!"<<endl;goto check;}
    }
    switch(choice)
    {
        case 1:disp_Comp(db, symb);
            goto menu;
        case 2:val(db,symb);
            goto menu;
        case 3:Tmoney(db, symb);
            goto menu;
        case 4:INV_LIST(db, symb);
            goto menu;
        case 5:disp_all(db);
            goto menu;
        case 6:break;
        default:cout<<"\n\t!!!!INVALID CHOICE!!!!\n";
            goto menu;
    }
    sqlite3_close(db);
}
void TRANSACTIONS()
{
    int choice=-1;
    menu:
    cout<<"\n\t\t\t---------TRANSACTIONS SECTION---------\n";
    cout<<"\n\t\tEnter your choice\n";
    cout<<"\t\t1.ADD A TRANSACTION\n";
    cout<<"\t\t2.DISPLAY INVESTOR'S TRANSACTIONS\n";
    cout<<"\t\t3.DISPLAY COMPANY'S TRANSACTIONS\n";
    cout<<"\t\t4.DISPLAY ALL TRANSACTIONS\n";
    cout<<"\t\t5.GO BACK\n";
    cout<<"\t\tENTER HERE - ";
    cin>>choice;
    sqlite3 *db;
    int ID=-1;
    string symb;
    sqlite3_open("DBMSp.db", &db);
    //check for investors existence
    if(choice==2)
    {
        checkI:
        cout<<"\t\tENTER THE INVESTOR ID :";
        cin>>ID;
        if(!checkI(db,ID))
        {cout<<"\t\t!!!!!ID DOES NOT EXIST!!!!!"<<endl;goto checkI;}
    }
    //check for companys existence
    if(choice==3)
    {
        checkC:
        cout<<"\t\tENTER THE COMPANY SYMBOL :";
        cin>>symb;
        if(!check(db,symb))
        {cout<<"\t\t!!!!!COMPANY DOES NOT EXIST!!!!!"<<endl;goto checkC;}
    }
    switch(choice)
    {
        case 1:add_TRANS(db);
            goto menu;
        case 2:disp_INV_TRANS(db,ID);
            goto menu;
        case 3:disp_COMP_TRANS(db,symb);
            goto menu;
        case 4:disp(db);
            goto menu;
        case 5:break;
        default:cout<<"\n\t!!!!INVALID CHOICE!!!!\n";
            goto menu;
    }
    sqlite3_close(db);
}
int main()
{
    int choice=-1;
    menu:
    cout<<"\t\t\t\t<<-----------WELCOME----------->>"<<endl;
    cout<<"\n\tENTER YOUR CHOICE\n";
    cout<<"\t1.INVESTORS\n"<<endl;
    cout<<"\t2.COMPANY\n"<<endl;
    cout<<"\t3.TRANSACTIONS\n"<<endl;
    cout<<"\t4.EXIT\t\n"<<endl;
    cout<<"\tENTER HERE - ";
    cin>>choice;
    switch(choice)
    {
        case 1:INVESTORS();
            goto menu;
        case 2:COMPANIES();
            goto menu;
        case 3:TRANSACTIONS();
            goto menu;
        case 4:
        {
            cout<<"\n\tARE YOU SURE YOU WANT TO EXIT?(Y/N)"<<endl;
            cout<<"\n\tENTER HERE - ";
            char ch;
            cin>>ch;
            if(ch=='Y' || ch=='y')
            break;
            else
            goto menu;
        }
        default:cout<<"\n\t!!!!INVALID CHOICE!!!!\n";
            goto menu;
    }
    cout<<"\n\t\t*-*-*-*-*-HAVE A NICE DAY SIR/MADAM-*-*-*-*-*\n"<<endl;
}
