#include <stdio.h>
#include <sqlite3.h>
#include<string>
#include<iostream>
using namespace std;
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
int checkI(sqlite3 *db,int ID)
{
    //function to check investors existence
    sqlite3_stmt *stmt;
    int flag=0;
    string str="select * from INVESTORS where INV_id="+to_string(ID)+";";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    while(sqlite3_step(stmt)!=SQLITE_DONE)
        flag=1;
    return flag;
}
void disp_INV(sqlite3 *db,int ID)
{
    sqlite3_stmt *stmt;
    sqlite3_stmt *stmt2;
    sqlite3_stmt *stmt3;
    string str="select * from INVESTORS where INV_id="+to_string(ID)+";";
    string str2="select * from HOLDINGS where INV_id="+to_string(ID)+";";
    string str3="select * from TRANSACTIONS where INV_id="+to_string(ID)+";";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    sqlite3_prepare_v2(db, str2.c_str(), -1, &stmt2, 0);
    sqlite3_prepare_v2(db, str3.c_str(), -1, &stmt3, 0);
    
    //CODE TO GET DATA FROM TABLE INVESTORS
    const unsigned char *name;
    const unsigned char *email;
    const unsigned char *Jdate;
    float investment=0;
    cout<<"\n\t\t\t*****INVESTOR'S DATA*****"<<endl;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        name=sqlite3_column_text(stmt, 1);
        email=sqlite3_column_text(stmt, 2);
        investment=sqlite3_column_double(stmt, 3);
        Jdate=sqlite3_column_text(stmt, 4);
        cout<<"\n\tINVESTORS'S ID ==>> "<<ID<<endl;
        cout<<"\tINVESTORS'S NAME ==>> "<<name<<endl;
        cout<<"\tINVESTORS'S EMAIL-ID ==>> "<<email<<endl;
        cout<<"\tCURRENT INVESTMENT ==>> "<<investment<<endl;
        cout<<"\tJOINING DATE ==>> "<<Jdate<<endl;
    }
    
    //CODE TO GET DATA FROM TABLE HOLDINGS
    cout<<"\n\t\t\t*****INVESTOR'S HOLDINGS*****"<<endl;
    const unsigned char *stock;
    int qty;
    float avgP=0;
    int flag=0;
    while(sqlite3_step(stmt2)!=SQLITE_DONE)
    {
        flag=1;
        stock=sqlite3_column_text(stmt2, 1);
        qty=sqlite3_column_int(stmt2, 2);
        avgP=sqlite3_column_double(stmt2, 3);
        cout<<"\n\tSTOCK NAME ==>> "<<stock<<endl;
        cout<<"\tQUANTITY ==>> "<<qty<<endl;
        cout<<"\tAVERAGE PRICE ==>> "<<avgP<<endl;
    }
    if(flag==0)
    cout<<"\t\tNO HOLDINGS"<<endl;
    
    //CODE TO GET DATA FROM TABLE TRANSACTIONS
    flag=0;
    cout<<"\n\t\t\t*****INVESTOR'S TRANSACTIONS*****"<<endl;
    const unsigned char *Stock;
    int BS=-1;
    float price=0;
    while(sqlite3_step(stmt3)!=SQLITE_DONE)
    {
        flag=1;
        Stock=sqlite3_column_text(stmt3, 1);
        qty=sqlite3_column_int(stmt3, 2);
        BS=sqlite3_column_int(stmt3, 3);
        price=sqlite3_column_double(stmt3, 4);
        cout<<"\n\tSTOCK NAME ==>> "<<Stock<<endl;
        cout<<"\tQUANTITY ==>> "<<qty<<endl;
        cout<<"\tORDER TYPE ==>>";
        if(BS==0)
            cout<<"Sell"<<endl;
        else
            cout<<"BUY"<<endl;
        cout<<"\tPRICE ==>> "<<price<<endl;
    }
    if(flag==0)
        cout<<"\t\tNO TRANSACTIONS"<<endl;
    cout<<endl;
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmt3);
}
void add_INV(sqlite3 *db)
{
    cout<<"\n\t\t\t----------ADDING NEW INVESTOR----------"<<endl;
    cout<<"\n\t\tENTER THE FOLLOWING DETAILS\n";
    string name,mail,date;
    cin.ignore();
    cout<<"\t\tENTER NAME :";
    getline(cin, name);
    cout<<"\t\tENTER E-MAIL ID :";
    getline(cin,mail);
    cout<<"\t\tENTER DATE(YYYY-MM-DD) :";
    getline(cin,date);
    string str="insert into INVESTORS(NAME,EMAIL_ID,INVESTMENT,JoiningDate) values('"+name+"','"+mail+"',0,'"+date+"');";
    int rc=sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
    if(rc!=SQLITE_OK)
        cout<<"\n\t\t\t!!!!!ADDITION FAILED!!!!!"<<endl;
    sqlite3_stmt *stmt;
    string str2="select INV_id from INVESTORS where NAME='"+name+"';";
    sqlite3_prepare_v2(db, str2.c_str(), -1, &stmt, 0);
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    cout<<"\n\t\tID OF ADDED INVESTOR IS ==>> "<<sqlite3_column_int(stmt,0)<<endl;
    cout<<endl;
}
int del_INV(sqlite3 *db,int ID)
{
    //FIRST CHECK THE INVESTMENT BEFORE DELETION
    string str="select * from INVESTORS where INV_id="+to_string(ID)+";";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    float IMT=0;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
        IMT=sqlite3_column_double(stmt,3);
    if(IMT>0)
    {
        cout<<"\n\t\t\t!!!!!INVESTMENT IS NOT ZERO!!!!!"<<endl;
        cout<<"\n\t\t\t!!!!!SELL ALL STOCKS FIRST!!!!!\n"<<endl;
        return 0;
    }
    
    //IF INVESTMENT IS ZERO CONTINUE
    cout<<"\n\t\t---------DELETE INVESTOR---------"<<endl;
    cout<<"\tCONTINUE WITH THE DELETION?([Y/N]"<<endl;
    cout<<"\tENTER HERE - ";
    char ch;
    cin>>ch;
    if(ch=='N' || ch=='n')
        return 0;
    string str2="delete from INVESTORS where INV_id="+to_string(ID)+";";
    cout<<"\t\t***DETAILS OF DELETED INVESTOR***"<<endl;
    disp_INV(db, ID);
    sqlite3_exec(db,str2.c_str(),callback,NULL,NULL);
    return 1;
}
void alter_INV(sqlite3 *db,int ID)
{
    string s="";
    int choice=-1;
    flag:
    cout<<"\n\t\t\t**********UPDATE INVESTOR'S INFORMATION**********\n"<<endl;
    cout<<"\t\tSELECT AN OPTION"<<endl;
    cout<<"\t\t1.CHANGE NAME"<<endl;
    cout<<"\t\t2.CHANGE EMAIL-ID"<<endl;
    cout<<"\t\t3.CHANGE JOINING-DATE"<<endl;
    cout<<"\t\t4.GO BACK"<<endl;
    cout<<"\t\tENTER HERE - ";
    cin>>choice;
    switch(choice)
    {
        case 1:
        {
            cin.ignore();
            cout<<"\n\t\tENTER NEW NAME ==>> ";
            getline(cin,s);
            string str="update INVESTORS set NAME='"+s+"' where INV_id="+to_string(ID)+";";
            sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
            cout<<"\n\t\t\t*****NAME UPDATED SUCCESSFULLY*****"<<endl;
            goto flag;
        }
        case 2:
        {
            cin.ignore();
            cout<<"\n\t\tENTER NEW EMAIL-ID ==>> ";
            getline(cin,s);
            string str="update INVESTORS set EMAIL_ID='"+s+"' where INV_id="+to_string(ID)+";";
            sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
            cout<<"\n\t\t*****EMAIL_ID UPDATED SUCCESSFULLY*****"<<endl;
            goto flag;
        }
        case 3:
        {
            cin.ignore();
            cout<<"\n\t\tENTER JOINING-DATE(YYYY-MM-DD) ==>> ";
            getline(cin,s);
            string str="update INVESTORS set JoiningDate='"+s+"' where INV_id="+to_string(ID)+";";
            sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
            cout<<"\n\t\tJOINING DATE UPDATED SUCCESSFULLY"<<endl;
            goto flag;
        }
        case 4:break;
        default:cout<<"\n\t\t!!!!!INVALID CHOICE!!!!!"<<endl;
            goto flag;
    }
}

