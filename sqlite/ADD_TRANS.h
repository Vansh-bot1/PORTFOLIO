#include <stdio.h>
#include <sqlite3.h>
#include<string>
#include<iostream>
using namespace std;

void add_TRANS(sqlite3 *db)
{
    //FIRST INSERT THE TRANSACTION
    cout<<"\n\t\t\t----------ADDING NEW TRANSACTION----------"<<endl;
    cout<<"\n\t\tENTER THE FOLLOWING DETAILS\n";
    string stock;
    int ID,Tqty;
    int bs;
    string s;
    float price=0;
    invI:
    cout<<"\t\tENTER INVESTOR ID :";
    cin>>ID;
    if(!checkI(db,ID))
    {cout<<"\n\t\t!!!INVESTOR DOES NOT EXIST!!!"<<endl;goto invI;}
    compI:
    cout<<"\t\tENTER STOCK SYMBOL :";
    cin>>stock;
    if(!check(db,stock))
    {cout<<"\n\t\t!!!!!COMPANY DOES NOT EXIST!!!!!"<<endl;goto compI;}
    cout<<"\t\tENTER QUANTITY :";
    cin>>Tqty;
    check:
    cout<<"\t\tENTER ORDER TYPE (1=BUY/0=SELL) :";
    cin>>bs;
    if(bs==1)
        s="BUY";
    else if(bs==0)
        s="SELL";
    else
    {
        cout<<"\n\t\t!!!INVALID ORDER TYPE!!! "<<endl;
        goto check;
    }
    if(s=="SELL")
    {
        sqlite3_stmt *stmt;
        string str="select * from HOLDINGS where INV_id="+to_string(ID)+" and STOCK='"+stock+"';";
        sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
        int flag=0;
            //IF FEW STOCKS ARE ALREADY PRESENT
        while(sqlite3_step(stmt)!=SQLITE_DONE)
        {
            flag=1;
            int qty=sqlite3_column_int(stmt, 2);
            if(qty<Tqty)
            {
                cout<<"\n\t\t\tONLY "<<qty<<" STOCKs ARE PRESENT\n"<<endl;
                goto invI;
            }
        }
        if(flag==0)
        {
            cout<<"\n\t\t\t!!!!!NO STOCKS ARE THERE IN HOLDINGS!!!!!\n"<<endl;
            goto invI;
        }
    }
    checkP:
    cout<<"\t\tENTER "<<s<<" PRICE :";
    cin>>price;
    if(price<0)
    {cout<<"\n\t\t!!!!!PRICE CANNOT BE NEGATIVE!!!!!"<<endl;goto checkP;}
    string str="insert into TRANSACTIONS values("+to_string(ID)+",'"+stock+"',"+to_string(Tqty)+","+to_string(bs)+","+to_string(price)+");";
    sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
    float amt1=Tqty*price;
    
    //NOW UPDATING THE HOLDING FOR BUY
    if(s=="BUY")
    {
    sqlite3_stmt *stmt;
    string str2="select * from HOLDINGS where INV_id="+to_string(ID)+" and STOCK='"+stock+"';";
    sqlite3_prepare_v2(db, str2.c_str(), -1, &stmt, 0);
    int flag=0;
        //IF FEW STOCKS ARE ALREADY PRESENT
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        flag=1;
        int qty=sqlite3_column_int(stmt, 2);
        float price=sqlite3_column_double(stmt, 3);
        float amt2=qty*price;
        Tqty+=qty;
        float avgP=(amt1+amt2)/Tqty;
        string str="update HOLDINGS set QTY="+to_string(Tqty)+" where INV_id="+to_string(ID)+" and STOCK='"+stock+"';";
        sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
        string str2="update HOLDINGS set AVG_Price="+to_string(avgP)+" where INV_id="+to_string(ID)+" and STOCK='"+stock+"';";
        sqlite3_exec(db,str2.c_str(),callback,NULL,NULL);
    }
        //IF STOCK IS BOUGHT FIRST TIME
    if(flag==0)
    {
        string strH="insert into HOLDINGS values("+to_string(ID)+",'"+stock+"',"+to_string(Tqty)+","+to_string(price)+");";
        sqlite3_exec(db,strH.c_str(),callback,NULL,NULL);
    }
        //UPDATING INVESTOR'S INVESTMENT FOR BUY
    string str3="update INVESTORS set INVESTMENT=INVESTMENT+"+to_string(amt1)+" where INV_id="+to_string(ID)+";";
        sqlite3_exec(db,str3.c_str(),callback,NULL,NULL);
    }
    
    //NOW UPDATING THE HOLDING FOR SELL
    if(s=="SELL")
    {
        sqlite3_stmt *stmt;
        string str="select * from HOLDINGS where INV_id="+to_string(ID)+" and STOCK='"+stock+"';";
        sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
            //IF FEW STOCKS ARE ALREADY PRESENT
        float amt1=0;
        while(sqlite3_step(stmt)!=SQLITE_DONE)
        {
            float avgP=sqlite3_column_double(stmt, 3);
            amt1=Tqty*avgP;
            string str="update HOLDINGS set QTY=QTY-"+to_string(Tqty)+" where INV_id="+to_string(ID)+" and STOCK='"+stock+"';";
            sqlite3_exec(db,str.c_str(),callback,NULL,NULL);
        }
        string str2="update INVESTORS set INVESTMENT=INVESTMENT-"+to_string(amt1)+" where INV_id="+to_string(ID)+";";
        sqlite3_exec(db,str2.c_str(),callback,NULL,NULL);
    }
}
