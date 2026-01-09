#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
bool studentdrop(sqlite3* db,const std::string& studentID,const std::string& courseid)
{
    std::string tablename="Schedule"+studentID;
    std::string sql="delete from "+tablename+" where course_id=?;";

    std::string sqlstatus="select status from "+tablename+" where course_id=?;";
    sqlite3_stmt* stmt=nullptr;
    int rc=sqlite3_prepare_v2(db,sqlstatus.c_str(),-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        return false;
    }

    rc=sqlite3_step(stmt);
    const unsigned char* dbstatus=sqlite3_column_text(stmt,0);
    std::string status=dbstatus?reinterpret_cast<const char*>(dbstatus):"";
    sqlite3_finalize(stmt);


    stmt=nullptr;

    rc=sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_DONE)
    {
        sqlite3_finalize(stmt);

        sqlite3_stmt* stmt=nullptr;
        if(status=="confirmed")
        {
            std::string sqlupdate="update Course set student_number=student_number-1 where course_id=?;";
            int rc=sqlite3_prepare_v2(db,sqlupdate.c_str(),-1,&stmt,nullptr);
            if(rc!=SQLITE_OK)
            {
                std::cerr<<"Failed to compile student_numberSQL"<<sqlite3_errmsg(db)<<std::endl;
                return false;
            }
            rc=sqlite3_bind_text(stmt,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
            if(rc!=SQLITE_OK)
            {
                std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
                return false;
            }
            rc=sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }




        return true;
    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
}

