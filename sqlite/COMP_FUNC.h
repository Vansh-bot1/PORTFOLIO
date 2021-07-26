#include <stdio.h>
#include <sqlite3.h>
#include<string>
#include<iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int check(sqlite3 *db,string symb)
{
    //function to companys existence
    sqlite3_stmt *stmt;
    int flag=0;
    string str="select * from COMPANIES where SYMBOL='"+symb+"';";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    while(sqlite3_step(stmt)!=SQLITE_DONE)
        flag=1;
    return flag;
}
void disp_all(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    string str="select * from COMPANIES;";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    const unsigned char *symb;
    int count=0;
    cout<<"\n\t\t-+-+-+-ALL COMPANY SYMBOLS-+-+-+-\n"<<endl;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        count++;
        symb=sqlite3_column_text(stmt, 1);
        cout<<count<<"--"<<symb<<endl;
    }
    sqlite3_finalize(stmt);
}
void disp_Comp(sqlite3 *db,string symb)
{
    sqlite3_stmt *stmt;
    string str="select * from COMPANIES where SYMBOL='"+symb+"';";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    const unsigned char *name;
    float Mcap=0;
    cout<<"\n\t\t-+-+-+-COMPANY INFORMATION-+-+-+-"<<endl;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        name=sqlite3_column_text(stmt, 0);
        Mcap=sqlite3_column_double(stmt, 2);
        cout<<"\n\tCOMPANY NAME ==> "<<name<<endl;
        cout<<"\n\tCOMPANY SYMBOL ==> "<<symb<<endl;
        cout<<fixed<<"\n\tCOMPANY MARKETCAP ==> Rs. "<<Mcap<<endl;
    }
    sqlite3_finalize(stmt);
}
void val(sqlite3 *db,string symb)
{
    sqlite3_stmt *stmt;
    sqlite3_stmt *stmt2;
    sqlite3_stmt *stmt3;
    string str="select * from CompPL where SYMBOL='"+symb+"(Sales)';";
    string str2="select * from CompPL where SYMBOL='"+symb+"(Profit)';";
    string str3="select * from CompPL where SYMBOL='"+symb+"(EPS)';";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    sqlite3_prepare_v2(db, str2.c_str(), -1, &stmt2, 0);
    sqlite3_prepare_v2(db, str3.c_str(), -1, &stmt3, 0);
    
    //sales CAGR calculation
    float firstS=0;
    float thirdS=0;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        firstS=sqlite3_column_double(stmt, 1);
        thirdS=sqlite3_column_double(stmt, 3);
    }
    float cagrS=0;
    cagrS=pow((thirdS/firstS),.33)-1;
    cagrS*=100;
    
    //profit CAGR calculation
    float firstP=0;
    float thirdP=0;
    while(sqlite3_step(stmt2)!=SQLITE_DONE)
    {
        firstP=sqlite3_column_double(stmt2, 1);
        thirdP=sqlite3_column_double(stmt2, 3);
    }
    float cagrP=0;
    cagrP=pow((thirdP/firstP),.33)-1;
    cagrP*=100;
    
    //INTRINSIC value & EPS calculation
    float thirdEPS=0;
    while(sqlite3_step(stmt3)!=SQLITE_DONE)
        thirdEPS=sqlite3_column_double(stmt3, 3);
    float value=0;
    value=((2*cagrS)+8.5)*4.4*thirdEPS/6;
    if(value<0)    //stocks intrinsic value cannot be negative so take it zero
        value=0;
    string s;
    if(cagrS>=14)
        s="VERY GOOD";
    else if(cagrS>=10 && cagrS<14)
        s="GOOD";
    else if(cagrS>=6 && cagrS<10)
        s="AVERAGE";
    else if(cagrS>=0 && cagrS<6)
        s="POOR";
    else s="NEGATIVE";
    cout<<"\n\t\t\t$$$$$$--VALUATION--$$$$$$"<<endl;
    cout<<setprecision(3)<<"\n\t\tCAGR in SALES ==>> "<<cagrS<<endl;
    cout<<setprecision(3)<<"\t\tCAGR in PROFIT ==>> "<<cagrP<<endl;
    cout<<setprecision(3)<<"\t\tEPS ==>> "<<thirdEPS<<endl;
    cout<<fixed<<"\t\tINTRINSIC VALUE ==>> "<<value<<endl;
    cout<<"\t\tGROWTH IS ==>> "<<s<<endl;
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmt3);
}
void Tmoney(sqlite3 *db,string symb)
{
    sqlite3_stmt *stmt;
    string str="select * from HOLDINGS where STOCK='"+symb+"';";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    double Tmon=0;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
        Tmon+=(sqlite3_column_double(stmt, 2))*(sqlite3_column_double(stmt, 3));
    cout<<fixed<<"\n\t\tTOTAL MONEY INVESTED BY THE INVESTORS IN "<<symb<<" IS ==>> Rs. "<<Tmon<<endl;
    sqlite3_finalize(stmt);
}
void INV_LIST(sqlite3 *db,string symb)
{
    sqlite3_stmt *stmt;
    string str="select * from HOLDINGS where STOCK='"+symb+"';";
    sqlite3_prepare_v2(db, str.c_str(), -1, &stmt, 0);
    int ID=-1;
    int count=0;
    cout<<"\t\t<><><><><>LIST OF INVESTORS<><><><><>"<<endl;;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        count++;
        ID=sqlite3_column_int(stmt, 0);
        cout<<"\n\t\t\t\t\t("<<count<<")";
        disp_INV(db, ID);
    }
    cout<<"\n\t\tTOTAL NUMBER OF INVESTORS ==>> "<<count<<endl;
}
