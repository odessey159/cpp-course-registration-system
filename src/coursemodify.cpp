#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"

bool coursemodify(sqlite3* db,const std::string& courseID,const std::string& credits,std::string& coursetitle,std::string& department,std::string& location)
{
    const char* sql=
        "update Course " 
        "set course_name=?, credits=?, department=?, class_location=? "
        "where course_id=?";

    sqlite3_stmt* stmt=nullptr;

    int rc1=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc1=sqlite3_bind_text(stmt,5,courseID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,1,coursetitle.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,2,credits.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,3,department.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,4,location.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc1=sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    return true;
}