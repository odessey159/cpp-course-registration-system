#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
bool adminprogress(sqlite3* db,const std::string& studentID,const std::string& courseid,const std::string& status)
{
    std::string tablename="Schedule"+studentID;
    std::string sql="update "+tablename+" set status=? where course_id=?;";



    sqlite3_stmt* stmt=nullptr;

    int rc=sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,status.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        return false;
    }
    rc=sqlite3_bind_text(stmt,2,courseid.c_str(),-1,SQLITE_TRANSIENT);
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
            std::string sqlupdate="update Course set student_number=student_number+1 where course_id=?;";
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

